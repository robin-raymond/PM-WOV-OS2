#ifndef _CONTROL_HPP_
#define _CONTROL_HPP_

class CDisplayPieceWindow : public CWindow {
		ULONG m_Piece;
		void Update();
	public:
		DPROCESSHEADER()
		DONPAINT()

		CDisplayPieceWindow();
		void Set(ULONG Piece) {m_Piece = Piece; Update();}
};

class CDisplayPositionWindow : public CWindow {
		SHORT m_XPos;
		SHORT m_YPos;
		CHAR m_Text[20];
		void Update();
	public:
		DPROCESSHEADER()
		DONPAINT()

		CDisplayPositionWindow();
		void Set(SHORT XPos, SHORT YPos) {m_XPos = XPos; m_YPos = YPos; Update();}
};

class CControlDlg : public CDialogBox {
		BOOL m_AttackingFighter;

		CWindow m_CPTPosition;
		CWindow m_DTPosition;

		CDisplayPositionWindow m_CPPosition;
		CDisplayPositionWindow m_DPosition;

		CWindow m_CPTAttack;
		CWindow m_CPTDefense;
		CWindow m_DTAttack;
		CWindow m_DTDefense;

		CSliderWindow m_CPAttack;
		CSliderWindow m_CPDefense;
		CSliderWindow m_DAttack;
		CSliderWindow m_DDefense;

		CDisplayPieceWindow m_CPPiece;
		CDisplayPieceWindow m_DPiece;

		CButtonWindow m_AttackFighter;
		CButtonWindow m_Move;
		CButtonWindow m_Find;
	public:
		DPROCESSHEADER()
		DONINITDLG()
		DONCHAR()
		DONCMDPUSHBUTTON()
		DONBUTTONCLICKED()

		CControlDlg();

		void HideAll();

		void Set(SHORT, SHORT,
					BOOL, USHORT, USHORT, ULONG,
					BOOL, SHORT, SHORT,
					BOOL, USHORT, USHORT, ULONG,
					BOOL);

};

#endif