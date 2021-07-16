#define _COMMON_CWINDOW_H_
#define _COMMON_COBLIST_H_
#include "common.h"

#include "board.hpp"
#include "players.hpp"
#include "map.hpp"
#include "abandon.hpp"
#include "control.hpp"
#include "resource.hpp"
#include "gameover.hpp"
#include "about.hpp"
#include "regme.hpp"
#include "register.hpp"
#include "wargame.h"

#include <i86.h>
#include <stdlib.h>
#include <time.h>

//REMOVE ME - DEBUG IO STREAM
//#include <iostream.h>
//END REMOVE ME - DEBUG IO STREAM

#define G_STAGE_1 1
#define G_STAGE_2 2
#define G_STAGE_3 3

#define HID_KEYS_HELP 8888

class CMainWindow : public CWindow {
		CBoard *m_Board;
		CBuild *m_PlayerBuilds[2];
		BOOL m_ComputerP1;
		BOOL m_ComputerP2;
		USHORT m_Skill;
		USHORT m_Stage;

		CMenuWindow m_MenuTargetTwo;
		CMenuWindow m_MenuRescueTwo;
		CMenuWindow m_MenuTargetOne;
		CMenuWindow m_MainMenu;

		CControlDlg m_Control;

		USHORT m_LightForest;
		USHORT m_DenseForest;
		USHORT m_Sand;
		USHORT m_Swamp;
		USHORT m_Villages;
		USHORT m_Bridges;
		USHORT m_XSize;
		USHORT m_YSize;

		BOOL m_River;

		USHORT m_ALightForest;
		USHORT m_ADenseForest;
		USHORT m_ASand;
		USHORT m_ASwamp;
		USHORT m_AVillages;
		USHORT m_ABridges;
		USHORT m_AXSize;
		USHORT m_AYSize;

		BOOL m_ARiver;

		USHORT m_Player;

		SHORT m_XSelect;
		SHORT m_YSelect;

		SHORT m_LastXShown;
		SHORT m_LastYShown;

		SHORT m_XTarget;
		SHORT m_YTarget;

		SHORT m_XShift;
		SHORT m_YShift;

		SHORT m_XViewed;
		SHORT m_YViewed;

		BOOL m_Dest;
		SHORT m_XDest;
		SHORT m_YDest;
		CFighter *m_LastFound;
		LONG m_Y2;

		BOOL m_GameOver;
		BOOL m_InfoBox;

		BOOL m_HasRegistered;
		BOOL m_ShownRegister;
	public:
		DPROCESSHEADER()

		DONCMDMENU()

		DONHSCROLLLINELEFT()
		DONHSCROLLLINERIGHT()
		DONHSCROLLPAGELEFT()
		DONHSCROLLPAGERIGHT()
		DONHSCROLLSLIDERPOSITION()

		DONVSCROLLLINEUP()
		DONVSCROLLLINEDOWN()
		DONVSCROLLPAGEUP()
		DONVSCROLLPAGEDOWN()
		DONVSCROLLSLIDERPOSITION()

		DONCHAR()
		DONBUTTON1DOWN()
		DONBUTTON2DOWN()
		DONBUTTON3DOWN()
		DONPAINT()
		DONSIZE()

		DONMOUSEMOVE()
		DONCMDOTHER()

		DONCREATE()
		DONDESTROY()

		DONQUERYKEYSHELP()

		void SetPlayers();
		void SetMap();
		void SetupResources();
		void MenuTargetTwo(LONG, LONG);
		void MenuRescueTwo(LONG, LONG);
		void MenuTargetOne(LONG, LONG);

		CMainWindow();
		void ScrollBarNewPos(USHORT, SHORT, SHORT, SHORT, BOOL);
		void RepaintAll();
		void GetTotalViewed();
		void UpdateControl(SHORT, SHORT);
		void DisplaySelect(HPS);
		void GameOver();

		void DisplayDest(HPS);
		void SetDest(BOOL, SHORT, SHORT, BOOL);
		void DisplayPiece(SHORT, SHORT, BOOL);
		void DisplayPiece(HPS, SHORT, SHORT, BOOL);
		void DisplayActionPiece(HPS, SHORT, SHORT, ULONG);

		BOOL RightClick(SHORT, SHORT);
		void Play(USHORT, BOOL);
		void Target(BOOL);
		void FindNext();
		void Turn();
		void Select(SHORT, SHORT);
		void Moving(SHORT, SHORT);
		void Adding(SHORT, SHORT);
		void MakeFocus(SHORT, SHORT);
		void Destroying(SHORT, SHORT);
		void HighScores();
		void UpdatePlayer();
		void About();
		void RegisterMe();
};

class CBindMainWindow : public CBindMove {
		CMainWindow *m_Window;
	public:
		CBindMainWindow() {m_Window = NULL;}
		void Set(CMainWindow &SomeWindow) {m_Window = &SomeWindow;}
		void Add(SHORT, SHORT);
		void Move(SHORT, SHORT);
		void Destroy(SHORT, SHORT);
};

void CBindMainWindow::Add(SHORT x, SHORT y)
{
	if (m_Window != NULL)
	{
		m_Window->Adding(x, y);
	}
}

void CBindMainWindow::Move(SHORT x, SHORT y)
{
	if (m_Window != NULL)
	{
		m_Window->Moving(x, y);
	}
}

void CBindMainWindow::Destroy(SHORT x, SHORT y)
{
	if (m_Window != NULL)
	{
		m_Window->Destroying(x, y);
	}
}


PPROCESSHEADER(CMainWindow)
{
	STARTPROCESSING()

		PONPAINT()

		STARTCMDPROCESSING()
			PONCMDMENU()
			PONCMDOTHER()
		ENDCMDPROCESSING()

		STARTHSCROLLPROCESSING()
			PONHSCROLLLINELEFT()
			PONHSCROLLLINERIGHT()
			PONHSCROLLPAGELEFT()
			PONHSCROLLPAGERIGHT()
			PONHSCROLLSLIDERPOSITION()
		ENDHSCROLLPROCESSING()

		STARTVSCROLLPROCESSING()
			PONVSCROLLLINEUP()
			PONVSCROLLLINEDOWN()
			PONVSCROLLPAGEUP()
			PONVSCROLLPAGEDOWN()
			PONVSCROLLSLIDERPOSITION()
		ENDVSCROLLPROCESSING()

		PONSIZE()
		PONCHAR()
		PONBUTTON1DOWN()
		PONBUTTON2DOWN()
		PONBUTTON3DOWN()

		PONMOUSEMOVE()

		PONCREATE()
		PONDESTROY()

		PONQUERYKEYSHELP()

	ENDPROCESSING()
}

