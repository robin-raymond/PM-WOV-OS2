#define _COMMON_CWINDOW_H_
#define _COMMON_COBLIST_H_
#include "common.h"

#include "bitmaps.h"
#include "board.hpp"

// RANDOM FUNCTION DEFINITION
#include <stdlib.h>
//#include <iostream.h>
//#include <conio.h>
//#include <time.h>
#define Random(x) (rand() % x)

// END OF RANDOM FUNCTION DEFINITION

#define TOTAL_BITMAPS 110
#define START_BITMAP_ID BITID_SBRCBR
#define END_BITMAP_ID BITID_NOWHERE

HBITMAP bitmaphandles[TOTAL_BITMAPS];

void LoadBitmaps(HPS hps)
{
	ULONG id;

	for(id  = START_BITMAP_ID; id <= END_BITMAP_ID; ++id)
	{
		bitmaphandles[id-START_BITMAP_ID] = ::GpiLoadBitmap(hps, NULL, id, 0L, 0L);
	}
}

void DeleteBitmaps()
{
	ULONG id;

	for(id  = START_BITMAP_ID; id <= END_BITMAP_ID; ++id)
	{
		::GpiDeleteBitmap(bitmaphandles[id-START_BITMAP_ID]);
	}
}

void DisplayBitmap(HPS hps, ULONG id, LONG x1, LONG y1)
{
	POINTL point;

	point.x = x1;
	point.y = y1;

	::WinDrawBitmap(hps, bitmaphandles[id-START_BITMAP_ID], NULL, &point,
						 DBM_IMAGEATTRS, DBM_IMAGEATTRS, DBM_NORMAL);
}

void DisplayStretchBitmap(HPS hps, ULONG id, RECTL &rect)
{
	::WinDrawBitmap(hps, bitmaphandles[id-START_BITMAP_ID], NULL, (PPOINTL)&rect,
						 DBM_IMAGEATTRS, DBM_IMAGEATTRS, DBM_STRETCH);
}

USHORT g_Strongest = 1;

USHORT GetStrongest()
{
	return g_Strongest;
}

void SetStrongest(USHORT Strongest)
{
	g_Strongest = Strongest;
}

CFighter::CFighter()
{
	m_Control = 0;
	ResetPointer((BINARY &)m_Attached);

	m_GroundFighter = FALSE;
	m_Plane = FALSE;

	m_Attack = 0;
	m_Defense = 0;
	m_ODefense = 0;
	m_XPos = 0;
	m_YPos = 0;
	m_DestX = 0;
	m_DestY = 0;
	m_Moves = 0;

	m_SpeedGround = 0;
	m_SpeedLightForest = 0;
	m_SpeedDenseForest = 0;
	m_SpeedSand = 0;
	m_SpeedSwamp = 0;
	m_SpeedWater = 0;

	m_Destroyed = FALSE;

	m_Piece = BITID_GROUNDF1;
}

BOOL CFighter::SmartDecide(CBoard &Board)
{
	if (HasFighter())
		return FALSE;
	else
	{
		if ((m_Piece == BITID_MISSILE1)||(m_Piece == BITID_MISSILE2))
			return TRUE;
		if ((m_Piece == BITID_BOMBER1)||(m_Piece == BITID_BOMBER2))
			return TRUE;
		if ((m_Piece == BITID_FIGHTER1)||(m_Piece == BITID_FIGHTER2))
			return TRUE;
		if ((m_Piece == BITID_REPAIR1)||(m_Piece == BITID_REPAIR2))  // HERE WEAPON
			return TRUE;

		if (Still())
			return TRUE;
		if ((Board.GetPart(m_DestX, m_DestY)).IsVillage())
		{
			if (m_Control == 1)
			{
				if ((Board.GetPart(m_DestX, m_DestY)).GetPiece() == BITID_VILLAGE1)
					return TRUE;
			}
			else
			{
				if (m_Control == 2)
				{
					if ((Board.GetPart(m_DestX, m_DestY)).GetPiece() == BITID_VILLAGE2)
						return TRUE;
				}
			}

			return FALSE;
		}
		return TRUE;
	}
}

BOOL CFighter::SmartFindDecide(CBoard &)
{
	if (HasFighter())
		return FALSE;
	else
	{
		if ((m_Piece == BITID_MISSILE1)||(m_Piece == BITID_MISSILE2))
			return TRUE;
		if ((m_Piece == BITID_BOMBER1)||(m_Piece == BITID_BOMBER2))
			return TRUE;
		if ((m_Piece == BITID_FIGHTER1)||(m_Piece == BITID_FIGHTER2)) // HERE WEAPON
			return TRUE;
		if ((m_Piece == BITID_REPAIR1)||(m_Piece == BITID_REPAIR2))
			return TRUE;

		if (Still())
			return TRUE;
		return FALSE;
	}
}

void CFighter::Fight(CFighter &Fighter)
{
	if ((m_Destroyed)||(Fighter.m_Destroyed))
		return;

	USHORT a1, a2;

	a1 = (USHORT)Random((LONG)m_Attack * 2);
	a2 = (USHORT)Random((LONG)Fighter.m_Attack * 2);

	if (m_Defense < a2)
		m_Destroyed = TRUE;
	else
		m_Defense -= (USHORT)(((LONG)a2 / 2) + 1);

	if (Fighter.m_Defense < a1)
		Fighter.m_Destroyed = TRUE;
	else
		Fighter.m_Defense -= (USHORT)(((LONG)a1 / 2) + 1);

	if ((m_Destroyed == FALSE)&&(Fighter.m_Destroyed == FALSE))
		Fight(Fighter);
}

BOOL CFighter::CheckMove(USHORT Speed)
{
	if (m_Plane)
	{
		m_Moves = 0;
		return TRUE;
	}

	if (m_Moves >= Speed)
	{
		m_Moves -= Speed;
		return TRUE;
	}
	return FALSE;
}

