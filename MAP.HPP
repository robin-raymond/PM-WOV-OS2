#ifndef _MAP_HPP_
#define _MAP_HPP_

class CMapDlg : public CDialogBox {
		USHORT m_LightForest;
		USHORT m_DenseForest;
		USHORT m_Sand;
		USHORT m_Swamp;
		USHORT m_Villages;
		USHORT m_Bridges;
		USHORT m_XSize;
      USHORT m_YSize;

		BOOL m_River;

		CSliderWindow m_SLightForest;
		CSliderWindow m_SDenseForest;
		CSliderWindow m_SSand;
		CSliderWindow m_SSwamp;
		CSliderWindow m_SVillages;
		CSliderWindow m_SBridges;
		CSliderWindow m_SXSize;
		CSliderWindow m_SYSize;
		CButtonWindow m_CRiver;
		CButtonWindow m_BOkay;
		CButtonWindow m_BCancel;
		CButtonWindow m_BHelp;

		CWindow m_TBridges;
		void UpdateAll();
	public:
		DPROCESSHEADER()

		DONINITDLG()
		DONCHAR()
		DONCMDPUSHBUTTON()
		DONBUTTONCLICKED()
		DONSLIDERCHANGE()

		CMapDlg();

		void Set(USHORT, USHORT, USHORT, USHORT, USHORT, USHORT,
					USHORT, USHORT, BOOL);
		void Get(USHORT &, USHORT &, USHORT &, USHORT &, USHORT &, USHORT &,
					USHORT &, USHORT &, BOOL &);

};

#endif