CMainWindow::CMainWindow()
{
	m_Board = NULL;
	m_PlayerBuilds[0] = NULL;
	m_PlayerBuilds[1] = NULL;

	m_ALightForest = 75;
	m_ADenseForest = 25;
	m_ASand = 15;
	m_ASwamp = 30;
	m_AVillages = 1;
	m_ABridges = 70;
	m_AXSize = 10;
	m_AYSize = 10;

	m_ARiver = TRUE;

	m_ComputerP1 = FALSE;
	m_ComputerP2 = TRUE;
	m_Skill = 0;

	m_Stage = G_STAGE_1;

	m_XSelect = 0;
	m_YSelect = 0;

	m_LastXShown = 0;
	m_LastYShown = 0;

	m_Player = 1;

	m_XShift = 0;
	m_YShift = 0;

	m_XTarget = 0;
	m_YTarget = 0;

	m_GameOver = TRUE;
	m_InfoBox = FALSE;

	m_Dest = FALSE;

	m_HasRegistered = VerifyRegister();
	m_ShownRegister = FALSE;
}

BOOL CMainWindow::OnQueryKeysHelp(MRESULT &Return)
{
	Return = (MRESULT)HID_KEYS_HELP;
	return TRUE;
}


BOOL CMainWindow::OnCreate(MRESULT &, PVOID, PCREATESTRUCT)
{
	m_MenuTargetOne.LoadMenu(*this, NULL, ID_TARGET_ONE);
	m_MenuTargetTwo.LoadMenu(*this, NULL, ID_TARGET_TWO);
	m_MenuRescueTwo.LoadMenu(*this, NULL, ID_RESCUE_TWO);

	HPS hps;
	hps = GetPS();
	LoadBitmaps(hps);
	ReleasePS(hps);

	m_Control.LoadDlg(CWindow(HWND_DESKTOP), *this, NULLHANDLE, DID_CONTROL_CENTER);

	return FALSE;
}

BOOL CMainWindow::OnDestroy(MRESULT &)
{
	if (m_Stage == G_STAGE_2)
	{
		ObjectDelete(m_Board);
		ObjectDelete(m_PlayerBuilds[0]);
		ObjectDelete(m_PlayerBuilds[1]);
	}
	m_Control.DestroyWindow();
	DeleteBitmaps();
	return FALSE;
}

void CMainWindow::UpdatePlayer()
{
	if (m_Stage == G_STAGE_2)
	{
		if (m_Player == 1)
			m_Control.SetWindowText("Control Center - Player One");
		else
			m_Control.SetWindowText("Control Center - Player Two");
	}
}

void CMainWindow::DisplayPiece(SHORT XPos, SHORT YPos, BOOL OkayDest)
{
	HPS hps;

	hps = GetPS();
	DisplayPiece(hps, XPos, YPos, OkayDest);
	ReleasePS(hps);
}

void CMainWindow::DisplayPiece(HPS hps, SHORT XPos, SHORT YPos, BOOL OkayDest)
{
	if ((XPos >= m_XShift) && (YPos >= m_YShift))
	{
		if ((XPos < m_XShift + m_XViewed) && (YPos < m_YShift + m_YViewed))
		{
			m_Board->DisplayPiece(XPos, YPos, hps, ((LONG)XPos - (LONG)m_XShift) * 32,
										((LONG)m_Y2 - 31) - (((LONG)YPos - (LONG)m_YShift) * 32) );

			if ((XPos == m_XSelect)&&(YPos == m_YSelect))
				DisplaySelect(hps);

			if ((m_Dest)&&(OkayDest))
			{
				if ((m_XDest == XPos)&&(YPos == m_YDest))
					DisplayDest(hps);
			}
		}
	}
}

void CMainWindow::DisplayActionPiece(HPS hps, SHORT XPos, SHORT YPos, ULONG Piece)
{
	if ((XPos >= m_XShift) && (YPos >= m_YShift))
	{
		if ((XPos < m_XShift + m_XViewed) && (YPos < m_YShift + m_YViewed))
		{
			m_Board->DisplayActionPiece(hps, ((LONG)XPos - (LONG)m_XShift) * 32,
										((LONG)m_Y2 - 31) - (((LONG)YPos - (LONG)m_YShift) * 32),
										Piece);
		}
	}
}

void CMainWindow::MakeFocus(SHORT XPos, SHORT YPos)
{
	if (m_Stage != G_STAGE_2)
		return;

	USHORT XViewed = m_XViewed;
	USHORT YViewed = m_YViewed;

	if (XViewed > 1)
		--XViewed;
	if (YViewed > 1)
		--YViewed;

	BOOL Repaint;

	Repaint = FALSE;

	if ((XPos < m_XShift)||(XPos >= m_XShift + XViewed))
	{
		Repaint = TRUE;
		m_XShift = (SHORT)((LONG)XPos - ((LONG)m_XViewed / 2));
		m_XShift = (SHORT)max(m_XShift, 0);
		if (m_XShift + m_XViewed < m_XSize)
			ScrollBarNewPos(FID_HORZSCROLL, m_XShift, m_XViewed, m_XSize, FALSE);
		else
			ScrollBarNewPos(FID_HORZSCROLL, m_XShift, (SHORT)((LONG)m_XSize - (LONG)m_XShift), m_XSize, FALSE);
	}

	if ((YPos < m_YShift)||(YPos >= m_YShift + YViewed))
	{
		Repaint = TRUE;
		m_YShift = (SHORT)((LONG)YPos - ((LONG)m_YViewed / 2));
		m_YShift = (SHORT)max(m_YShift, 0);
		if (m_YShift + m_YViewed < m_YSize)
			ScrollBarNewPos(FID_VERTSCROLL, m_YShift, m_YViewed, m_YSize, FALSE);
		else
			ScrollBarNewPos(FID_VERTSCROLL, m_YShift, (SHORT)((LONG)m_YSize - (LONG)m_YShift), m_YSize, FALSE);
	}

	if (Repaint)
	{
		HPS hps;

		hps = GetPS();

		SHORT x, y;
		for (y = m_YShift; y < m_YShift + m_YViewed; ++y)
		{
			for (x = m_XShift; x < m_XShift + m_XViewed; ++x)
				DisplayPiece(hps, x, y, TRUE);
		}
		ReleasePS(hps);
	}
}

