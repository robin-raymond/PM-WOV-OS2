#define _COMMON_CWINDOW_H_
#define _COMMON_COBLIST_H_
#include "common.h"

#include "wargame.h"

#include "board.hpp"
#include "gameover.hpp"
#include "hiscore.hpp"

#include <stdio.h>  // for sprintf

PQUICKPROCESSHEADER(CDisplayGamePieceWindow)
{
	STARTPROCESSING()
		PONPAINT()
	ENDPROCESSING()
}

PQUICKPROCESSHEADER(CDisplayDestroyedWindow)
{
	STARTPROCESSING()
		PONPAINT()
	ENDPROCESSING()
}

PQUICKPROCESSHEADER(CDisplayClassWindow)
{
	STARTPROCESSING()
		PONPAINT()
	ENDPROCESSING()
}

void CDisplayGamePieceWindow::Update()
{
	LONG x1, y1, x2, y2;

	GetWindowRect(x1, y1, x2, y2);
	InvalidateRect(x1, y1, x2, y2, FALSE);
}

BOOL CDisplayGamePieceWindow::OnPaint(MRESULT &)
{
	RECTL rect;

	GetWindowRect(rect);

	HPS hps;

	hps = BeginPaint(NULLHANDLE, NULL);

	DisplayStretchBitmap(hps, m_Piece, rect);

	EndPaint(hps);

	return TRUE;
}

void CDisplayDestroyedWindow::Update()
{
	sprintf(m_Text, "%ld", m_Number);

	LONG x1, y1, x2, y2;

	GetWindowRect(x1, y1, x2, y2);
	InvalidateRect(x1, y1, x2, y2, FALSE);
}

BOOL CDisplayDestroyedWindow::OnPaint(MRESULT &)
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

void CDisplayClassWindow::Update()
{
	LONG x1, y1, x2, y2;

	GetWindowRect(x1, y1, x2, y2);
	InvalidateRect(x1, y1, x2, y2, FALSE);
}

BOOL CDisplayClassWindow::OnPaint(MRESULT &)
{
	RECTL rect;

	GetWindowRect(rect);

	HPS hps;

	hps = BeginPaint(NULLHANDLE, NULL);

	WinDrawText(hps, -1, m_Actual, &rect, CLR_NEUTRAL, CLR_BACKGROUND,
					DT_CENTER | DT_VCENTER | DT_ERASERECT);

	EndPaint(hps);

	return TRUE;
}

PPROCESSHEADER(CAskInitialsDlg)
{
	STARTPROCESSING()
		PONINITDLG()
//		PONCHAR()

		STARTCMDPROCESSING()
			PONCMDPUSHBUTTON()
		ENDCMDPROCESSING()

	ENDPROCESSING()
}
/*
BOOL CAskInitialsDlg::OnChar(MRESULT &, USHORT flags, UCHAR, UCHAR, USHORT, USHORT Virtual)
{
//	if (flags & KC_KEYUP)
//		return FALSE;

	if (Virtual == VK_ESC)
		return TRUE;

	return FALSE;
}*/

BOOL CAskInitialsDlg::OnInitDlg(MRESULT &, CWindow, PVOID)
{
	GetChild(m_Initials, FID_INITIALS);

	GetChild(m_Player1, TID_PLAYER_1);
	GetChild(m_Player2, TID_PLAYER_2);

	if (m_Player == 1)
		m_Player2.Hide(NULL);
	else
		m_Player1.Hide(NULL);

	return FALSE;
}

void CAskInitialsDlg::FillBuffer()
{
	char Sour[10];

	m_Initials.GetWindowText((PSZ)Sour, (ULONG)sizeof(Sour));

	if (*Sour)
	{
		m_Buffer[0] = (*Sour);
		if (*(Sour + 1))
		{
			m_Buffer[1] = (*(Sour + 1));
			if (*(Sour + 2))
				m_Buffer[2] = (*(Sour + 2));
		}
	}
}

BOOL CAskInitialsDlg::OnCmdPushButton(MRESULT &, USHORT Which, BOOL)
{
	switch(Which)
	{
		case BID_OKAY:
						FillBuffer();
						DismissDlg(TRUE);
						return TRUE;
	}
	return FALSE;
}





PPROCESSHEADER(CHighScoresDlg)
{
	STARTPROCESSING()
		PONINITDLG()

		STARTCMDPROCESSING()
			PONCMDPUSHBUTTON()
		ENDCMDPROCESSING()

	ENDPROCESSING()
}

