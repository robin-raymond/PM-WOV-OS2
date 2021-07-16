#ifndef _RESOURCE_HPP_
#define _RESOURCE_HPP_

/*class CDisplayNumberWindow : public CWindow {
		SHORT m_Number;
		CHAR m_Text[20];
		void Update();
	public:
		DPROCESSHEADER()
		DONPAINT()

		CDisplayNumberWindow() {m_Text[0] = 0;}
		void Set(SHORT Number) {m_Number = Number; Update();}
};*/

class CResourceDlg : public CDialogBox {
		CBuild *m_Build;

		USHORT m_Player;

//		CDisplayNumberWindow m_DTotal;

		CSliderWindow m_GroundForces;
		CSliderWindow m_Tank;
		CSliderWindow m_FastTank;
		CSliderWindow m_BomberPlane;
		CSliderWindow m_FighterPlane;
		CSliderWindow m_Missile;
		CSliderWindow m_SandBuggy;
		CSliderWindow m_WaterForces;
		CSliderWindow m_RescueRepair; // HERE WEAPONS

		CButtonWindow m_Okay;
		CButtonWindow m_Cancel;
		CButtonWindow m_Help;

		void Get();
		void Set();
	public:
		DPROCESSHEADER()

		DONINITDLG()
		DONCHAR()
		DONCMDPUSHBUTTON()

		CResourceDlg(CBuild &Build, USHORT Player) {m_Build = &Build, m_Player = Player;}
};

#endif