void CMainWindow::UpdateControl(SHORT XPos, SHORT YPos)
{
	CPart *Part;

	Part = &(m_Board->GetPart(XPos, YPos));

	if (Part->HasFighter())
	{
		CFighter *Fighter;

		Fighter = Part->GetFighter();

		SHORT DestX, DestY;
		USHORT Attack, Defense;
		ULONG Piece;

		DestX = Fighter->GetDestX();
		DestY = Fighter->GetDestY();
		Attack = Fighter->GetAttack();
		Defense = Fighter->GetDefense();
		Piece = Fighter->GetPiece();

		if (((DestX == XPos) && (DestY == YPos)) || (m_Player != Fighter->GetControl()))
		{
			SetDest(FALSE, 0, 0, TRUE);
			m_Control.Set(XPos, YPos,
							  TRUE, Attack, Defense, Piece,
							  FALSE, 0, 0,
							  FALSE, 0, 0, 0,
							  FALSE);
		}
		else
		{
			SetDest(TRUE, DestX, DestY, TRUE);
			CPart *DPart = &(m_Board->GetPart(DestX, DestY));
			if (DPart->HasFighter())
			{
				CFighter *DFighter;

				DFighter = DPart->GetFighter();
				USHORT DAttack, DDefense;
				ULONG DPiece;

				DAttack = DFighter->GetAttack();
				DDefense = DFighter->GetDefense();
				DPiece = DFighter->GetPiece();
				m_Control.Set(XPos, YPos,
								  TRUE, Attack, Defense, Piece,
								  TRUE, DestX, DestY,
								  TRUE, DAttack, DDefense, DPiece,
								  Fighter->HasFighter());
			}
			else
			{
				ULONG DPiece;

				DPiece = DPart->GetPiece();
				m_Control.Set(XPos, YPos,
								  TRUE, Attack, Defense, Piece,
								  TRUE, DestX, DestY,
								  FALSE, 0, 0, DPiece,
								  FALSE);
			}
		}
	}
	else
	{
		SetDest(FALSE, 0, 0, FALSE);
		m_Control.Set(XPos, YPos,
						  FALSE, 0, 0, Part->GetPiece(),
						  FALSE, 0, 0,
						  FALSE, 0, 0, 0,
						  FALSE);
	}
	m_LastXShown = XPos;
	m_LastYShown = YPos;
}

void CMainWindow::SetDest(BOOL On, SHORT XPos, SHORT YPos, BOOL Fighter)
{
	if ((m_Dest == FALSE)&&(On == FALSE))
		return;
	if (m_Dest == FALSE)
	{
		if (On == FALSE)
			return;

		m_XDest = XPos;
		m_YDest = YPos;
		m_Dest = TRUE;

		HPS hps = GetPS();
		DisplayDest(hps);
		ReleasePS(hps);
	}
	else
	{
		if ((On == FALSE) && (Fighter == FALSE))
			return;

		DisplayPiece(m_XDest, m_YDest, FALSE);

		if (On == FALSE)
		{
			m_Dest = FALSE;
			return;
		}

		m_XDest = XPos;
		m_YDest = YPos;
		m_Dest = TRUE;

		HPS hps = GetPS();
		DisplayDest(hps);
		ReleasePS(hps);
	}
}

BOOL CMainWindow::OnMouseMove(MRESULT &, SHORT XMouse, SHORT YMouse, USHORT)
{
	if (m_Stage == G_STAGE_2)
	{
		if ((XMouse < 0) && (YMouse < 0))
			return FALSE;

		LONG x1, y1, x2, y2;

		GetWindowRect(x1, y1, x2, y2);

		SHORT XPos, YPos;

		XPos = (SHORT)(((LONG)XMouse / 32) + (LONG)m_XShift);
		YPos = (SHORT)(((y2 - (LONG)YMouse) / 32) + (LONG)m_YShift);

		if ((XPos == m_LastXShown)&&(YPos == m_LastYShown))
			return FALSE;
		if (m_Board->OutOfBounds(XPos, YPos))
			return FALSE;

		UpdateControl(XPos, YPos);

		return TRUE;
	}
	return FALSE;
}

void CMainWindow::DisplaySelect(HPS hps)
{
	GpiSetColor(hps, CLR_RED);

	POINTL point1, point2, point3, point4;
	point1.x = (((LONG)m_XSelect - (LONG)m_XShift) * 32);
	point1.y = m_Y2 - (((LONG)m_YSelect - (LONG)m_YShift) * 32) - 1;
	point2.x = point1.x + 31;
	point2.y = point1.y;
	point3.x = point1.x + 31;
	point3.y = point1.y - 30;
	point4.x = point1.x;
	point4.y = point1.y - 30;
	GpiMove(hps, &point1);
	GpiLine(hps, &point2);
	GpiLine(hps, &point3);
	GpiLine(hps, &point4);
	GpiLine(hps, &point1);

	GpiMove(hps, &point1);
	GpiLine(hps, &point3);
	GpiLine(hps, &point2);
	GpiLine(hps, &point4);
	GpiLine(hps, &point1);
}

void CMainWindow::DisplayDest(HPS hps)
{
	if (m_Dest)
	{
		GpiSetColor(hps, CLR_YELLOW);

		POINTL point1, point2, point3, point4;
		point1.x = (((LONG)m_XDest - (LONG)m_XShift) * 32);
		point1.y = m_Y2 - (((LONG)m_YDest - (LONG)m_YShift) * 32) - 1;
		point2.x = point1.x + 31;
		point2.y = point1.y;
		point3.x = point1.x + 31;
		point3.y = point1.y - 30;
		point4.x = point1.x;
		point4.y = point1.y - 30;
		GpiMove(hps, &point1);
		GpiLine(hps, &point2);
		GpiLine(hps, &point3);
		GpiLine(hps, &point4);
		GpiLine(hps, &point1);
	}
}

void CMainWindow::Adding(SHORT XPos, SHORT YPos)
{
	if (m_Stage == G_STAGE_2)
	{
//		MakeFocus(XPos, YPos);

		DisplayPiece(XPos, YPos, TRUE);
	}
}


void CMainWindow::Moving(SHORT XPos, SHORT YPos)
{
	if (m_Stage == G_STAGE_2)
	{
//		MakeFocus(XPos, YPos);

		DisplayPiece(XPos, YPos, TRUE);
		if (!((XPos < m_XShift)||(XPos >= m_XShift + m_XViewed)))
		{
			if (!((YPos < m_YShift)||(YPos >= m_YShift + m_YViewed)))
			{
				if (m_Board->GetPart(XPos, YPos).HasFighter())
					delay(60);
			}
		}
	}
}

