#define _COMMON_CWINDOW_H_
#define _COMMON_COBLIST_H_
#include "common.h"

#include "wargame.h"

#include "control.hpp"
#include "board.hpp"

#include <stdio.h>  // for sprintf

PQUICKPROCESSHEADER(CDisplayPieceWindow)
{
	STARTPROCESSING()
		PONPAINT()
	ENDPROCESSING()
}

PQUICKPROCESSHEADER(CDisplayPositionWindow)
{
	STARTPROCESSING()
		PONPAINT()
	ENDPROCESSING()
}

CDisplayPieceWindow::CDisplayPieceWindow()
{
	m_Piece = BITID_FIELD;
}

CDisplayPositionWindow::CDisplayPositionWindow()
{
	m_XPos = 1;
	m_YPos = 1;
	m_Text[0] = 0;
}

void CDisplayPieceWindow::Update()
{
	LONG x1, y1, x2, y2;

	GetWindowRect(x1, y1, x2, y2);
	InvalidateRect(x1, y1, x2, y2, FALSE);
}

BOOL CDisplayPieceWindow::OnPaint(MRESULT &)
{
	RECTL rect;

	GetWindowRect(rect);

	HPS hps;

	hps = BeginPaint(NULLHANDLE, NULL);

	DisplayStretchBitmap(hps, m_Piece, rect);

	EndPaint(hps);

	return TRUE;
}

void CDisplayPositionWindow::Update()
{
	LONG x = (LONG)m_XPos + 1;
	LONG y = (LONG)m_YPos + 1;

	sprintf(m_Text, "(%ld,%ld)", x, y);

	LONG x1, y1, x2, y2;

	GetWindowRect(x1, y1, x2, y2);
	InvalidateRect(x1, y1, x2, y2, FALSE);
}

BOOL CDisplayPositionWindow::OnPaint(MRESULT &)
{
	RECTL rect;

	GetWindowRect(rect);

	HPS hps;

	hps = BeginPaint(NULLHANDLE, NULL);

	WinDrawText(hps, -1, m_Text, &rect, CLR_NEUTRAL, CLR_BACKGROUND,
					DT_CENTER | DT_VCENTER | DT_ERASERECT);

	EndPaint(hps);

	return TRUE;
}

PPROCESSHEADER(CControlDlg)
{
	STARTPROCESSING()
		PONINITDLG()

		PONCHAR()

		STARTCMDPROCESSING()
			PONCMDPUSHBUTTON()
		ENDCMDPROCESSING()

		STARTCONTROLPROCESSING()

			INCLUDEIDPROCESSING(CID_CC_TRACK_WEAPON)
			STARTCONTROLIDPROCESSING()
				PONBUTTONCLICKED()
			ENDCONTROLIDPROCESSING()

		ENDCONTROLPROCESSING()
	ENDPROCESSING()
}

CControlDlg::CControlDlg()
{
}

BOOL CControlDlg::OnInitDlg(MRESULT &, CWindow, PVOID)
{
	GetChild(m_CPTPosition, TID_CC_XY_CP);
	GetChild(m_DTPosition, TID_CC_XY_D);

	GetChild(m_CPPosition, UID_CC_XY_CP);
	GetChild(m_DPosition, UID_CC_XY_D);

	GetChild(m_CPTAttack, TID_CC_ATTACK_CP);
	GetChild(m_CPTDefense, TID_CC_DEFENSE_CP);

	GetChild(m_DTAttack, TID_CC_ATTACK_D);
	GetChild(m_DTDefense, TID_CC_DEFENSE_D);

	GetChild(m_CPAttack, SLID_CC_ATTACK_CP);
	GetChild(m_CPDefense, SLID_CC_DEFENSE_CP);

	GetChild(m_DAttack, SLID_CC_ATTACK_D);
	GetChild(m_DDefense, SLID_CC_DEFENSE_D);

	GetChild(m_CPPiece, UID_CC_BIT_CP);
	GetChild(m_DPiece, UID_CC_BIT_D);

	GetChild(m_AttackFighter, CID_CC_TRACK_WEAPON);
	GetChild(m_Move, BID_OKAY);
	GetChild(m_Find, BID_FIND);

	BOOL m_AttackingFighter = FALSE;

	m_CPPosition.Setup();
	m_DPosition.Setup();
	m_CPPiece.Setup();
	m_DPiece.Setup();

	HideAll();
	return FALSE;
}

//BOOL CControlDlg::OnChar(MRESULT &, USHORT flags, UCHAR, UCHAR, USHORT, USHORT Virtual)
BOOL CControlDlg::OnChar(MRESULT &, USHORT flags, UCHAR, UCHAR, USHORT Letter, USHORT Virtual)
{
	if (flags & KC_KEYUP)
		return FALSE;

	if (Virtual == VK_ESC)
		return TRUE;

	if (flags & KC_CTRL)
	{
		CWindow Owner;
		if ((Letter == 'f')||(Letter == 'F'))
		{
			GetOwner(Owner);
			Owner.SendMsg(WM_COMMAND, MPFROMLONG(BID_FIND), MPFROM2SHORT((USHORT)CMDSRC_OTHER, (USHORT)FALSE));
			return TRUE;
		}

		if ((Letter == 't')||(Letter == 'T'))
		{
			GetOwner(Owner);
			Owner.SendMsg(WM_COMMAND, MPFROMLONG(DID_CONTROL_CENTER), MPFROM2SHORT((USHORT)CMDSRC_OTHER, (USHORT)FALSE));
			return TRUE;
		}
	}

	return FALSE;
}

