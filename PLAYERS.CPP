#define _COMMON_CWINDOW_H_
#include "common.h"

#include "wargame.h"
#include "players.hpp"


PPROCESSHEADER(CPlayersDlg)
{
	STARTPROCESSING()
		PONINITDLG()
		PONCHAR()

		STARTCMDPROCESSING()
			PONCMDPUSHBUTTON()
		ENDCMDPROCESSING()

		STARTCONTROLPROCESSING()

			INCLUDEIDPROCESSING(RID_HUMAN_P1)
			INCLUDEIDPROCESSING(RID_COMPUTER_P1)
			INCLUDEIDPROCESSING(RID_HUMAN_P2)
			INCLUDEIDPROCESSING(RID_COMPUTER_P2)
			STARTCONTROLIDPROCESSING()
				PONBUTTONCLICKED()
			ENDCONTROLIDPROCESSING()

			INCLUDEIDPROCESSING(SLID_SKILL_P1)
			INCLUDEIDPROCESSING(SLID_SKILL_P2)
			STARTCONTROLIDPROCESSING()
				PONSLIDERCHANGE()
			ENDCONTROLIDPROCESSING()

		ENDCONTROLPROCESSING()
	ENDPROCESSING()
}

BOOL CPlayersDlg::OnChar(MRESULT &, USHORT flags, UCHAR, UCHAR, USHORT, USHORT Virtual)
{
	if (flags & KC_KEYUP)
		return FALSE;

	if (Virtual == VK_ESC)
	{
		DismissDlg(FALSE);
		return TRUE;
	}

	return FALSE;
}

BOOL CPlayersDlg::OnInitDlg(MRESULT &, CWindow, PVOID)
{
	GetChild(m_RHumanP1, RID_HUMAN_P1);
	GetChild(m_RComputerP1, RID_COMPUTER_P1);
	GetChild(m_RHumanP2, RID_HUMAN_P2);
	GetChild(m_RComputerP2, RID_COMPUTER_P2);
	GetChild(m_SSkillP1, SLID_SKILL_P1);
	GetChild(m_SSkillP2, SLID_SKILL_P2);
	UpdateP1();
	UpdateP2();
	return FALSE;
}

BOOL CPlayersDlg::OnSliderChange(MRESULT &, USHORT, ULONG)
{
	if (m_ComputerP1)
	{
		m_Skill = m_SSkillP1.GetArmPosition();
	}
	else
	{
		if (m_ComputerP2)
		{
			m_Skill = m_SSkillP2.GetArmPosition();
		}
	}
	return FALSE;
}

BOOL CPlayersDlg::OnButtonClicked(MRESULT &, USHORT Which)
{
	switch(Which)
	{
		case RID_HUMAN_P1:
							m_ComputerP1 = FALSE;
							UpdateP1();
							return TRUE;
		case RID_COMPUTER_P1:
							m_ComputerP1 = TRUE;
							if (m_ComputerP2 == TRUE)
							{
								m_ComputerP2 = FALSE;
								UpdateP2();
							}
							UpdateP1();
							return TRUE;
		case RID_HUMAN_P2:
							m_ComputerP2 = FALSE;
							UpdateP2();
							return TRUE;
		case RID_COMPUTER_P2:
							m_ComputerP2 = TRUE;
							if (m_ComputerP1 == TRUE)
							{
								m_ComputerP1 = FALSE;
								UpdateP1();
							}
							UpdateP2();
							return TRUE;
	}
	return FALSE;
}

BOOL CPlayersDlg::OnCmdPushButton(MRESULT &, USHORT Which, BOOL)
{
	switch(Which)
	{
		case BID_OKAY:
							DismissDlg(TRUE);
							return TRUE;
		case BID_CANCEL:
							DismissDlg(FALSE);
							return TRUE;
	}
	return FALSE;
}

CPlayersDlg::CPlayersDlg()
{
	m_ComputerP1 = FALSE;
	m_ComputerP2 = TRUE;
	m_Skill = 2;
}

void CPlayersDlg::UpdateP1()
{
	if (m_ComputerP1)
	{
		m_RHumanP1.SetCheck(0);
		m_RComputerP1.SetCheck(1);
		m_SSkillP1.SetArmPosition(m_Skill);
		m_SSkillP1.Show(NULL);
	}
	else
	{
		m_RHumanP1.SetCheck(1);
		m_RComputerP1.SetCheck(0);
		m_SSkillP1.Hide(NULL);
	}
}

void CPlayersDlg::UpdateP2()
{
	if (m_ComputerP2)
	{
		m_RHumanP2.SetCheck(0);
		m_RComputerP2.SetCheck(1);
		m_SSkillP2.SetArmPosition(m_Skill);
		m_SSkillP2.Show(NULL);
	}
	else
	{
		m_RHumanP2.SetCheck(1);
		m_RComputerP2.SetCheck(0);
		m_SSkillP2.Hide(NULL);
	}
}

void CPlayersDlg::Set(BOOL ComputerP1, BOOL ComputerP2, USHORT Skill)
{
	m_ComputerP1 = ComputerP1;
	m_ComputerP2 = ComputerP2;
	m_Skill = Skill;
}

void CPlayersDlg::Get(BOOL &ComputerP1, BOOL &ComputerP2, USHORT &Skill)
{
	ComputerP1 = m_ComputerP1;
	ComputerP2 = m_ComputerP2;
	Skill = m_Skill;
}