void CMainWindow::Destroying(SHORT XPos, SHORT YPos)
{
	if (m_Stage == G_STAGE_2)
	{
		MakeFocus(XPos, YPos);
		HPS hps;

		hps = GetPS();

		DisplayActionPiece(hps, XPos, YPos, BITID_BLOWUP1);
		delay(100);
		DisplayActionPiece(hps, XPos, YPos, BITID_BLOWUP2);
		delay(100);
		DisplayActionPiece(hps, XPos, YPos, BITID_BLOWUP3);
		delay(100);
		DisplayActionPiece(hps, XPos, YPos, BITID_BLOWUP4);
		delay(100);
		DisplayActionPiece(hps, XPos, YPos, BITID_BLOWUP5);
		delay(100);
		DisplayPiece(hps, XPos, YPos, TRUE);

		ReleasePS(hps);
	}
}

BOOL CMainWindow::OnPaint(MRESULT &)
{
	if (m_Stage == G_STAGE_2)
	{
		HPS hps;

		hps = BeginPaint(NULLHANDLE, NULL);

		SHORT x, y;
		for (y = m_YShift; y < m_YShift + m_YViewed; ++y)
		{
			for (x = m_XShift; x < m_XShift + m_XViewed; ++x)
				DisplayPiece(hps, x, y, TRUE);
		}

		EndPaint(hps);

		return TRUE;
	}
	else
	{
		HPS hps;
		hps = BeginPaint(NULLHANDLE, NULL);
		GpiErase(hps);
		EndPaint(hps);
	}
	return FALSE;
}

void CMainWindow::RepaintAll()
{
	LONG x1, y1, x2, y2;

	GetWindowRect(x1, y1, x2, y2);
	InvalidateRect(x1, y1, x2, y2, FALSE);
}

void CMainWindow::ScrollBarNewPos(USHORT Id, SHORT NewPos, SHORT Viewable, SHORT Total, BOOL Repaint)
{
	CScrollBarWindow Tmp;
	CWindow Frame;

	GetParent(Frame);
	Frame.GetChild(Tmp, Id);
	Tmp.SetPosition(NewPos);

	Tmp.SetThumbSize(Viewable, Total);

	if (Repaint)
		RepaintAll();
}

void CMainWindow::GetTotalViewed()
{
	LONG x1, y1, x2, y2;

	GetWindowRect(x1, y1, x2, y2);

	m_XViewed = (SHORT)(x2 / 32);
	m_YViewed = (SHORT)(y2 / 32);

	if ((x2 % 32) != 0)
	{
		++m_XViewed;
	}
	if ((y2 % 32) != 0)
	{
		++m_YViewed;
	}
}

BOOL CMainWindow::OnHScrollLineLeft(MRESULT &, USHORT Id)
{
	if (m_Stage != G_STAGE_2)
		return FALSE;

	if (m_XShift > 0)
	{
		--m_XShift;
		if (m_XShift + m_XViewed < m_XSize)
			ScrollBarNewPos(Id, m_XShift, m_XViewed, m_XSize, TRUE);
		else
			ScrollBarNewPos(Id, m_XShift, (SHORT)((LONG)m_XSize - (LONG)m_XShift), m_XSize, TRUE);
	}

	return TRUE;
}

BOOL CMainWindow::OnHScrollPageLeft(MRESULT &, USHORT Id)
{
	if (m_Stage != G_STAGE_2)
		return FALSE;

	if ((m_XShift - m_XViewed + 1) >= 0)
		m_XShift = (SHORT)((LONG)m_XShift - (LONG)m_XViewed + 1);
	else
		m_XShift = 0;

	if (m_XShift + m_XViewed < m_XSize)
		ScrollBarNewPos(Id, m_XShift, m_XViewed, m_XSize, TRUE);
	else
		ScrollBarNewPos(Id, m_XShift, (SHORT)((LONG)m_XSize - (LONG)m_XShift), m_XSize, TRUE);
	return TRUE;
}

BOOL CMainWindow::OnHScrollLineRight(MRESULT &, USHORT Id)
{
	if (m_Stage != G_STAGE_2)
		return FALSE;

	if (m_XShift + 1 < m_XSize)
	{
		++m_XShift;
		if ((SHORT)((LONG)m_XShift + (LONG)m_XViewed) < m_XSize)
			ScrollBarNewPos(Id, m_XShift, m_XViewed, m_XSize, TRUE);
		else
			ScrollBarNewPos(Id, m_XShift, (SHORT)((LONG)m_XSize - (LONG)m_XShift), m_XSize, TRUE);
	}

	return TRUE;
}

BOOL CMainWindow::OnHScrollPageRight(MRESULT &, USHORT Id)
{
	if (m_Stage != G_STAGE_2)
		return FALSE;

	if ((m_XShift + m_XViewed - 1) < (m_XSize - 1))
		m_XShift = (SHORT)((LONG)m_XShift + (LONG)m_XViewed - 1);
	else
		m_XShift = (SHORT)((LONG)m_XSize - 1);

	if (m_XShift + m_XViewed < m_XSize)
		ScrollBarNewPos(Id, m_XShift, m_XViewed, m_XSize, TRUE);
	else
		ScrollBarNewPos(Id, m_XShift, (SHORT)((LONG)m_XSize - (LONG)m_XShift), m_XSize, TRUE);

	return TRUE;
}

BOOL CMainWindow::OnHScrollSliderPosition(MRESULT &, USHORT Id, SHORT NewPos)
{
	if (m_Stage != G_STAGE_2)
		return FALSE;

	m_XShift = NewPos;

	if (m_XShift + m_XViewed < m_XSize)
		ScrollBarNewPos(Id, m_XShift, m_XViewed, m_XSize, TRUE);
	else
		ScrollBarNewPos(Id, m_XShift, (SHORT)((LONG)m_XSize - (LONG)m_XShift), m_XSize, TRUE);

	return TRUE;
}

BOOL CMainWindow::OnVScrollLineUp(MRESULT &, USHORT Id)
{
	if (m_Stage != G_STAGE_2)
		return FALSE;

	if (m_YShift > 0)
	{
		--m_YShift;

		if (m_YShift + m_YViewed < m_YSize)
			ScrollBarNewPos(Id, m_YShift, m_YViewed, m_YSize, TRUE);
		else
			ScrollBarNewPos(Id, m_YShift, (SHORT)((LONG)m_YSize - (LONG)m_YShift), m_YSize, TRUE);
	}

	return TRUE;
}