CHighScoresDlg::CHighScoresDlg()
{
}

BOOL CHighScoresDlg::OnInitDlg(MRESULT &, CWindow, PVOID)
{
	GetChild(m_IRank1, UID_ADMIRAL_S);
	GetChild(m_IRank2, UID_SARGENT_MAJOR_S);
	GetChild(m_IRank3, UID_SARGENT_S);
	GetChild(m_IRank4, UID_CORPORAL_S);
	GetChild(m_IRank5, UID_PRIVATE_1_S);
	GetChild(m_IRank6, UID_PRIVATE_2_S);
	GetChild(m_IRank7, UID_PRIVATE_3_S);

	GetChild(m_PRank1, UID_ADMIRAL_P);
	GetChild(m_PRank2, UID_SARGENT_MAJOR_P);
	GetChild(m_PRank3, UID_SARGENT_P);
	GetChild(m_PRank4, UID_CORPORAL_P);
	GetChild(m_PRank5, UID_PRIVATE_1_P);
	GetChild(m_PRank6, UID_PRIVATE_2_P);
	GetChild(m_PRank7, UID_PRIVATE_3_P);

	m_IRank1.Setup();
	m_IRank2.Setup();
	m_IRank3.Setup();
	m_IRank4.Setup();
	m_IRank5.Setup();
	m_IRank6.Setup();
	m_IRank7.Setup();

	m_PRank1.Setup();
	m_PRank2.Setup();
	m_PRank3.Setup();
	m_PRank4.Setup();
	m_PRank5.Setup();
	m_PRank6.Setup();
	m_PRank7.Setup();

	m_IRank1.Set(GetInitials(0));
	m_IRank2.Set(GetInitials(1));
	m_IRank3.Set(GetInitials(2));
	m_IRank4.Set(GetInitials(3));
	m_IRank5.Set(GetInitials(4));
	m_IRank6.Set(GetInitials(5));
	m_IRank7.Set(GetInitials(6));

	m_PRank1.Set(GetPoints(0));
	m_PRank2.Set(GetPoints(1));
	m_PRank3.Set(GetPoints(2));
	m_PRank4.Set(GetPoints(3));
	m_PRank5.Set(GetPoints(4));
	m_PRank6.Set(GetPoints(5));
	m_PRank7.Set(GetPoints(6));
	return FALSE;
}

BOOL CHighScoresDlg::OnCmdPushButton(MRESULT &, USHORT Which, BOOL)
{
	switch(Which)
	{
		case BID_OKAY:
						DismissDlg(TRUE);
						return TRUE;
	}
	return FALSE;
}



PPROCESSHEADER(CGameOverDlg)
{
	STARTPROCESSING()
		PONINITDLG()

		STARTCMDPROCESSING()
			PONCMDPUSHBUTTON()
		ENDCMDPROCESSING()

	ENDPROCESSING()
}

CGameOverDlg::CGameOverDlg(BOOL WinP1)
{
	m_WinP1 = WinP1;
}

char g_Admiral[] = "ADMIRAL";
char g_SargentMajor[] = "SARGENT MAJOR";
char g_Sargent[] = "SARGENT";
char g_Corporal[] = "CORPORAL";
char g_PrivateFirst[] = "PRIVATE FIRST CLASS";
char g_PrivateSecond[] = "PRIVATE SECOND CLASS";
char g_PrivateThird[] = "PRIVATE THIRD CLASS";
char g_Regular[] = "PRIVATE FORTH CLASS";