BOOL CFighter::Still()
{
	if (m_Attached == NULL)
	{
		if ((m_XPos == m_DestX)&&(m_YPos == m_DestY))
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CFighter::UpdateLink()
{
	if (m_Attached != NULL)
	{
		m_DestX = m_Attached->GetXPos();
		m_DestY = m_Attached->GetYPos();
	}
}

CPart::CPart()
{
	SetOpenField();
	ResetPointer((BINARY &)m_Fighter);
	m_Bridge = FALSE;
}

void CPart::SetValues(BOOL Val1, BOOL Val2, BOOL Val3, BOOL Val4, BOOL Val5, BOOL Val6)
{
	m_River = Val1;
	m_LightForest = Val2;
	m_DenseForest = Val3;
	m_Village = Val4;
	m_Sand = Val5;
	m_Swamp = Val6;

	m_Control = 0;
	m_Palette = 0;
}

BOOL CPart::IsOpenField()
{
	if (m_River)
		return FALSE;
	if (m_LightForest)
		return FALSE;
	if (m_DenseForest)
		return FALSE;
	if (m_Village)
		return FALSE;
	if (m_Sand)
		return FALSE;
	if (m_Swamp)
		return FALSE;
	return TRUE;
}

void CPart::SetBridge()
{
	if (m_River)
		m_Bridge = TRUE;
}

void CPart::SetControl(USHORT m_Player)
{
	if (m_Village)
		m_Control = m_Player;
}

void CPart::SetPalette(USHORT Palette)
{
	m_Palette = Palette;
}

ULONG CPart::SandPiece(BOOL tl, BOOL t, BOOL tr, BOOL l, BOOL r, BOOL bl, BOOL b, BOOL br)
{
	tl = min(TRUE,tl);
	t = min(TRUE,t);
	tr = min(TRUE,tr);
	l = min(TRUE,l);
	r = min(TRUE,r);
	bl = min(TRUE,bl);
	b = min(TRUE,b);
	br = min(TRUE,br);

	switch (t)
	{
		case TRUE:
			switch(b)
			{
				case TRUE:
					switch(l)
					{
						case TRUE:
							switch(r)
							{
								case TRUE:
									switch(tl)
									{
										case TRUE:
											switch(tr)
											{
												case TRUE:
													switch (bl)
													{
														case TRUE:
															switch (br)
															{
																case TRUE:
																	return BITID_STBLR;
																case FALSE:
																	return BITID_SA2;
															}
														case FALSE:
															switch (br)
															{
																case TRUE:
																	return BITID_SA3;
																case FALSE:
																	return BITID_SA23;
															}
													}
												case FALSE:
													switch (bl)
													{
														case TRUE:
															switch (br)
															{
																case TRUE:
																	return BITID_SA1;
																case FALSE:
																	return BITID_SA12;
															}
														case FALSE:
															switch (br)
															{
																case TRUE:
																	return BITID_SA13;
																case FALSE:
																	return BITID_SA123;
															}
													}
											}
										case FALSE:
											switch(tr)
											{
												case TRUE:
													switch (bl)
													{
														case TRUE:
															switch (br)
															{
																case TRUE:
																	return BITID_SA4;
																case FALSE:
																	return BITID_SA24;
															}
														case FALSE:
															switch (br)
															{
																case TRUE:
																	return BITID_SA34;
																case FALSE:
																	return BITID_SA234;
															}
													}
												case FALSE:
													switch (bl)
													{
														case TRUE:
															switch (br)
															{
																case TRUE:
																	return BITID_SA14;
																case FALSE:
																	return BITID_SA124;
															}
														case FALSE:
															switch (br)
															{
																case TRUE:
																	return BITID_SA134;
																case FALSE:
																	return BITID_SA1234;
															}
													}
											}
									}
								case FALSE:
									switch(tl)
									{
										case TRUE:
											switch (bl)
											{
												case TRUE:
													return BITID_STBL;
												case FALSE:
													return BITID_STBLCBL;
											}
										case FALSE:
											switch (bl)
											{
												case TRUE:
													return BITID_STBLCTL;
												case FALSE:
													return BITID_STBLCB;
											}
									}
							}
						case FALSE:
							switch(r)
							{
								case TRUE:
									switch(tr)
									{
										case TRUE:
											switch(br)
											{
												case TRUE:
													return BITID_STBR;
												case FALSE:
													return BITID_STBRCBR;
											}
										case FALSE:
											switch(br)
											{
												case TRUE:
													return BITID_STBRCTR;
												case FALSE:
													return BITID_STBRCB;
											}
									}
								case FALSE:
									return BITID_STB;
							}
					}
				case FALSE:
					switch(l)
					{
						case TRUE:
							switch(r)
							{
								case TRUE:
									switch(tl)
									{
										case TRUE:
											switch(tr)
											{
												case TRUE:
													return BITID_STLR;
												case FALSE:
													return BITID_STLRCTR;
											}
										case FALSE:
											switch(tr)
											{
												case TRUE:
													return BITID_STLRCTL;
												case FALSE:
													return BITID_STLRCB;
											}
									}
								case FALSE:
									switch(tl)
									{
										case TRUE:
											return BITID_STL;
										case FALSE:
											return BITID_STLCTL;
									}
							}
						case FALSE:
							switch(r)
							{
								case TRUE:
									switch(tr)
									{
										case TRUE:
											return BITID_STR;
										case FALSE:
											return BITID_STRCTR;
									}
								case FALSE:
									return BITID_ST;
							}
					}
			}
		case FALSE:
			switch(b)
			{
				case TRUE:
					switch(l)
					{
						case TRUE:
							switch(r)
							{
								case TRUE:
									switch(bl)
									{
										case TRUE:
											switch(br)
											{
												case TRUE:
													return BITID_SBLR;
												case FALSE:
													return BITID_SBLRCBR;
											}
										case FALSE:
											switch(br)
											{
												case TRUE:
													return BITID_SBLRCBL;
												case FALSE:
													return BITID_SBLRCB;
											}
									}
								case FALSE:
									switch(bl)
									{
										case TRUE:
											return BITID_SBL;
										case FALSE:
											return BITID_SBLCBL;
									}
							}
						case FALSE:
							switch(r)
							{
								case TRUE:
									switch(br)
									{
										case TRUE:
											return BITID_SBR;
										case FALSE:
											return BITID_SBRCBR;
									}
								case FALSE:
									return BITID_SB;
							}
					}
				case FALSE:
					switch(l)
					{
						case TRUE:
							switch(r)
							{
								case TRUE:
									return BITID_SLR;
								case FALSE:
									return BITID_SL;
							}
						case FALSE:
							switch(r)
							{
								case TRUE:
									return BITID_SR;
								case FALSE:
									return BITID_SNONE;
							}
					}
			}
	}
	return BITID_NOWHERE;
}

ULONG CPart::RiverPiece(BOOL t, BOOL b, BOOL l, BOOL r, BOOL bridge)
{
	t = min(TRUE,t);
	l = min(TRUE,l);
	r = min(TRUE,r);
	b = min(TRUE,b);
	bridge = min(TRUE, bridge);

	switch(t)
	{
		case TRUE:
			switch(b)
			{
				case TRUE:
					switch(l)
					{
						case TRUE:
							switch(r)
							{
								case TRUE:
									return BITID_RSTBL;
								case FALSE:
									return BITID_RSTBL;
							}
						case FALSE:
							switch(r)
							{
								case TRUE:
									return BITID_RSTBR;
								case FALSE:
									switch(bridge)
									{
										case TRUE:
                              	return BITID_BTB;
										case FALSE:
											return BITID_RTB;
									}
							}
					}
				case FALSE:
					switch(l)
					{
						case TRUE:
							switch(r)
							{
								case TRUE:
									return BITID_RSLRT;
								case FALSE:
									return BITID_RTLC;
							}
						case FALSE:
							return BITID_RTRC;
					}
			}
		case FALSE:
			switch(b)
			{
				case TRUE:
					switch(l)
					{
						case TRUE:
							switch(r)
							{
								case TRUE:
									return BITID_RSLRB;
								case FALSE:
									return BITID_RBLC;
							}
						case FALSE:
							return BITID_RBRC;
					}
				case FALSE:
					switch(bridge)
					{
						case TRUE:
							return BITID_BLR;
						case FALSE:
							return BITID_RLR;
					}
			}
	}
	return BITID_NOWHERE;
}

void CPart::ChoosePiece(CBoard &Board, SHORT XPos, SHORT YPos)
{
	if (IsRiver())
	{
		m_Piece = (ULONG)RiverPiece((Board.GetRiverPart(XPos, (SHORT)(((LONG)YPos) - 1))).IsRiver(),
									(Board.GetRiverPart(XPos, (SHORT)((LONG)YPos + 1))).IsRiver(),
									(Board.GetRiverPart((SHORT)((LONG)XPos - 1), YPos)).IsRiver(),
									(Board.GetRiverPart((SHORT)((LONG)XPos + 1), YPos)).IsRiver(),
									HasBridge());
	}
	else
	{  // NOT RIVER
		if (IsSand())
		{
			m_Piece = SandPiece(Board.GetSandPart((SHORT)((LONG)XPos - 1), (SHORT)((LONG)YPos - 1)).IsSand(),
									  Board.GetSandPart(XPos, (SHORT)((LONG)YPos - 1)).IsSand(),
									  Board.GetSandPart((SHORT)((LONG)XPos + 1), (SHORT)((LONG)YPos - 1)).IsSand(),
									  Board.GetSandPart((SHORT)((LONG)XPos - 1), YPos).IsSand(),
									  Board.GetSandPart((SHORT)((LONG)XPos + 1), YPos).IsSand(),
									  Board.GetSandPart((SHORT)((LONG)XPos - 1), (SHORT)((LONG)YPos + 1)).IsSand(),
									  Board.GetSandPart(XPos, (SHORT)((LONG)YPos + 1)).IsSand(),
									  Board.GetSandPart((SHORT)((LONG)XPos + 1), (SHORT)((LONG)YPos + 1)).IsSand());
		}
		else
		{  // IS NOT SAND
			if (IsVillage())
			{
				if (m_Control == 0)
					m_Piece = BITID_VILLAGEN;
				else
					if (m_Control == 1)
						m_Piece = BITID_VILLAGE1;
					else
						m_Piece = BITID_VILLAGE2;
			}
			else
			{
				if (IsLightForest())
				{
					switch (Random(3))
					{
						case 0:
								m_Piece = BITID_LIGHT;
								break;
						case 1:
								m_Piece = BITID_LIGHT2;
								break;
						default:
								m_Piece = BITID_LIGHT3;
								break;
					}
				}
				else
				{
					if (IsDenseForest())
					{
						switch (Random(3))
						{
							case 0:
									m_Piece = BITID_DENSE;
									break;
							case 1:
									m_Piece = BITID_DENSE2;
									break;
							default:
									m_Piece = BITID_DENSE3;
									break;
						}
					}
					else
					{
						if (IsSwamp())
						{
							switch (Random(6))
							{
								case 0:
										m_Piece = BITID_SWAMP;
										break;
								case 1:
										m_Piece = BITID_SWAMP2;
										break;
								case 2:
										m_Piece = BITID_SWAMP3;
										break;
								case 3:
										m_Piece = BITID_SWAMP4;
										break;
								case 4:
										m_Piece = BITID_SWAMP5;
										break;
								default:
										m_Piece = BITID_SWAMP6;
										break;
							}
						}
						else
						{
							switch(Random(2))
							{
								case 0:
										m_Piece = BITID_FIELD;
										break;
								default:
										m_Piece = BITID_FIELD2;
										break;
							}
						}
					}
				}
			}
		}
	}
}

void CBoard::DisplayActionPiece(HPS hps, LONG xpos, LONG ypos, ULONG Piece)
{
	DisplayBitmap(hps, Piece, xpos, ypos);
}

void CBoard::DisplayPiece(SHORT x, SHORT y, HPS hps, LONG xpos, LONG ypos)
{
	if ((x < 0)||(y < 0))
		DisplayBitmap(hps, BITID_NOWHERE, xpos, ypos);
	else
	{
		if ((x >= m_XSize)||(y >= m_YSize))
			DisplayBitmap(hps, BITID_NOWHERE, xpos, ypos);
		else
		{
			if (GetPart(x, y).HasFighter())
			{
				DisplayBitmap(hps, ((GetPart(x, y)).GetFighter())->GetPiece(), xpos, ypos);
			}
			else
			{
				DisplayBitmap(hps, (GetPart(x, y)).GetPiece(), xpos, ypos);
			}
		}
	}
}

CBoard::CBoard()
{
	ResetPointer((BINARY &)m_Parts);
	m_XSize = 0;
	m_YSize = 0;
	m_TotalVillages = 0;
	NullRiver.SetRiver();
	NullSand.SetSand();
	m_VillagesP1 = 0;
	m_VillagesP2 = 0;
	m_TurnsP1 = 0;
	m_TurnsP2 = 0;
	m_DestroyedP1 = 0;
	m_DestroyedP2 = 0;
}

void CBoard::SizeBoard(SHORT XSize, SHORT YSize)
{
	FreeBoard();
	if ((XSize <= 0)||(YSize <= 0))
		return;

	m_XSize = XSize;
	m_YSize = YSize;
	Alloc((BINARY &)m_Parts, sizeof(CPart **)*((USHORT)YSize));
	ResetBlock((BINARY &)m_Parts,sizeof(CPart **)*YSize);

	CheckAlloc((BINARY)m_Parts);

	SHORT YLoop, XLoop;

	for(YLoop = 0; YLoop < m_YSize; ++YLoop)
	{
		CheckBase((BINARY)m_Parts, (BINARY)m_Parts + YLoop);

		Alloc((BINARY &)*(m_Parts + YLoop),sizeof(CPart *)*XSize);
		ResetBlock((BINARY)*(m_Parts + YLoop),sizeof(CPart *)*XSize);

		CheckAlloc((BINARY)*(m_Parts + YLoop));

		CPart **Temp;
		Temp = *(m_Parts + YLoop);
		CheckAlloc((BINARY)Temp);
		for (XLoop = 0; XLoop < m_XSize; ++XLoop)
		{
			CheckBase((BINARY)Temp, (BINARY)Temp + XLoop);
			*(Temp + XLoop) = (CPart *)ObjectNew(CPart);
		}
	}
}

void CBoard::FreeBoard()
{
	if (m_Parts != NULL)
	{
		SHORT YLoop, XLoop;

		for(YLoop = 0; YLoop < m_YSize; ++YLoop)
		{
			CPart **Temp;
			Temp = *(m_Parts + YLoop);

			CheckBase((BINARY)m_Parts, (BINARY)m_Parts + YLoop);
			CheckAlloc((BINARY)Temp);
			for (XLoop = 0; XLoop < m_XSize; ++XLoop)
			{
				CheckBase((BINARY)Temp, (BINARY)Temp + XLoop);
				ObjectCheck((BINARY)*(Temp + XLoop));
				ObjectDelete(*(Temp + XLoop));
			}
			Free((BINARY &)Temp);
		}
		CheckAlloc((BINARY)m_Parts);
		Free((BINARY &)m_Parts);

		m_XSize = 0;
		m_YSize = 0;
		m_TotalVillages = 0;
		NullRiver.SetRiver();
		NullSand.SetSand();
		m_VillagesP1 = 0;
		m_VillagesP2 = 0;
		POSITION Pos;
		CDATA Pointer;

		Pointer = m_Fighters.GetHeadPosition(Pos);
		while (Pos != NULL)
		{
			CFighter *Fighter = (CFighter *)((CPointer *)Pointer)->GetValue();
			ObjectCheck((BINARY)Fighter);
			ObjectDelete(Fighter);
			m_Fighters.RemoveHead();
			Pointer = m_Fighters.GetHeadPosition(Pos);
		}
	}
}

void CBoard::DefinePieces()
{
	if (m_Parts != NULL)
	{
		SHORT YLoop, XLoop;

		for(YLoop = 0; YLoop < m_YSize; ++YLoop)
		{
			CPart **Temp;
			Temp = *(m_Parts + YLoop);

			CheckBase((BINARY)m_Parts, (BINARY)m_Parts + YLoop);
			CheckAlloc((BINARY)Temp);
			for (XLoop = 0; XLoop < m_XSize; ++XLoop)
			{
				ObjectCheck((BINARY)*(Temp + XLoop));

				(*(*(Temp + XLoop))).ChoosePiece(*this, XLoop, YLoop);
				(*(*(Temp + XLoop))).SetPalette((USHORT)Random(TOTAL_PALETTES));
			}
		}
	}
}

void CBoard::ClearAllTraces()
{
	if (m_Parts != NULL)
	{
		SHORT YLoop, XLoop;

		for(YLoop = 0; YLoop < m_YSize; ++YLoop)
		{
			CPart **Temp;
			Temp = *(m_Parts + YLoop);

			CheckBase((BINARY)m_Parts, (BINARY)m_Parts + YLoop);
			CheckAlloc((BINARY)Temp);
			for (XLoop = 0; XLoop < m_XSize; ++XLoop)
			{
				ObjectCheck((BINARY)*(Temp + XLoop));

				(*(*(Temp + XLoop))).SetTrace(NULL);
			}
		}
	}
}

CPart &CBoard::GetPart(SHORT XPos, SHORT YPos)
{
	if ((XPos < 0)||(YPos < 0))
		return Null;
	if ((XPos >= m_XSize)||(YPos >= m_YSize))
		return Null;

	CPart **Temp;
	Temp = *(m_Parts + YPos);

	CheckBase((BINARY)m_Parts, (BINARY)m_Parts + YPos);
	CheckAlloc((BINARY)Temp);
	ObjectCheck((BINARY)*(Temp + XPos));
	return *(*(Temp + XPos));
}

CPart &CBoard::GetRiverPart(SHORT XPos, SHORT YPos)
{
	if ((XPos < 0)||(YPos < 0))
		return NullRiver;
	if ((XPos >= m_XSize)||(YPos >= m_YSize))
		return NullRiver;

	CPart **Temp;
	Temp = *(m_Parts + YPos);

	CheckBase((BINARY)m_Parts, (BINARY)m_Parts + YPos);
	CheckAlloc((BINARY)Temp);
	ObjectCheck((BINARY)*(Temp + XPos));
	return *(*(Temp + XPos));
}

CPart &CBoard::GetSandPart(SHORT XPos, SHORT YPos)
{
	if ((XPos < 0)||(YPos < 0))
		return NullSand;
	if ((XPos >= m_XSize)||(YPos >= m_YSize))
		return NullSand;

	CPart **Temp;
	Temp = *(m_Parts + YPos);

	CheckBase((BINARY)m_Parts, (BINARY)m_Parts + YPos);
	CheckAlloc((BINARY)Temp);
	ObjectCheck((BINARY)*(Temp + XPos));
	return *(*(Temp + XPos));
}

#define MAXREC 40

LONG g_rec = 0;

void CBoard::SetLightForest(SHORT XPos, SHORT YPos, USHORT Strength)
{
	if ((XPos < 0)||(YPos < 0))
		return;
	if ((XPos >= m_XSize)||(YPos >= m_YSize))
		return;
	if ((GetPart(XPos, YPos)).IsLightForest())
		return;
	if (g_rec >= MAXREC)
		return;

	++g_rec;

	(GetPart(XPos, YPos)).SetLightForest();

	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetLightForest((SHORT)((LONG)XPos - 1), YPos, Strength);
	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetLightForest((SHORT)((LONG)XPos + 1), YPos, Strength);
	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetLightForest(XPos, (SHORT)((LONG)YPos - 1), Strength);
	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetLightForest(XPos, (SHORT)((LONG)YPos + 1), Strength);

	--g_rec;
}

void CBoard::SetLightForest(USHORT Number, USHORT Strength)
{
	SHORT XPos;
	SHORT YPos;

	USHORT Loop;
	for(Loop = 0; Loop < Number; ++Loop)
	{
		XPos = (SHORT)Random(m_XSize);
		YPos = (SHORT)Random(m_YSize);
		SetLightForest(XPos, YPos, Strength);
	}
}

void CBoard::SetDenseForest(SHORT XPos, SHORT YPos, USHORT Strength)
{
	if ((XPos < 0)||(YPos < 0))
		return;
	if ((XPos >= m_XSize)||(YPos >= m_YSize))
		return;
	if ((GetPart(XPos, YPos)).IsDenseForest())
		return;

	if (g_rec >= MAXREC)
		return;

	++g_rec;


	(GetPart(XPos, YPos)).SetDenseForest();

	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetDenseForest((SHORT)((LONG)XPos - 1), YPos, Strength);
	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetDenseForest((SHORT)((LONG)XPos + 1), YPos, Strength);
	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetDenseForest(XPos, (SHORT)((LONG)YPos - 1), Strength);
	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetDenseForest(XPos, (SHORT)((LONG)YPos + 1), Strength);

	--g_rec;
}

void CBoard::SetDenseForest(USHORT Number, USHORT Strength)
{
	SHORT XPos;
	SHORT YPos;

	USHORT Loop;
	for(Loop = 0; Loop < Number; ++Loop)
	{
		XPos = (SHORT)Random(m_XSize);
		YPos = (SHORT)Random(m_YSize);

		SetDenseForest(XPos, YPos, Strength);
	}
}

void CBoard::SetSand(SHORT XPos, SHORT YPos, USHORT Strength)
{
	if ((XPos < 0)||(YPos < 0))
		return;
	if ((XPos >= m_XSize)||(YPos >= m_YSize))
		return;
	if ((GetPart(XPos, YPos)).IsSand())
		return;

	if (g_rec >= MAXREC)
		return;

	++g_rec;



	(GetPart(XPos, YPos)).SetSand();

	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetSand((SHORT)((LONG)XPos - 1), YPos, Strength);
	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetSand((SHORT)((LONG)XPos + 1), YPos, Strength);
	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetSand(XPos, (SHORT)((LONG)YPos - 1), Strength);
	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetSand(XPos, (SHORT)((LONG)YPos + 1), Strength);

	--g_rec;
}

void CBoard::SetSand(USHORT Number, USHORT Strength)
{
	SHORT XPos;
	SHORT YPos;

	USHORT Loop;
	for(Loop = 0; Loop < Number; ++Loop)
	{
		XPos = (SHORT)Random(m_XSize);
		YPos = (SHORT)Random(m_YSize);

		SetSand(XPos, YPos, Strength);
	}
}

void CBoard::SetSwamp(SHORT XPos, SHORT YPos, USHORT Strength)
{
	if ((XPos < 0)||(YPos < 0))
		return;
	if ((XPos >= m_XSize)||(YPos >= m_YSize))
		return;
	if ((GetPart(XPos, YPos)).IsSwamp())
		return;

	if (g_rec >= MAXREC)
		return;

	++g_rec;

	(GetPart(XPos, YPos)).SetSwamp();

	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetSwamp((SHORT)((LONG)XPos - 1), YPos, Strength);
	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetSwamp((SHORT)((LONG)XPos + 1), YPos, Strength);
	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetSwamp(XPos, (SHORT)((LONG)YPos - 1), Strength);
	if (Random(Strength) < FILL_MAP_STRENGTH)
		SetSwamp(XPos, (SHORT)((LONG)YPos + 1), Strength);

	--g_rec;
}

void CBoard::SetSwamp(USHORT Number, USHORT Strength)
{
	SHORT XPos;
	SHORT YPos;

	USHORT Loop;
	for(Loop = 0; Loop < Number; ++Loop)
	{
		XPos = (SHORT)Random(m_XSize);
		YPos = (SHORT)Random(m_YSize);

		SetSwamp(XPos, YPos, Strength);
	}
}

void CBoard::SetRiver(USHORT SBridge)
{
	if (SBridge > FILL_MAX_STRENGTH)
		SBridge = FILL_MAX_STRENGTH;

	SBridge = (USHORT)((ULONG)FILL_MAX_STRENGTH - (ULONG)SBridge);

	SBridge += FILL_MIN_STRENGTH;

	SHORT XPos, YPos;
	SHORT Direction = (SHORT)(Random(3) - 1);

	if (Random(3) < 2)
	{
		YPos = 0;
		XPos = (SHORT)(Random(m_XSize - 2) + 1);

		while (YPos + 1 < m_YSize)
		{
			if (Random(2) == 0)
				Direction = (SHORT)(Random(3) - 1);

			(GetPart(XPos, YPos)).SetRiver();
			if (XPos + (Direction * 2) < 0)
				Direction = 0;
			if (XPos + (Direction * 2) >= m_XSize)
				Direction = 0;

			if (YPos + 1 < m_YSize)
			{
				(GetPart((SHORT)((LONG)XPos + Direction), (SHORT)((LONG)YPos + 1))).SetRiver();
				(GetPart(XPos, (SHORT)((LONG)YPos + 1))).SetRiver();
				if ((Direction == 0)&&(Random(SBridge) < FILL_MAP_STRENGTH))
				{
					(GetPart((SHORT)((LONG)XPos + Direction), (SHORT)((LONG)YPos + 1))).SetBridge();
				}
			}

			XPos += Direction;
			++YPos;
		}
	}
	else
	{
		XPos = 0;
		YPos = (SHORT)((LONG)Random(m_YSize - 2) + 1);

		while (XPos + 1 < m_XSize)
		{
			if (Random(2) == 0)
				Direction = (SHORT)((LONG)Random(3) - 1);

			if (YPos + (Direction * 2) < 0)
				Direction = 0;
			if (YPos + (Direction * 2) >= m_YSize)
				Direction = 0;

			(GetPart(XPos, YPos)).SetRiver();
			if (XPos + 1 < m_XSize)
			{
				(GetPart((SHORT)((LONG)XPos + 1), YPos)).SetRiver();
				(GetPart((SHORT)((LONG)XPos + 1), (SHORT)((LONG)YPos + Direction))).SetRiver();
				if ((Direction == 0)&&(Random(SBridge) < FILL_MAP_STRENGTH))
				{
					(GetPart((SHORT)((LONG)XPos + 1), (SHORT)((LONG)YPos + Direction))).SetBridge();
				}
			}

			++XPos;
			YPos += Direction;
		}
	}
}

void CBoard::GenerateMap(USHORT Total, USHORT SLightForest, USHORT SDenseForest, USHORT SSand, USHORT SSwamp)
{
	if (SLightForest > FILL_MAX_STRENGTH)
		SLightForest = FILL_MAX_STRENGTH;
	if (SDenseForest > FILL_MAX_STRENGTH)
		SDenseForest = FILL_MAX_STRENGTH;
	if (SSand > FILL_MAX_STRENGTH)
		SSand = FILL_MAX_STRENGTH;
	if (SSwamp > FILL_MAX_STRENGTH)
		SSwamp = FILL_MAX_STRENGTH;

	SLightForest = (USHORT)((ULONG)FILL_MAX_STRENGTH - (ULONG)SLightForest);
	SDenseForest = (USHORT)((ULONG)FILL_MAX_STRENGTH - (ULONG)SDenseForest);
	SSand = (USHORT)((ULONG)FILL_MAX_STRENGTH - (ULONG)SSand);
	SSwamp = (USHORT)((ULONG)FILL_MAX_STRENGTH - (ULONG)SSwamp);

	SLightForest += FILL_MIN_STRENGTH;
	SDenseForest += FILL_MIN_STRENGTH;
	SSand += FILL_MIN_STRENGTH;
	SSwamp += FILL_MIN_STRENGTH;

	USHORT Loop;

	for (Loop = 0; Loop < Total; ++Loop)
	{
		switch(Random(4))
		{
			case 0:
				SetLightForest(1, SLightForest);
				break;
			case 1:
				SetDenseForest(1, SDenseForest);
				break;
			case 2:
				SetSand(1, SSand);
				break;
			default:
				SetSwamp(1, SSwamp);
				break;
		}
	}
}

void CBoard::AddVillages(USHORT Total)
{
	USHORT ActualTotal;

	ActualTotal = Total;
	USHORT Loop;
	for (Loop = 0; Loop < Total; ++Loop)
	{
		SHORT XPos, YPos;

		XPos = (SHORT)Random(m_XSize);
		YPos = (SHORT)Random(m_YSize);

		USHORT GetOut = 0;
		while((GetPart(XPos, YPos).IsRiver()) && (GetOut > 50))
		{
			XPos = (SHORT)Random(m_XSize);
			YPos = (SHORT)Random(m_YSize);
			++GetOut;
		}
		if (GetPart(XPos, YPos).IsVillage())
			--ActualTotal;
		else
		{
			if (GetPart(XPos, YPos).IsRiver())
				--ActualTotal;
			else
				GetPart(XPos, YPos).SetVillage();
		}
	}
	m_TotalVillages = ActualTotal;
}

BOOL CBoard::FindGoodLocation(SHORT &XPos, SHORT &YPos, CFighter &Fighter, CBindMove &Bind)
{
	USHORT Tries;

	Tries = 0;
	do
	{
		YPos = (SHORT)Random(m_YSize);

		if (Fighter.GetControl() == 1)
			XPos = (SHORT)Random(3);
		else
			XPos = (SHORT)(((LONG)Random(3)) + ((LONG)m_XSize - 3));
		if (!(GetPart(XPos, YPos).IsVillage()))
		{
			if (!(GetPart(XPos, YPos).HasFighter()))
			{
				if (!(GetPart(XPos, YPos).IsRiver()))
				{
					Fighter.SetXPos(XPos);
					Fighter.SetYPos(YPos);
					Fighter.SetDestX(XPos);  //CHANGE BACK
					Fighter.SetDestY(YPos);
//					Fighter.SetDestX(Random(m_XSize));
//					Fighter.SetDestY(Random(m_YSize));
					Fighter.SetAttached(NULL);
					GetPart(XPos, YPos).SetFighter(&Fighter);

					Bind.Add(XPos, YPos);

					return TRUE;
				}
			}
		}
		++Tries;
	} while(Tries < TOTAL_PLACEMENT_RETRIES);
	return FALSE;
}

BOOL CBoard::AddFighter(CFighter &Fighter, CBindMove &Bind)
{
	SHORT XPos, YPos;

	if(FindGoodLocation(XPos, YPos, Fighter, Bind))
	{
		CPointer Pointer;

		Pointer.SetValue(&Fighter);
		m_Fighters.AddTail(&Pointer);
		return TRUE;
	}
	else
	{
		CFighter *Temp;
		Temp = &Fighter;
		ObjectDelete((BINARY &)Temp);
	}
	return FALSE;
}

USHORT DatX1Dir[3][3] = {
{0,-1,-1},
{1,0,-1},
{1,1,0}
};

USHORT DatY1Dir[3][3] = {
{-1,-1,0},
{-1,0,1},
{0,1,-1}
};

USHORT DatX2Dir[3][3] = {
{-1,-1,0},
{-1,0,1},
{0,1,1}
};

USHORT DatY2Dir[3][3] = {
{0,1,1},
{-1,0,1},
{-1,-1,0}
};

void CBoard::GoAround(SHORT &XPos, SHORT &YPos, SHORT DirX, SHORT DirY, BOOL &OkayMove, CFighter *Fighter, CBindMove &Bind)
{
	XPos = Fighter->GetXPos();
	YPos = Fighter->GetYPos();

	XPos += DatX1Dir[DirX + 1][DirY + 1];
	YPos += DatY1Dir[DirX + 1][DirY + 1];

	if ((GetPart(XPos, YPos).HasFighter()) || (OutOfBounds(XPos, YPos)))
	{
		XPos = Fighter->GetXPos();
		YPos = Fighter->GetYPos();

		XPos += DatX2Dir[DirX + 1][DirY + 1];
		YPos += DatY2Dir[DirX + 1][DirY + 1];
		if ((GetPart(XPos, YPos).HasFighter()) || (OutOfBounds(XPos, YPos)))
		{
			OkayMove = FALSE;
			Fighter->FlushMoves();
			if ((Fighter->GetPiece() == BITID_LAUNCH1)||(Fighter->GetPiece() == BITID_LAUNCH2))
			{
				Fighter->SetDestroyed();
				// MOVING ITEM OFF PAGE
				GetPart(Fighter->GetXPos(), Fighter->GetYPos()).SetFighter(NULL);
				Bind.Destroy(Fighter->GetXPos(), Fighter->GetYPos());
				// END OF MOVING
			}
		}
	}
}

void CBoard::Choose(USHORT Player)
{
	POSITION Pos;

	CDATA Pointer;

	Pointer = m_Fighters.GetHeadPosition(Pos);

	while (Pos != NULL)
	{
		CFighter *Fighter;

		Fighter = (CFighter *)((CPointer *)Pointer)->GetValue();
		if (Fighter->GetControl() == Player)
		{
			BOOL NewDest = FALSE;

			if (Fighter->SmartDecide(*this))
			{
				NewDest = TRUE;
				BOOL Done = FALSE;

				USHORT Loop = 0;

				SHORT XPos, YPos;
				while ((!Done)&&(Loop < 60))
				{

					XPos = (SHORT)Random(m_XSize);
					YPos = (SHORT)Random(m_YSize);

					if (GetPart(XPos, YPos).HasFighter())  // HERE WEAPONS
					{
						if (GetWhich(Fighter->GetPiece()) == 8)
						{
							if ((GetPart(XPos, YPos).GetFighter())->GetControl() == Player)
							{
								Fighter->SetAttached(GetPart(XPos, YPos).GetFighter());
								Fighter->UpdateLink();
								Done = !(Fighter->SmartDecide(*this));
							}
						}
						else
						{
							if ((GetPart(XPos, YPos).GetFighter())->GetControl() != Player)
							{
								Fighter->SetAttached(GetPart(XPos, YPos).GetFighter());
								Fighter->UpdateLink();
								Done = !(Fighter->SmartDecide(*this));
							}
						}
					}
					else
					{
						if (GetPart(XPos, YPos).IsVillage())
						{
							Fighter->SetDestX(XPos);
							Fighter->SetDestY(YPos);
							Done = !(Fighter->SmartDecide(*this));
						}
					}
					if (!Done)
						++Loop;
				}
				if (Loop >= 60)
				{
					if (Random(20) == 0)
					{
						Fighter->SetDestX(XPos);
						Fighter->SetDestY(YPos);
					}
				}
			}
		}
		Pointer = m_Fighters.GetNext(Pos);
	}
}

void CBoard::Move(USHORT Player, CBindMove &Bind)
{
	if (Player == 1)
		++m_TurnsP1;
	else
		++m_TurnsP2;

	POSITION Pos;

	CDATA Pointer;

	Pointer = m_Fighters.GetHeadPosition(Pos);

	while (Pos != NULL)
	{
		CFighter *Fighter;

		Fighter = (CFighter *)((CPointer *)Pointer)->GetValue();
		if (Fighter->GetControl() == Player)
		{
			if ((Fighter->GetPiece() == BITID_MISSILE1) || (Fighter->GetPiece() == BITID_MISSILE2))
			{
				if (!(Fighter->Still()))
				{
					SHORT Direction = 0;
					if (Player == 1)
						Direction = 1;
					else
						Direction = -1;

					if (!(GetPart((SHORT)((LONG)Fighter->GetXPos() + Direction), Fighter->GetYPos()).HasFighter()))
					{
						CFighter *NewFighter;
						NewFighter = (CFighter *)ObjectNew(CFighter);
						NewFighter->SetPlane();
						if (Player == 1)
							NewFighter->SetPiece(BITID_LAUNCH1);
						else
							NewFighter->SetPiece(BITID_LAUNCH2);

						NewFighter->SetAttack(Fighter->GetAttack());
						NewFighter->SetDefense(0);

						NewFighter->SetAttached(Fighter->GetAttached());
						NewFighter->SetDestX(Fighter->GetDestX());
						NewFighter->SetDestY(Fighter->GetDestY());

						NewFighter->SetControl(Player);

						// MOVING ITEM ON PAGE
						NewFighter->SetXPos((SHORT)((LONG)Fighter->GetXPos() + Direction));
						NewFighter->SetYPos(Fighter->GetYPos());

						GetPart((SHORT)((LONG)Fighter->GetXPos() + Direction), Fighter->GetYPos()).SetFighter(NewFighter);
						Bind.Move((SHORT)((LONG)Fighter->GetXPos() + (LONG)Direction), (SHORT)Fighter->GetYPos());
						// END OF MOVING

						CPointer NewPointer;
						NewPointer.SetValue(NewFighter);

						m_Fighters.AddTail(&NewPointer);
					}
				}
			}
		}
		Pointer = m_Fighters.GetNext(Pos);
	}

	Pointer = m_Fighters.GetHeadPosition(Pos);
	while (Pos != NULL)
	{
		CFighter *Fighter;

		Fighter = (CFighter *)((CPointer *)Pointer)->GetValue();
		if (Fighter->GetControl() == Player)
		{
			if ((!(Fighter->Still()))&&(!(Fighter->IsDestroyed())))
			{
				ClearAllTraces();
				BOOL StillMoving = TRUE;

				Fighter->AddMoves();
				Fighter->UpdateLink();
				while(StillMoving)
				{
					SHORT DirX, DirY;
					SHORT XPos, YPos;

					DirX = 0;
					DirY = 0;

					XPos = Fighter->GetXPos();
					YPos = Fighter->GetYPos();

					if (XPos < Fighter->GetDestX())
						DirX = 1;
					if (XPos > Fighter->GetDestX())
						DirX = -1;

					if (YPos < Fighter->GetDestY())
						DirY = 1;
					if (YPos > Fighter->GetDestY())
						DirY = -1;

					XPos += DirX;
					YPos += DirY;

					if (GetPart(XPos, YPos).IsOpenField())
						StillMoving = Fighter->MoveGround();
					if (GetPart(XPos, YPos).IsLightForest())
						StillMoving = Fighter->MoveLightForest();
					if (GetPart(XPos, YPos).IsDenseForest())
						StillMoving = Fighter->MoveDenseForest();
					if (GetPart(XPos, YPos).IsSand())
						StillMoving = Fighter->MoveSand();
					if (GetPart(XPos, YPos).IsSwamp())
						StillMoving = Fighter->MoveSwamp();
					if (GetPart(XPos, YPos).IsRiver())
					{
						if (GetPart(XPos, YPos).HasBridge())
							StillMoving = Fighter->MoveGround();
						else
							StillMoving = Fighter->MoveWater();
					}
					if (GetPart(XPos, YPos).IsVillage())
						StillMoving = Fighter->MoveLightForest();

					if (StillMoving)
					{
						BOOL OkayMove = TRUE;
						if (GetPart(XPos, YPos).HasFighter()) // HERE WEAPONS
						{
							if (GetPart(XPos, YPos).GetFighter() == Fighter->GetAttached())
							{
								if (GetWhich(Fighter->GetPiece()) == 8)
								{
									(GetPart(XPos, YPos).GetFighter())->Repair();
									Fighter->SetStill();
									OkayMove = FALSE;
								}
								else
								{
									Fighter->Fight(*GetPart(XPos, YPos).GetFighter());
									if (Fighter->IsDestroyed())
									{
										OkayMove = FALSE;
										// MOVING PART NOW - DESTROYED
										GetPart(Fighter->GetXPos(), Fighter->GetYPos()).SetFighter(NULL);
										Bind.Destroy(Fighter->GetXPos(), Fighter->GetYPos());
										// END MOVING
									}
									if (GetPart(XPos, YPos).GetFighter()->IsDestroyed())
									{
										if (Player == 1)
											++m_DestroyedP1;
										else
											++m_DestroyedP2;
										// MOVING PART NOW - DESTROYED
										GetPart(XPos,YPos).SetFighter(NULL);
										Bind.Destroy(XPos, YPos);
										// END MOVING
									}
								}
							}
							else
							{
								if ((XPos == Fighter->GetDestX()) && (YPos == Fighter->GetDestY()))
								{
									if (GetWhich(Fighter->GetPiece()) == 8)
									{
										(GetPart(XPos, YPos).GetFighter())->Repair();
										Fighter->SetStill();
										OkayMove = FALSE;
									}
									else
									{
										if ((GetPart(XPos, YPos).GetFighter())->GetControl() != Player)
										{
											Fighter->Fight(*GetPart(XPos, YPos).GetFighter());
											if (Fighter->IsDestroyed())
											{
												OkayMove = FALSE;
												// MOVING PART NOW - DESTROYED
												GetPart(Fighter->GetXPos(), Fighter->GetYPos()).SetFighter(NULL);
												Bind.Destroy(Fighter->GetXPos(), Fighter->GetYPos());
												// END MOVING
											}
											if (GetPart(XPos, YPos).GetFighter()->IsDestroyed())
											{
												// MOVING PART NOW - DESTROYED
												GetPart(XPos,YPos).SetFighter(NULL);
												Bind.Destroy(XPos, YPos);
												// END MOVING
												if (Player == 1)
													++m_DestroyedP1;
												else
													++m_DestroyedP2;
											}
										}
										else
										{
											OkayMove = FALSE;
											if ((Fighter->GetPiece() == BITID_LAUNCH1)||(Fighter->GetPiece() == BITID_LAUNCH2))
											{
												Fighter->SetDestroyed();
												// MOVING ITEM OFF PAGE
												GetPart(Fighter->GetXPos(), Fighter->GetYPos()).SetFighter(NULL);
												Bind.Destroy(Fighter->GetXPos(), Fighter->GetYPos());
												// END OF MOVING
											}
										}
									}
								}
								else
								{  // FIGHTER MUST GO AROUND POSITION
									GoAround(XPos, YPos, DirX, DirY, OkayMove, Fighter, Bind);
								}
							}
						}
						if (OkayMove)
						{
							if ((GetPart(XPos, YPos)).GetTrace() == Fighter)
							{
								OkayMove = FALSE;
								StillMoving = FALSE;
							}
							if ((GetPart(XPos, YPos).IsVillage())&&(!(Fighter->IsPlane())))
							{
								if (GetPart(XPos, YPos).WhoControls() != Player)
								{
									if (GetPart(XPos, YPos).WhoControls() == 1)
									{
										--m_VillagesP1;
										++m_VillagesP2;
										GetPart(XPos, YPos).SetControl(2);
									}
									else
									{
										if (GetPart(XPos, YPos).WhoControls() == 2)
										{
											--m_VillagesP2;
											++m_VillagesP1;
											GetPart(XPos, YPos).SetControl(1);
										}
										else
										{
											if (Player == 1)
											{
												GetPart(XPos, YPos).SetControl(1);
												++m_VillagesP1;
											}
											else
											{
												GetPart(XPos, YPos).SetControl(2);
												++m_VillagesP2;
											}
										}
									}
									// MOVING THE VILLAGE CONTROL
									if (Player == 1)
										GetPart(XPos, YPos).SetPiece(BITID_VILLAGE1);
									else
										GetPart(XPos, YPos).SetPiece(BITID_VILLAGE2);
									Bind.Move(XPos, YPos);
									// END MOVING
								}
							}
							// MOVING PART NOW
							GetPart(Fighter->GetXPos(), Fighter->GetYPos()).SetFighter(NULL);
							Bind.Move(Fighter->GetXPos(), Fighter->GetYPos());
							Fighter->SetXPos(XPos);
							Fighter->SetYPos(YPos);
							GetPart(Fighter->GetXPos(), Fighter->GetYPos()).SetFighter(Fighter);
							Bind.Move(XPos, YPos);
							// END MOVING
							GetPart(Fighter->GetXPos(), Fighter->GetYPos()).SetTrace(Fighter);
							if ((XPos == Fighter->GetDestX()) && (YPos == Fighter->GetDestY()))
							{
								OkayMove = FALSE;
								StillMoving = FALSE;
								Fighter->FlushMoves();
								if ((Fighter->GetPiece() == BITID_LAUNCH1)||(Fighter->GetPiece() == BITID_LAUNCH2))
								{
									Fighter->SetDestroyed();
									// MOVING ITEM OFF PAGE
									GetPart(Fighter->GetXPos(), Fighter->GetYPos()).SetFighter(NULL);
									Bind.Destroy(Fighter->GetXPos(), Fighter->GetYPos());
									// END OF MOVING
								}
							}
						}
						else
						{
							StillMoving = FALSE;
						}
					}
				}
			}
		}
		Pointer = m_Fighters.GetNext(Pos);
	}

	Pointer = m_Fighters.GetHeadPosition(Pos);
	POSITION OldPos;

	while (Pos != NULL)
	{
		CFighter *Fighter;

		Fighter = (CFighter *)((CPointer *)Pointer)->GetValue();
		OldPos = Pos;
		Pointer = m_Fighters.GetNext(Pos);
		if (Fighter->IsDestroyed())
		{
			CDATA Pointer2;
			POSITION Pos2;

			CFighter *Fighter2;

			Pointer2 = m_Fighters.GetHeadPosition(Pos2);
			while (Pos2 != NULL)
			{
				Fighter2 = (CFighter *)((CPointer *)Pointer2)->GetValue();
				if (Fighter2->GetAttached() == Fighter)
				{
					Fighter2->SetAttached(NULL);
				}
				Pointer2 = m_Fighters.GetNext(Pos2);
			}
			m_Fighters.RemoveAt(OldPos);
			ObjectCheck((BINARY)Fighter);
			ObjectDelete(Fighter);
		}
	}

	Pointer = m_Fighters.GetHeadPosition(Pos);

	while (Pos != NULL)
	{
		CFighter *Fighter;

		Fighter = (CFighter *)((CPointer *)Pointer)->GetValue();
		Fighter->UpdateLink();
		Pointer = m_Fighters.GetNext(Pos);
	}

}

CFighter *CBoard::FindNext(USHORT m_Player, CFighter *Last)
{
	CDATA Pointer;
	POSITION Pos;

	Pointer = m_Fighters.GetHeadPosition(Pos);
	BOOL NextOne = FALSE;

	while (Pos != NULL)
	{
		CFighter *Fighter;

		Fighter = (CFighter *)((CPointer *)Pointer)->GetValue();
		if (Last == Fighter)
			NextOne = TRUE;
		else
		{
			if ((Fighter->GetControl() == m_Player)&&(Fighter->SmartFindDecide(*this)))
			{
				if (Last == NULL)
					return Fighter;

				if (NextOne)
					return Fighter;
			}

		}
		Pointer = m_Fighters.GetNext(Pos);
	}
	return NULL;
}

USHORT GetWhich(ULONG Piece)
{
	switch(Piece)
	{
		case BITID_GROUNDF1:
		case BITID_GROUNDF2:
			return 0;
		case BITID_TANK1:
		case BITID_TANK2:
			return 1;
		case BITID_FTANK1:
		case BITID_FTANK2:
			return 2;
		case BITID_BOMBER1:
		case BITID_BOMBER2:
			return 3;
		case BITID_FIGHTER1:
		case BITID_FIGHTER2:
			return 4;
		case BITID_MISSILE1:
		case BITID_MISSILE2:
			return 5;
		case BITID_STANK1:
		case BITID_STANK2:
			return 6;
		case BITID_WATERF1:
		case BITID_WATERF2:  // HERE WEAPON
			return 7;
		case BITID_REPAIR1:
		case BITID_REPAIR2:
			return 8;
	}
	return 0;
}

ULONG GetWhich(ULONG Piece, USHORT Player)
{
	switch(Player)
	{
		case 1:
			switch(Piece)
			{
				case 0:
					return BITID_GROUNDF1;
				case 1:
					return BITID_TANK1;
				case 2:
					return BITID_FTANK1;
				case 3:
					return BITID_BOMBER1;
				case 4:
					return BITID_FIGHTER1;
				case 5:
					return BITID_MISSILE1;
				case 6:
					return BITID_STANK1;
				case 7:
					return BITID_WATERF1;  // HERE WEAPON
				case 8:
					return BITID_REPAIR1;
			}
			break;
		default:
			switch(Piece)
			{
				case 0:
					return BITID_GROUNDF2;
				case 1:
					return BITID_TANK2;
				case 2:
					return BITID_FTANK2;
				case 3:
					return BITID_BOMBER2;
				case 4:
					return BITID_FIGHTER2;
				case 5:
					return BITID_MISSILE2;
				case 6:
					return BITID_STANK2;
				case 7:
					return BITID_WATERF2;  // HERE WEAPON
				case 8:
					return BITID_REPAIR2;
			}
			break;
	}
	return 0;
}
 // HERE WEAPON

USHORT DatFighters[TOTALWEAPONS][11] =
{
{0,100,150,  50,55,75,60,100,150, 3, 0},
{0,500,400,  45,45,700,45,100,200, 8, 0},
{0,350,200,  25,30,600,25,200,250, 5, 0},
{1,1000,700,  0,0,0,0,0,0, 11, 0},
{1,500,400,   0,0,0,0,0,0, 9, 0},
{0,95,96,    200,200,1000,200,1000,1300, 10, 0},
{0,400,1000, 100,100,2000,10,200,300, 8, 0},
{0,300,300,  100,100,300,100,10,5, 4, 0},
{1,50,50,    0,0,0,0,0,0, 2, 0}
};

void CBuild::ResetWeapons()
{
	SetPerGroundForces(99);
	SetPerTank(0);
	SetPerFastTank(0);
	SetPerBomberPlane(0);
	SetPerFighterPlane(0);
	SetPerMissile(0);
	SetPerSandBuggy(0);
	SetPerWaterForces(0);
	SetPerRescueRepair(0);  // HERE WEAPONS
}

USHORT CBuild::MakeTotal(USHORT Value)
{
	if (Value == 0)
		return 0;
	if (Value * m_Total < 100)
		return 1;
	Value = (USHORT)(Value == 99 ? 100 : Value);
	return (USHORT)((Value * m_Total) / 100);
}

ULONG CBuild::GetNextBuild()
{
	BOOL Continue;
	ULONG Piece;

	Continue = TRUE;

	while (Continue)
	{
		switch (m_Position)
		{
			case 0:
				if (GetTotalGroundForces() > m_Round)
				{
					Continue = FALSE;
					Piece = GetWhich(GetWhich(BITID_GROUNDF1), m_Player);
				}
				break;
			case 1:
				if (GetTotalTank() > m_Round)
				{
					Continue = FALSE;
					Piece = GetWhich(GetWhich(BITID_TANK1), m_Player);
				}
				break;
			case 2:
				if (GetTotalFastTank() > m_Round)
				{
					Continue = FALSE;
					Piece = GetWhich(GetWhich(BITID_FTANK1), m_Player);
				}
				break;
			case 3:
				if (GetTotalBomberPlane() > m_Round)
				{
					Continue = FALSE;
					Piece = GetWhich(GetWhich(BITID_BOMBER1), m_Player);
				}
				break;
			case 4:
				if (GetTotalFighterPlane() > m_Round)
				{
					Continue = FALSE;
					Piece = GetWhich(GetWhich(BITID_FIGHTER1), m_Player);
				}
				break;
			case 5:
				if (GetTotalMissile() > m_Round)
				{
					Continue = FALSE;
					Piece = GetWhich(GetWhich(BITID_MISSILE1), m_Player);
				}
				break;
			case 6:
				if (GetTotalSandBuggy() > m_Round)
				{
					Continue = FALSE;
					Piece = GetWhich(GetWhich(BITID_STANK1), m_Player);
				}
				break;
			case 7:
				if (GetTotalWaterForces() > m_Round)
				{
					Continue = FALSE;
					Piece = GetWhich(GetWhich(BITID_WATERF1), m_Player);
				}
				break;
			case 8:
				if (GetTotalRescueRepair() > m_Round)
				{
					Continue = FALSE;
					Piece = GetWhich(GetWhich(BITID_REPAIR1), m_Player);  // HERE WEAPONS
				}
				break;
		}
		if ((m_Round == 0) && (Continue == FALSE))
			m_FoundZero = TRUE;

		++m_Position;
		if (m_Position > 8)
		{
			m_Position = 0;

			if ((m_Round == 0) && (m_FoundZero == FALSE))
			{
				Continue = FALSE;
				Piece = GetWhich(GetWhich(BITID_GROUNDF1), m_Player);
			}
			else
			{
				++m_Round;
				if (m_Round >= GetTotal())
				{
					m_Round = 0;
					m_FoundZero = FALSE;
				}
			}
		}
	}
	return Piece;
}

void CBuild::SetTotal(USHORT Total)
{
	Alloc((BINARY &)m_FighterClass,sizeof(ULONG)*Total);
	Alloc((BINARY &)m_Expected,sizeof(USHORT)*Total);
	Alloc((BINARY &)m_Invested,sizeof(USHORT)*Total);

	USHORT Loop1, Loop2;
	for (Loop1 = 0; Loop1 < TOTALWEAPONS; ++Loop1)
	{
		for (Loop2 = 0; Loop2 < 11; ++Loop2)
			m_DatFighters[Loop1][Loop2] = DatFighters[Loop1][Loop2];
	}

	m_Total = Total;

	USHORT Loop;

	for (Loop = 0; Loop < m_Total; ++Loop)
	{
		m_FighterClass[Loop] = 0;
		SetBuild(Loop, BITID_GROUNDF1, m_Player);
		m_Expected[Loop] = 1;
		m_Invested[Loop] = 1;
	}
}

CBuild::~CBuild()
{
	Free((BINARY &)m_FighterClass);
	Free((BINARY &)m_Expected);
	Free((BINARY &)m_Invested);
}

void CBuild::SetBuild(USHORT Resource, ULONG Piece)
{
	if (Resource >= m_Total)
		return;

	m_FighterClass[Resource] = Piece;
	m_Expected[Resource] = m_DatFighters[GetWhich(Piece)][9];
}

void CBuild::SetBuild(USHORT Resource, ULONG Piece, USHORT Player)
{
	SetBuild(Resource, GetWhich(GetWhich(Piece), Player));
}

ULONG CBuild::GetResource(USHORT Resource)
{
	if (Resource >= m_Total)
		return 0;
	return m_FighterClass[Resource];
}

USHORT CBuild::GetTotal(ULONG Piece)
{
	USHORT Total;
	USHORT Loop;

	Total = 0;
	for (Loop = 0; Loop < m_Total; ++Loop)
	{
		if (GetWhich(m_FighterClass[Loop]) == GetWhich(Piece))
		{
			++Total;
		}
	}
	return Total;
}

void CBuild::Rebalance()
{
	USHORT Loop;

	m_Position =  0;
	m_Round = 0;
	m_FoundZero = FALSE;
	for (Loop = 0; Loop < m_Total; ++Loop)
	{
		SetBuild(Loop, GetNextBuild());
	}
}

void CBuild::BuildFighters(CBoard &Board, CBindMove &Bind, BOOL Smart)
{
	USHORT Loop;

	for (Loop = 0; Loop < TOTALWEAPONS; ++Loop)
		m_DatFighters[Loop][10] = (USHORT)FALSE;

	for (Loop = 0; Loop < m_Total; ++Loop)
	{
		++m_Invested[Loop];
		if (m_Expected[Loop] <= m_Invested[Loop])
		{
			USHORT Which = GetWhich(m_FighterClass[Loop]);
			CFighter *Fighter;
			Fighter = (CFighter *)ObjectNew(CFighter);

			if (m_DatFighters[Which][0] == 0)
				Fighter->SetGroundFighter();
			else
				Fighter->SetPlane();

			Fighter->SetAttack(m_DatFighters[Which][1]);
			Fighter->SetDefense(m_DatFighters[Which][2]);
			Fighter->SetControl(m_Player);

			if (m_DatFighters[Which][1] > GetStrongest())
			{
				SetStrongest(m_DatFighters[Which][1]);
			}
			if (m_DatFighters[Which][2] > GetStrongest())
			{
				SetStrongest(m_DatFighters[Which][2]);
			}

			Fighter->SetSpeedGround(m_DatFighters[Which][3]);
			Fighter->SetSpeedLightForest(m_DatFighters[Which][4]);
			Fighter->SetSpeedDenseForest(m_DatFighters[Which][5]);
			Fighter->SetSpeedSand(m_DatFighters[Which][6]);
			Fighter->SetSpeedSwamp(m_DatFighters[Which][7]);
			Fighter->SetSpeedWater(m_DatFighters[Which][8]);

			Fighter->SetPiece(m_FighterClass[Loop]);

			Board.AddFighter(*Fighter, Bind);

			if (m_DatFighters[Which][10] == FALSE)
			{
				m_DatFighters[Which][10] = (USHORT)TRUE;
				m_DatFighters[Which][1] += Random(m_DatFighters[Which][1] / 10);
				if (m_DatFighters[Which][1] > 15000)
					m_DatFighters[Which][1] = 15000;

				m_DatFighters[Which][2] += Random(m_DatFighters[Which][2] / 10);
				if (m_DatFighters[Which][2] > 15000)
					m_DatFighters[Which][2] = 15000;
			}

			if (Smart)
			{
				SetBuild(Loop, GetWhich(Random(TOTALWEAPONS), m_Player));
				m_Invested[Loop] = 0;

			}
			else
			{
				SetBuild(Loop, GetNextBuild());
				m_Invested[Loop] = 0;
			}
		}
	}
}