BOOL CMainWindow::OnVScrollPageUp(MRESULT &, USHORT Id)
{
	if (m_Stage != G_STAGE_2)
		return FALSE;

	if (((LONG)m_YShift - (LONG)m_YViewed + 1) >= 0)
		m_YShift = (SHORT)((LONG)m_YShift - (LONG)m_YViewed + 1);
	else
		m_YShift = 0;

	if (m_YShift + m_YViewed < m_YSize)
		ScrollBarNewPos(Id, m_YShift, m_YViewed, m_YSize, TRUE);
	else
		ScrollBarNewPos(Id, m_YShift, (SHORT)((LONG)m_YSize - (LONG)m_YShift), m_YSize, TRUE);
	return TRUE;
}

BOOL CMainWindow::OnVScrollLineDown(MRESULT &, USHORT Id)
{
	if (m_Stage != G_STAGE_2)
		return FALSE;

	if (m_YShift + 1 < m_YSize)
	{
		++m_YShift;

		if (m_YShift + m_YViewed < m_YSize)
			ScrollBarNewPos(Id, m_YShift, m_YViewed, m_YSize, TRUE);
		else
			ScrollBarNewPos(Id, m_YShift, (SHORT)((LONG)m_YSize - (LONG)m_YShift), m_YSize, TRUE);
	}

	return TRUE;
}

BOOL CMainWindow::OnVScrollPageDown(MRESULT &, USHORT Id)
{
	if (m_Stage != G_STAGE_2)
		return FALSE;

	if ((m_YShift + m_YViewed - 1) < (m_YSize - 1))
		m_YShift = (SHORT)((LONG)m_YShift + (LONG)m_YViewed - 1);
	else
		m_YShift = (SHORT)((LONG)m_YSize - 1);

	if (m_YShift + m_YViewed < m_YSize)
		ScrollBarNewPos(Id, m_YShift, m_YViewed, m_YSize, TRUE);
	else
		ScrollBarNewPos(Id, m_YShift, (SHORT)((LONG)m_YSize - (LONG)m_YShift), m_YSize, TRUE);

	return TRUE;
}

BOOL CMainWindow::OnVScrollSliderPosition(MRESULT &, USHORT Id, SHORT NewPos)
{
	if (m_Stage != G_STAGE_2)
		return FALSE;

	m_YShift = NewPos;

	if (m_YShift + m_YViewed < m_YSize)
		ScrollBarNewPos(Id, m_YShift, m_YViewed, m_YSize, TRUE);
	else
		ScrollBarNewPos(Id, m_YShift, (SHORT)((LONG)m_YSize - (LONG)m_YShift), m_YSize, TRUE);

	return TRUE;
}


//BOOL CMainWindow::OnChar(MRESULT &mr, USHORT flags, UCHAR RepeatCount, UCHAR RawScan, USHORT Letter, USHORT Virtual)
BOOL CMainWindow::OnChar(MRESULT &, USHORT flags, UCHAR, UCHAR, USHORT Letter, USHORT Virtual)
{
	if (flags & KC_KEYUP)
		return FALSE;

	if (m_Stage == G_STAGE_2)
	{
		if (m_GameOver)
			return FALSE;
		if (m_InfoBox)
			return FALSE;

		if (flags & KC_CTRL)
		{
			if ((Letter == 'f')||(Letter == 'F'))
			{
				FindNext();
				return TRUE;
			}

			if ((Letter == 't')||(Letter == 'T'))
			{
				Turn();
				return TRUE;
			}
		}
	}

	if (Virtual == VK_ESC)
		return TRUE;

	return FALSE;
}

void CMainWindow::FindNext()
{
	if (m_Stage == G_STAGE_2)
	{
		BOOL WasNull = (m_LastFound == NULL);

		m_LastFound = m_Board->FindNext(m_Player, m_LastFound);
		if (m_LastFound != NULL)
			Select(m_LastFound->GetXPos(), m_LastFound->GetYPos());
		else
		{
			if (!WasNull)
			{
				m_LastFound = m_Board->FindNext(m_Player, m_LastFound);
				if (m_LastFound != NULL)
					Select(m_LastFound->GetXPos(), m_LastFound->GetYPos());
				else
				{
					::WinAlarm(HWND_DESKTOP, WA_NOTE);
				}
			}
			else
			{
				::WinAlarm(HWND_DESKTOP, WA_NOTE);
			}
		}
	}
}

void CMainWindow::Play(USHORT Player, BOOL Computer)
{
	CBindMainWindow Bind;
	Bind.Set(*this);
	(m_PlayerBuilds[(LONG)Player - 1])->BuildFighters(*m_Board, Bind, Computer);

	m_Board->Move(Player, Bind);
	m_LastFound = NULL;
}

void CMainWindow::Turn()
{
	if (m_Stage == G_STAGE_2)
	{
		if (m_Player == 1)
		{
			Play(m_Player, FALSE);
			m_Player = 2;
         UpdatePlayer();
			if(m_ComputerP2)
			{
				m_Board->Choose(m_Player);
				Play(m_Player, TRUE);
				m_Player = 1;
				UpdatePlayer();
			}
			if (m_Board->HasWon())
				GameOver();
		}
		else
		{
			Play(m_Player, FALSE);
			m_Player = 1;
			UpdatePlayer();
			if (m_ComputerP1)
			{
				m_Board->Choose(m_Player);
				Play(m_Player, TRUE);
				m_Player = 2;
				UpdatePlayer();
			}
			if (m_Board->HasWon())
				GameOver();
		}
		MakeFocus(m_XSelect, m_YSelect);
	}
}

BOOL CMainWindow::OnCmdOther(MRESULT &, USHORT Which, BOOL)
{
	if (m_GameOver)
		return FALSE;

	if (m_InfoBox)
		return FALSE;

	if (m_Stage == G_STAGE_2)
	{
		switch(Which)
		{
			case DID_CONTROL_CENTER:
										Turn();
										return TRUE;
			case BID_FIND:
										FindNext();
										return TRUE;
		}
	}
	return FALSE;
}

