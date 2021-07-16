#ifndef _GAMEOVER_HPP_
#define _GAMEOVER_HPP_

class CDisplayGamePieceWindow : public CWindow {
		ULONG m_Piece;
		void Update();
	public:
		DPROCESSHEADER()
		DONPAINT()

		CDisplayPieceWindow() {m_Piece = BITID_WINP1;}
		void Set(ULONG Piece) {m_Piece = Piece; Update();}
};

class CDisplayDestroyedWindow : public CWindow {
		LONG m_Number;
		CHAR m_Text[80];
		void Update();
	public:
		DPROCESSHEADER()
		DONPAINT()

		CDisplayDestroyedWindow() {m_Text[0] = 0;}
		void Set(LONG Number) {m_Number = Number; Update();}
};

class CDisplayPointsWindow : public CDisplayDestroyedWindow {
};

class CDisplayClassWindow : public CWindow {
		CHAR m_Text[80];
		PSZ m_Actual;
		void Update();
	public:
		DPROCESSHEADER()
		DONPAINT()

		CDisplayClassWindow() {m_Text[0] = 0; m_Actual = m_Text;}
		void Set(PSZ Class) {m_Actual = Class; Update();}
};

class CDisplayInitialsWindow : public CDisplayClassWindow {
};

class CGameOverDlg : public CDialogBox {
		BOOL m_WinP1;

		LONG m_DestroyedP1;
		LONG m_DestroyedP2;

		USHORT m_ClassP1;
		USHORT m_ClassP2;

		LONG m_PointsP1;
		LONG m_PointsP2;

		CDisplayGamePieceWindow m_DPieceP1;
		CDisplayGamePieceWindow m_DPieceP2;

		CDisplayClassWindow m_DClassP1;
		CDisplayClassWindow m_DClassP2;

		CDisplayDestroyedWindow m_DDestroyedP1;
		CDisplayDestroyedWindow m_DDestroyedP2;

		CDisplayPointsWindow m_DPointsP1;
		CDisplayPointsWindow m_DPointsP2;

	public:
		DPROCESSHEADER()
		DONINITDLG()
		DONCMDPUSHBUTTON()

		CGameOverDlg(BOOL);
		void SetP1(SHORT, SHORT, USHORT, USHORT, LONG, LONG, BOOL, CWindow);
		void SetP2(SHORT, SHORT, USHORT, USHORT, LONG, LONG, BOOL, CWindow);
};

class CHighScoresDlg : public CDialogBox {
		CDisplayInitialsWindow m_IRank1;
		CDisplayInitialsWindow m_IRank2;
		CDisplayInitialsWindow m_IRank3;
		CDisplayInitialsWindow m_IRank4;
		CDisplayInitialsWindow m_IRank5;
		CDisplayInitialsWindow m_IRank6;
		CDisplayInitialsWindow m_IRank7;

		CDisplayPointsWindow m_PRank1;
		CDisplayPointsWindow m_PRank2;
		CDisplayPointsWindow m_PRank3;
		CDisplayPointsWindow m_PRank4;
		CDisplayPointsWindow m_PRank5;
		CDisplayPointsWindow m_PRank6;
		CDisplayPointsWindow m_PRank7;
	public:
		DPROCESSHEADER()
		DONINITDLG()
		DONCMDPUSHBUTTON()

		CHighScoresDlg();
};

class CAskInitialsDlg : public CDialogBox {
		char m_Buffer[4];
		CEntryFieldWindow m_Initials;

		CWindow m_Player1;
		CWindow m_Player2;

		USHORT m_Player;

		void FillBuffer();
	public:
		DPROCESSHEADER()
		DONINITDLG()
//		DONCHAR()
		DONCMDPUSHBUTTON()

		CAskInitialsDlg(USHORT Player) {m_Player = Player; m_Buffer[0] = 0; m_Buffer[1] = 0; m_Buffer[2] = 0; m_Buffer[3] = 0;}

		PSZ Get() {return m_Buffer;}
};

#endif