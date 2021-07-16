#ifndef _PLAYERS_HPP_
#define _PlAYERS_HPP_

class CPlayersDlg : public CDialogBox {
		BOOL m_ComputerP1;
		BOOL m_ComputerP2;
		USHORT m_Skill;

		CRadioButtonWindow m_RHumanP1;
		CRadioButtonWindow m_RComputerP1;
		CRadioButtonWindow m_RHumanP2;
		CRadioButtonWindow m_RComputerP2;

		CSliderWindow m_SSkillP1;
		CSliderWindow m_SSkillP2;
		void UpdateP1();
		void UpdateP2();
	public:
		DPROCESSHEADER()

		DONINITDLG()
		DONCHAR()
		DONCMDPUSHBUTTON()
		DONBUTTONCLICKED()
		DONSLIDERCHANGE()

		CPlayersDlg();

		void Set(BOOL, BOOL, USHORT);
		void Get(BOOL &, BOOL &, USHORT &);
};

#endif