void CMainWindow::Target(BOOL Weapon)
{
	if (m_Stage == G_STAGE_2)
	{
		if (m_Board->GetPart(m_XSelect, m_YSelect).HasFighter())
		{
			CFighter *Fighter;

			Fighter = m_Board->GetPart(m_XSelect, m_YSelect).GetFighter();

			if (Fighter->GetControl() == m_Player)
			{
				if (Weapon)
				{
					if (m_Board->GetPart(m_XTarget, m_YTarget).HasFighter())
					{
						Fighter->SetAttached(m_Board->GetPart(m_XTarget, m_YTarget).GetFighter());
						Fighter->SetDestX(m_XTarget);
						Fighter->SetDestY(m_YTarget);
					}
					else
					{
						Fighter->SetAttached(NULL);
						Fighter->SetDestX(m_XTarget);
						Fighter->SetDestY(m_YTarget);
					}
				}
				else
				{
					Fighter->SetAttached(NULL);
					Fighter->SetDestX(m_XTarget);
					Fighter->SetDestY(m_YTarget);
				}
			}
		}
	}
}

BOOL CMainWindow::OnCmdMenu(MRESULT &, USHORT Which, BOOL)
{
	if (m_InfoBox)
		return FALSE;

	CHelpInstanceWindow Help;


	switch(Which)
	{
		case MID_NEW:
						SetMap();
						return FALSE;
		case MID_PLAYERS:
						SetPlayers();
						return TRUE;
		case MID_EXIT:
						SendMsg(WM_CLOSE, 0L, 0L);
						return TRUE;
		case MID_TARGET_LOCATION:
						if (!m_GameOver)
							Target(FALSE);
						return TRUE;
		case MID_TARGET_WEAPON:
						if (!m_GameOver)
							Target(TRUE);
						return TRUE;
		case MID_RESCUE:
						if (!m_GameOver)
							Target(TRUE);
						return TRUE;
		case MID_CONTROL_RESOURCES:
						if (!m_GameOver)
							SetupResources();
						return TRUE;
		case MID_TURN:
						if (!m_GameOver)
							Turn();
						return TRUE;
		case MID_FIND:
						if (!m_GameOver)
							FindNext();
						return TRUE;
		case MID_HELP_INDEX:
						Help = GetHelpInstance();
						Help.ShowContents();
						return TRUE;
		case MID_GENERAL_HELP:
						Help = GetHelpInstance();
						Help.ShowGeneral();
						return TRUE;
		case MID_USING_HELP:
						Help = GetHelpInstance();
						Help.ShowUsing();
						return TRUE;
		case MID_KEYS_HELP:
						Help = GetHelpInstance();
						Help.ShowKeys();
						return TRUE;
		case MID_REGISTRATION_HELP:
						Help = GetHelpInstance();
						Help.ShowPanel(550);
						return TRUE;
		case MID_HIGH_SCORES:
						HighScores();
						return TRUE;
		case MID_HELP_ABOUT:
						About();
						return TRUE;
	}
	return FALSE;
}

void CMainWindow::Select(SHORT XPos, SHORT YPos)
{
	if (m_Stage == G_STAGE_2)
	{
		if ((XPos == m_XSelect)&&(YPos == m_YSelect))
			return;

		if (m_Board->OutOfBounds(XPos, YPos))
			return;

		SHORT OldX, OldY;

		OldX = m_XSelect;
		OldY = m_YSelect;

		m_XSelect = XPos;
		m_YSelect = YPos;

		DisplayPiece(OldX, OldY, TRUE);
		DisplayPiece(m_XSelect, m_YSelect, TRUE);
		MakeFocus(m_XSelect, m_YSelect);
	}
}

BOOL CMainWindow::OnButton1Down(MRESULT &, SHORT XMouse, SHORT YMouse, USHORT, USHORT)
{
	if (m_Stage == G_STAGE_2)
	{
		if ((XMouse < 0) && (YMouse < 0))
			return FALSE;

		LONG x1, y1, x2, y2;

		GetWindowRect(x1, y1, x2, y2);

		SHORT XPos, YPos;

		XPos = (SHORT)(((LONG)XMouse / 32) + (LONG)m_XShift);
		YPos = (SHORT)(((y2 - (LONG)YMouse) / 32) + (LONG)m_YShift);

		Select(XPos, YPos);
		return TRUE;
	}
	return FALSE;
}