BOOL CGameOverDlg::OnInitDlg(MRESULT &, CWindow, PVOID)
{
	GetChild(m_DPieceP1, UID_PIC_WIN_P1);
	GetChild(m_DPieceP2, UID_PIC_WIN_P2);

	GetChild(m_DClassP1, UID_RANK_P1);
	GetChild(m_DClassP2, UID_RANK_P2);

	GetChild(m_DDestroyedP1, UID_WEAPONS_P1);
	GetChild(m_DDestroyedP2, UID_WEAPONS_P2);

	GetChild(m_DPointsP1, UID_POINTS_P1);
	GetChild(m_DPointsP2, UID_POINTS_P2);

	m_DPieceP1.Setup();
	m_DPieceP2.Setup();

	m_DClassP1.Setup();
	m_DClassP2.Setup();

	m_DDestroyedP1.Setup();
	m_DDestroyedP2.Setup();

	m_DPointsP1.Setup();
	m_DPointsP2.Setup();

	if (m_WinP1)
	{
		m_DPieceP1.Set(BITID_WINP1);
		m_DPieceP2.Set(BITID_LOSEP1);
	}
	else
	{
		m_DPieceP1.Set(BITID_LOSEP1);
		m_DPieceP2.Set(BITID_WINP2);
	}

	switch(m_ClassP1)
	{
		case 0:
					m_DClassP1.Set(g_Admiral);
					break;
		case 1:
					m_DClassP1.Set(g_SargentMajor);
					break;
		case 2:
					m_DClassP1.Set(g_Sargent);
					break;
		case 3:
					m_DClassP1.Set(g_Corporal);
					break;
		case 4:
					m_DClassP1.Set(g_PrivateFirst);
					break;
		case 5:
					m_DClassP1.Set(g_PrivateSecond);
					break;
		case 6:
					m_DClassP1.Set(g_PrivateThird);
					break;
		default:
					m_DClassP1.Set(g_Regular);
					break;
	}

	switch(m_ClassP2)
	{
		case 0:
					m_DClassP2.Set(g_Admiral);
					break;
		case 1:
					m_DClassP2.Set(g_SargentMajor);
					break;
		case 2:
					m_DClassP2.Set(g_Sargent);
					break;
		case 3:
					m_DClassP2.Set(g_Corporal);
					break;
		case 4:
					m_DClassP2.Set(g_PrivateFirst);
					break;
		case 5:
					m_DClassP2.Set(g_PrivateSecond);
					break;
		case 6:
					m_DClassP2.Set(g_PrivateThird);
					break;
		default:
					m_DClassP2.Set(g_Regular);
					break;
	}

	m_DDestroyedP1.Set(m_DestroyedP1);
	m_DDestroyedP2.Set(m_DestroyedP2);

	m_DPointsP1.Set(m_PointsP1);
	m_DPointsP2.Set(m_PointsP2);

	return TRUE;
}

BOOL CGameOverDlg::OnCmdPushButton(MRESULT &, USHORT Which, BOOL)
{
	switch(Which)
	{
		case BID_OKAY:
		case BID_NEXT:
						DismissDlg(TRUE);
						return TRUE;
	}
	return FALSE;
}

char g_Default[] = "CPU";

void CGameOverDlg::SetP2(SHORT XSize, SHORT YSize, USHORT Villages, USHORT Skill, LONG Turns, LONG Destroyed, BOOL IsComputer, CWindow Parent)
{
	LONG SV, SA;

	SV = ((LONG)XSize * (LONG)YSize * (LONG)Villages * ((LONG)Skill + 1));

	Turns = max(1, Turns);

	SA = SV / Turns;

	if (!m_WinP1)
	{
		if (InRank(SA))
		{
			if (IsComputer)
				m_ClassP2 = AddRank(SA, g_Default);
			else
			{
				CAskInitialsDlg Test(2);

				Test.Dlg(CWindow(HWND_DESKTOP), Parent, NULLHANDLE, DID_CONGRAT);
				m_ClassP2 = AddRank(SA, Test.Get());
			}
		}
		else
			m_ClassP2 = AddRank(SA, g_Default);

		m_PointsP2 = SA;
	}
	else
	{
		m_ClassP2 = 10;
		m_PointsP2 = 0;
	}

	m_DestroyedP2 = Destroyed;
}

void CGameOverDlg::SetP1(SHORT XSize, SHORT YSize, USHORT Villages, USHORT Skill, LONG Turns, LONG Destroyed, BOOL IsComputer, CWindow Parent)
{
	LONG SV, SA;

	SV = ((LONG)XSize * (LONG)YSize * (LONG)Villages * ((LONG)Skill + 1));

	Turns = max(1, Turns);

	SA = SV / Turns;

	if (m_WinP1)
	{
		if (InRank(SA))
		{
			if (IsComputer)
				m_ClassP1 = AddRank(SA, g_Default);
			else
			{
				CAskInitialsDlg Test(1);

				Test.Dlg(CWindow(HWND_DESKTOP), Parent, NULLHANDLE, DID_CONGRAT);
				m_ClassP1 = AddRank(SA, Test.Get());
			}
		}
		else
			m_ClassP1 = AddRank(SA, g_Default);

		m_PointsP1 = SA;
	}
	else
	{
		m_ClassP1 = 10;
		m_PointsP1 = 0;
	}

	m_DestroyedP1 = Destroyed;
}