void CControlDlg::HideAll()
{
	m_CPTPosition.Hide(NULL);
	m_DTPosition.Hide(NULL);
	m_CPPosition.Hide(NULL);
	m_DPosition.Hide(NULL);

	m_CPTAttack.Hide(NULL);
	m_CPTDefense.Hide(NULL);
	m_DTAttack.Hide(NULL);
	m_DTDefense.Hide(NULL);

	m_CPAttack.Hide(NULL);
	m_CPDefense.Hide(NULL);
	m_DAttack.Hide(NULL);
	m_DDefense.Hide(NULL);

	m_CPPiece.Hide(NULL);
	m_DPiece.Hide(NULL);
	m_Move.Hide(NULL);
	m_Find.Hide(NULL);

	m_AttackFighter.Hide(NULL);
}

BOOL CControlDlg::OnButtonClicked(MRESULT &, USHORT Which)
{
	switch(Which)
	{
		case CID_CC_TRACK_WEAPON:
							m_AttackFighter.SetCheck((USHORT)m_AttackingFighter);
							return TRUE;
	}
	return FALSE;
}

BOOL CControlDlg::OnCmdPushButton(MRESULT &, USHORT Which, BOOL)
{
	CWindow Owner;
	switch(Which)
	{
		case BID_OKAY:
							GetOwner(Owner);
							Owner.SendMsg(WM_COMMAND, MPFROMLONG(DID_CONTROL_CENTER), MPFROM2SHORT((USHORT)CMDSRC_OTHER, (USHORT)FALSE));
							return TRUE;
		case BID_FIND:
							GetOwner(Owner);
							Owner.SendMsg(WM_COMMAND, MPFROMLONG(BID_FIND), MPFROM2SHORT((USHORT)CMDSRC_OTHER, (USHORT)FALSE));
							return TRUE;
	}
	return FALSE;
}

void CControlDlg::Set(SHORT CPXPos, SHORT CPYPos,
							 BOOL CPHasFighter, USHORT CPAttack, USHORT CPDefense, ULONG CPPiece,
							 BOOL DHasDest, SHORT DXPos, SHORT DYPos,
							 BOOL DHasFighter, USHORT DAttack, USHORT DDefense, ULONG DPiece,
							 BOOL DAttackingFighter)
{
	m_AttackingFighter = DAttackingFighter;

	m_CPTPosition.Show(NULL);

	m_CPPosition.Set(CPXPos, CPYPos);
	m_CPPosition.Show(NULL);

	m_CPPiece.Set(CPPiece);
	m_CPPiece.Show(NULL);

	m_Move.Show(NULL);
	m_Find.Show(NULL);

	if (CPHasFighter)
	{
		m_CPTAttack.Show(NULL);
		m_CPTDefense.Show(NULL);

		USHORT AttackP = (USHORT)(((LONG)CPAttack * 100) / (LONG)GetStrongest());
		USHORT DefenseP = (USHORT)(((LONG)CPDefense * 100) / (LONG)GetStrongest());

		AttackP = (USHORT)min(AttackP, 99);
		DefenseP = (USHORT)min(DefenseP, 99);

		m_CPAttack.SetArmPosition(AttackP);
		m_CPAttack.Show(NULL);

		m_CPDefense.SetArmPosition(DefenseP);
		m_CPDefense.Show(NULL);
	}
	else
	{
		m_CPAttack.Hide(NULL);
		m_CPDefense.Hide(NULL);

		m_CPTAttack.Hide(NULL);
		m_CPTDefense.Hide(NULL);
	}

	if (DHasDest)
	{
		m_DTPosition.Show(NULL);
		m_DPosition.Set(DXPos, DYPos);
		m_DPosition.Show(NULL);

		m_DPiece.Set(DPiece);
		m_DPiece.Show(NULL);

		if (DHasFighter)
		{
			m_DTAttack.Show(NULL);
			m_DTDefense.Show(NULL);

			USHORT AttackP = (USHORT)(((LONG)DAttack * 100) / (LONG)GetStrongest());
			USHORT DefenseP = (USHORT)(((LONG)DDefense * 100) / (LONG)GetStrongest());

			AttackP = (USHORT)min(AttackP, 99);
			DefenseP = (USHORT)min(DefenseP, 99);

			m_DAttack.SetArmPosition(AttackP);
			m_DAttack.Show(NULL);

			m_DDefense.SetArmPosition(DefenseP);
			m_DDefense.Show(NULL);

			m_AttackFighter.SetCheck((USHORT)DAttackingFighter);
			m_AttackFighter.Show(NULL);
		}
		else
		{
			m_DTAttack.Hide(NULL);
			m_DTDefense.Hide(NULL);

			m_DAttack.Hide(NULL);
			m_DDefense.Hide(NULL);

			m_AttackFighter.Hide(NULL);
		}
	}
	else
	{
		m_DTPosition.Hide(NULL);
		m_DPosition.Hide(NULL);

		m_DTAttack.Hide(NULL);
		m_DTDefense.Hide(NULL);

		m_DAttack.Hide(NULL);
		m_DDefense.Hide(NULL);

		m_DPiece.Hide(NULL);

		m_AttackFighter.Hide(NULL);
	}
}