BOOL CMainWindow::RightClick(SHORT XMouse, SHORT YMouse)
{
	if ((XMouse < 0) && (YMouse < 0))
		return FALSE;

	LONG x1, y1, x2, y2;

	GetWindowRect(x1, y1, x2, y2);

	SHORT XPos, YPos;

	XPos = (SHORT)(((LONG)XMouse / 32) + (LONG)m_XShift);
	YPos = (SHORT)(((y2 - (LONG)YMouse) / 32) + (LONG)m_YShift);

	if ((XPos == m_XSelect)&&(YPos == m_YSelect))
		return FALSE;

	if (m_Board->GetPart(m_XSelect, m_YSelect).HasFighter())
	{
		CFighter *Fighter;

		Fighter = m_Board->GetPart(m_XSelect, m_YSelect).GetFighter();
		if (Fighter->GetControl() == m_Player)
		{
			m_XTarget = XPos;
			m_YTarget = YPos;

			LONG xmap1, ymap1;
			xmap1 = XMouse;
			ymap1 = YMouse;

//			Target(FALSE);

			MapPointsToWindow(CWindow(HWND_DESKTOP), xmap1, ymap1);
			if (m_Board->GetPart(XPos, YPos).HasFighter())
			{
				if (GetWhich(Fighter->GetPiece()) == 8)  // HERE WEAPONS
					MenuRescueTwo(xmap1,ymap1);
				else
					MenuTargetTwo(xmap1,ymap1);
			}
			else
				MenuTargetOne(xmap1,ymap1);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CMainWindow::OnButton2Down(MRESULT &, SHORT x, SHORT y, USHORT, USHORT)
{
	if (m_Stage == G_STAGE_2)
		return RightClick(x, y);
	return FALSE;
}

BOOL CMainWindow::OnButton3Down(MRESULT &, SHORT x, SHORT y, USHORT, USHORT)
{
	if (m_Stage == G_STAGE_2)
		return RightClick(x, y);
	return FALSE;
}

void CMainWindow::MenuTargetOne(LONG x, LONG y)
{
	m_MenuTargetOne.PopupMenu(CWindow(HWND_DESKTOP), *this, x, y, 0,
				PU_HCONSTRAIN | PU_VCONSTRAIN |
				PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 | PU_KEYBOARD);
}

void CMainWindow::MenuTargetTwo(LONG x, LONG y)
{
	m_MenuTargetTwo.PopupMenu(CWindow(HWND_DESKTOP), *this, x, y, 0,
				PU_HCONSTRAIN | PU_VCONSTRAIN |
				PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 | PU_KEYBOARD);
}

void CMainWindow::MenuRescueTwo(LONG x, LONG y)
{
	m_MenuRescueTwo.PopupMenu(CWindow(HWND_DESKTOP), *this, x, y, 0,
				PU_HCONSTRAIN | PU_VCONSTRAIN |
				PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 | PU_KEYBOARD);
}

void CMainWindow::GameOver()
{
	if (m_Stage == G_STAGE_2)
	{
		m_GameOver = TRUE;
		m_Control.HideAll();
		CGameOverDlg Test(m_Board->HasWonP1());

		Test.SetP1(m_Board->GetXSize(), m_Board->GetYSize(), m_Board->GetTotalVillages(), m_Skill, m_Board->GetTurnsP1(), m_Board->GetDestroyedP1(), m_ComputerP1, *this);
		Test.SetP2(m_Board->GetXSize(), m_Board->GetYSize(), m_Board->GetTotalVillages(), m_Skill, m_Board->GetTurnsP2(), m_Board->GetDestroyedP2(), m_ComputerP2, *this);

		Test.Dlg(CWindow(HWND_DESKTOP), *this, NULLHANDLE, DID_GAME_OVER);

		HighScores();

		m_Stage = G_STAGE_1;

		ObjectDelete(m_Board);
		ObjectDelete(m_PlayerBuilds[0]);
		ObjectDelete(m_PlayerBuilds[1]);

		m_MainMenu.SetDisabled(MID_CONTROL_RESOURCES, TRUE);
		m_MainMenu.SetDisabled(MID_FIND, TRUE);
		m_MainMenu.SetDisabled(MID_TURN, TRUE);
		RepaintAll();
	}
}

void CMainWindow::HighScores()
{
	if (m_Stage == G_STAGE_2)
		m_Control.HideAll();

	m_InfoBox = TRUE;
	CHighScoresDlg High;

	High.Dlg(CWindow(HWND_DESKTOP), *this, NULLHANDLE, DID_HIGH_SCORES);
	m_InfoBox = FALSE;
}

void CMainWindow::About()
{
	if (m_Stage == G_STAGE_2)
		m_Control.HideAll();

	m_InfoBox = TRUE;
	CAboutDlg About;

	About.Dlg(CWindow(HWND_DESKTOP), *this, NULLHANDLE, DID_ABOUT);
	m_InfoBox = FALSE;
	m_HasRegistered = VerifyRegister();
}

void CMainWindow::RegisterMe()
{
	if (m_Stage == G_STAGE_2)
		m_Control.HideAll();

	m_InfoBox = TRUE;
	CRegisterMeDlg Reg;

	Reg.Dlg(CWindow(HWND_DESKTOP), *this, NULLHANDLE, DID_REGISTER_ME);
	m_InfoBox = FALSE;
}

void CMainWindow::SetPlayers()
{
	if (m_Stage == G_STAGE_2)
	{
   	m_InfoBox = TRUE;
		CAbandonDlg Test;

		m_Control.HideAll();

		if(Test.Dlg(CWindow(HWND_DESKTOP), *this, NULLHANDLE, DID_ABANDON_GAME))
		{
			m_Stage = G_STAGE_1;
			ObjectDelete(m_Board);
			ObjectDelete(m_PlayerBuilds[0]);
			ObjectDelete(m_PlayerBuilds[1]);

			m_MainMenu.SetDisabled(MID_CONTROL_RESOURCES, TRUE);
			m_MainMenu.SetDisabled(MID_FIND, TRUE);
			m_MainMenu.SetDisabled(MID_TURN, TRUE);
			RepaintAll();
		}
		else
			UpdateControl(m_XSelect, m_YSelect);
		m_InfoBox = FALSE;
	}

	if (m_Stage == G_STAGE_1)
	{
		CPlayersDlg Test;

		Test.Set(m_ComputerP1, m_ComputerP2, m_Skill);

		if(Test.Dlg(CWindow(HWND_DESKTOP), *this,
						NULLHANDLE, DID_PLAYER_CONTROL))
		{
			Test.Get(m_ComputerP1, m_ComputerP2, m_Skill);
		}
	}
}

void CMainWindow::SetupResources()
{
	if (m_Stage != G_STAGE_2)
		return;

	m_Control.HideAll();

	if (m_HasRegistered == FALSE)
	{
		if (m_ShownRegister == FALSE)
		{
			RegisterMe();
			m_ShownRegister = TRUE;
		}
	}

	m_InfoBox = TRUE;

	CResourceDlg Test(*(m_PlayerBuilds[(LONG)m_Player - 1]), m_Player);

	Test.Dlg(CWindow(HWND_DESKTOP), *this, NULLHANDLE, DID_RESOURCES);

	m_InfoBox = FALSE;

	UpdateControl(m_XSelect, m_YSelect);
}

void CMainWindow::SetMap()
{
	if (m_Stage == G_STAGE_2)
	{
		m_InfoBox = TRUE;
		m_Control.HideAll();

		CAbandonDlg Test;

		if(Test.Dlg(CWindow(HWND_DESKTOP), *this, NULLHANDLE, DID_ABANDON_GAME))
		{
			m_Stage = G_STAGE_1;
			ObjectDelete(m_Board);
			ObjectDelete(m_PlayerBuilds[0]);
			ObjectDelete(m_PlayerBuilds[1]);

			m_MainMenu.SetDisabled(MID_CONTROL_RESOURCES, TRUE);
			m_MainMenu.SetDisabled(MID_FIND, TRUE);
			m_MainMenu.SetDisabled(MID_TURN, TRUE);
			RepaintAll();
		}
		else
			UpdateControl(m_XSelect, m_YSelect);
		m_InfoBox = FALSE;
	}

	if (m_Stage == G_STAGE_1)
	{
		CMapDlg Test;

		Test.Set(m_ALightForest, m_ADenseForest, m_ASand, m_ASwamp, m_AVillages, m_ABridges, m_AXSize, m_AYSize, m_ARiver);
		if(Test.Dlg(CWindow(HWND_DESKTOP), *this,
					NULLHANDLE, DID_CREATE_MAP))
		{
			m_Player = 1;
			CDialogBox Wait;
			Wait.LoadDlg(CWindow(HWND_DESKTOP), *this, NULLHANDLE, DID_PLEASE_WAIT_MAP);
			Test.Get(m_ALightForest, m_ADenseForest, m_ASand, m_ASwamp, m_AVillages, m_ABridges, m_AXSize, m_AYSize, m_ARiver);

			m_LightForest = m_ALightForest;
			m_DenseForest = m_ADenseForest;
			m_Sand = m_ASand;
			m_Swamp = m_ASwamp;
			m_Villages = m_AVillages;
			m_Bridges = m_ABridges;
			m_XSize = m_AXSize;
			m_YSize = m_AYSize;
			m_River = m_ARiver;

			m_XSize += 20;
			m_YSize += 20;

			m_Board = (CBoard *)ObjectNew(CBoard);
			m_PlayerBuilds[0] = (CBuild *)ObjectNew(CBuild);
			m_PlayerBuilds[1] = (CBuild *)ObjectNew(CBuild);

			(m_PlayerBuilds[0])->SetPlayer(1);
			(m_PlayerBuilds[1])->SetPlayer(2);

			if (m_ComputerP1)
				(m_PlayerBuilds[0])->SetTotal((USHORT)((((LONG)m_XSize * (LONG)m_YSize) / 200) + (LONG)m_Skill));
			else
				(m_PlayerBuilds[0])->SetTotal((USHORT)(((LONG)m_XSize * (LONG)m_YSize) / 200));

			if (m_ComputerP2)
				(m_PlayerBuilds[1])->SetTotal((USHORT)((((LONG)m_XSize * (LONG)m_YSize) / 200) + (LONG)m_Skill));
			else
				(m_PlayerBuilds[1])->SetTotal((USHORT)(((LONG)m_XSize * (LONG)m_YSize) / 200));

			m_Board->SizeBoard(m_XSize, m_YSize);
			m_Board->GenerateMap((USHORT)(((LONG)m_XSize * (LONG)m_YSize) / 45), m_LightForest, m_DenseForest, m_Sand, m_Swamp);
			if (m_River)
			{
				m_Board->SetRiver(m_Bridges);
			}

			++m_Villages;

			m_Villages *= (USHORT)(((LONG)m_XSize * (LONG)m_YSize) / 200);
			m_Board->AddVillages(m_Villages);
			m_Board->DefinePieces();

			m_XShift = 0;
			m_YShift = 0;

			m_XSelect = 0;
			m_YSelect = 0;

			m_Dest = FALSE;

			m_LastFound = FALSE;

			LONG x1, y1, x2, y2;

			GetWindowRect(x1, y1, x2, y2);

			m_Y2 = y2;

			GetTotalViewed();

			CScrollBarWindow Tmp;
			CWindow Frame;

			GetParent(Frame);
			Frame.GetChild(Tmp, FID_HORZSCROLL);
			Tmp.SetScrollBar(0, 0, (SHORT)((LONG)m_XSize - 1));
			Tmp.SetThumbSize(m_XViewed, m_XSize);
			Frame.GetChild(Tmp, FID_VERTSCROLL);
			Tmp.SetScrollBar(0, 0, (SHORT)((LONG)m_YSize - 1));
			Tmp.SetThumbSize(m_YViewed, m_YSize);

			m_Stage = G_STAGE_2;

			Frame.GetChild(m_MainMenu, FID_MENU);

			m_MainMenu.SetEnabled(MID_CONTROL_RESOURCES, TRUE);
			m_MainMenu.SetEnabled(MID_FIND, TRUE);
			m_MainMenu.SetEnabled(MID_TURN, TRUE);
			RepaintAll();
			Wait.DestroyWindow();

			if (m_ComputerP1)
				m_Player = 2;

			m_GameOver = FALSE;

			UpdatePlayer();
			UpdateControl(0, 0);
		}
	}
}

BOOL CMainWindow::OnSize(MRESULT &, SHORT, SHORT, SHORT, SHORT)
{
	if (m_Stage == G_STAGE_2)
	{
		LONG x1, y1, x2, y2;

		GetWindowRect(x1, y1, x2, y2);

		m_Y2 = y2;

		// FIX THIS - When resizing, if the resized fits all displayed pieces,
		// but the x or y shift is set then the data will not all be displayed

		GetTotalViewed();

		m_XShift = 0;
		m_YShift = 0;

		CScrollBarWindow Tmp;
		CWindow Frame;

		GetParent(Frame);
		Frame.GetChild(Tmp, FID_HORZSCROLL);
		Tmp.SetScrollBar(0, 0, (SHORT)((LONG)m_XSize - 1));
		Tmp.SetThumbSize(m_XViewed, m_XSize);
		Frame.GetChild(Tmp, FID_VERTSCROLL);
		Tmp.SetScrollBar(0, 0, (SHORT)((LONG)m_YSize - 1));
		Tmp.SetThumbSize(m_YViewed, m_YSize);

		return FALSE;
	}
	return FALSE;
}

void main()
{
	time_t t;

	srand((unsigned) time(&t));

	HAB MainHAB;
	HMQ MainHMQ;

	InitializeWindows(MainHAB, MainHMQ);

	ULONG FrameFlags = FCF_TITLEBAR | FCF_SYSMENU | FCF_SIZEBORDER |
							 FCF_BORDER | FCF_MINMAX | FCF_SHELLPOSITION |
							 FCF_TASKLIST | FCF_VERTSCROLL | FCF_HORZSCROLL | FCF_MENU |
							 FCF_ICON;// | FCF_ACCELTABLE;

	RegisterClass(MainHAB, CS_SIZEREDRAW, CS_SIZEREDRAW, CS_SIZEREDRAW);

	CMainWindow MainWindow;
	CHelpInstanceWindow HelpWindow;


	MainWindow.CreateStdWindow(CWindow(HWND_DESKTOP),
											WS_VISIBLE,
											&FrameFlags,
											"PM War Over Villages",
											0L,
											NULLHANDLE,
											ID_RESOURCE);

	ULONG Test;
	Test = HelpWindow.Create(MainHAB, NULL, ID_RESOURCE, NULLHANDLE,
							NULLHANDLE, ID_RESOURCE, 0,
							"PM War Over Villages Help",
							FALSE,
							"WARGAME.HLP");

	CWindow Frame;

	MainWindow.GetParent(Frame);

	Frame.Associate(HelpWindow);

	QMSG Message;

	while (WinGetMsg(MainHAB, &Message, NULLHANDLE, 0, 0))
		WinDispatchMsg(MainHAB, &Message);

	Frame.DestroyWindow();

	HelpWindow.Destroy();

	TerminateWindows(MainHAB, MainHMQ);
}

