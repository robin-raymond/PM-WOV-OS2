#ifndef _BOARD_HPP_
#define _BOARD_HPP_

#include "bitmaps.h"

#define TOTAL_PALETTES 1

#define TOTAL_MOVES 50
#define FILL_MAP_STRENGTH 60
#define FILL_MIN_STRENGTH 100
#define FILL_MAX_STRENGTH 100

#define TOTAL_PLACEMENT_RETRIES 40
#define TOTALWEAPONS 9

void LoadBitmaps(HPS);
void DeleteBitmaps();
void DisplayBitmap(HPS, ULONG, LONG, LONG);
void DisplayStretchBitmap(HPS, ULONG, RECTL &);

USHORT GetStrongest();
void SetStrongest(USHORT);
USHORT GetWhich(ULONG);
ULONG GetWhich(ULONG, USHORT);

class CPart;
class CBoard;
class CFighter;
class CBuild;

class CBindMove {
	public:
		virtual void Add(SHORT, SHORT) = 0;
		virtual void Move(SHORT, SHORT) = 0;
		virtual void Destroy(SHORT, SHORT) = 0;
};

class CPart {
		BOOL m_River;
		BOOL m_LightForest;
		BOOL m_DenseForest;
		BOOL m_Village;
		BOOL m_Sand;
		BOOL m_Swamp;
		BOOL m_Bridge;

		USHORT m_Control;

		ULONG m_Piece;
		USHORT m_Palette;

		CFighter *m_Fighter;
		CFighter *m_Trace;

		void SetValues(BOOL, BOOL, BOOL, BOOL, BOOL, BOOL);
	public:
		CPart();
		BOOL IsOpenField();
		BOOL IsRiver() {return m_River;}
		BOOL IsForest() {return m_DenseForest || m_LightForest;}
		BOOL IsLightForest() {return m_LightForest;}
		BOOL IsDenseForest() {return m_DenseForest;}
		BOOL IsVillage() {return m_Village;}
		BOOL IsSand() {return m_Sand;}
		BOOL IsSwamp() {return m_Swamp;}
		BOOL HasBridge() {return m_Bridge;}
		USHORT WhoControls() {return m_Control;}
		BOOL HasRiverLR() {return m_Piece == BITID_RLR;}
		BOOL HasRiverTB() {return m_Piece == BITID_RTB;}
		BOOL HasBridgeLR() {return m_Piece == BITID_BLR;}
		BOOL HasBridgeTB() {return m_Piece == BITID_BTB;}
		BOOL HasFighter() {return m_Fighter != NULL;}

		USHORT GetPalette() {return m_Palette;}
		ULONG GetPiece() {return m_Piece;}
		void SetPiece(ULONG Piece) {m_Piece = Piece;}

		CFighter *GetFighter() {return m_Fighter;}
		void SetFighter(CFighter *Fighter) {m_Fighter = Fighter;}

		CFighter *GetTrace() {return m_Trace;}
		void SetTrace(CFighter *Fighter) {m_Trace = Fighter;}

		void SetOpenField() {SetValues(FALSE, FALSE, FALSE, FALSE, FALSE, FALSE);}
		void SetRiver() {SetValues(TRUE, FALSE, FALSE, FALSE, FALSE, FALSE);}
		void SetLightForest() {SetValues(FALSE, TRUE, FALSE, FALSE, FALSE, FALSE);}
		void SetDenseForest() {SetValues(FALSE, FALSE, TRUE, FALSE, FALSE, FALSE);}
		void SetVillage() {SetValues(FALSE, FALSE, FALSE, TRUE, FALSE, FALSE);}
		void SetSand() {SetValues(FALSE, FALSE, FALSE, FALSE, TRUE, FALSE);}
		void SetSwamp() {SetValues(FALSE, FALSE, FALSE, FALSE, FALSE, TRUE);}

		void SetBridge();
		void SetControl(USHORT);
		void SetPalette(USHORT);

		void ChoosePiece(CBoard &, SHORT, SHORT);
		ULONG SandPiece(BOOL, BOOL, BOOL, BOOL, BOOL, BOOL, BOOL, BOOL);
		ULONG RiverPiece(BOOL, BOOL, BOOL, BOOL, BOOL);
};

class CBoard {
		SHORT m_XSize;
		SHORT m_YSize;

		CPart ***m_Parts;
		CPart Null;
		CPart NullRiver;
		CPart NullSand;

		USHORT m_VillagesP1;
		USHORT m_VillagesP2;
		USHORT m_TotalVillages;

		CObList m_Fighters;

		LONG m_DestroyedP1;
		LONG m_DestroyedP2;
		LONG m_TurnsP1;
		LONG m_TurnsP2;

		void SetLightForest(SHORT, SHORT, USHORT);
		void SetDenseForest(SHORT, SHORT, USHORT);
		void SetSand(SHORT, SHORT, USHORT);
		void SetSwamp(SHORT, SHORT, USHORT);

		void GoAround(SHORT &, SHORT &, SHORT, SHORT, BOOL &, CFighter *, CBindMove &);
		BOOL FindGoodLocation(SHORT &, SHORT &, CFighter &, CBindMove &);
	public:
		CBoard();
		void SizeBoard(SHORT, SHORT);
		void FreeBoard();

		BOOL OutOfBounds(SHORT XPos, SHORT YPos) {return (XPos < 0) || (YPos < 0) || (XPos >= m_XSize) || (YPos >= m_YSize);}

		SHORT GetXSize() {return m_XSize;}
		SHORT GetYSize() {return m_YSize;}

		void SetLightForest(USHORT, USHORT);
		void SetDenseForest(USHORT, USHORT);
		void SetSand(USHORT, USHORT);
		void SetSwamp(USHORT, USHORT);
		void SetRiver(USHORT);

		void ClearAllTraces();

		void AddVillages(USHORT);
		void CalculateVillages();

		void DefinePieces();

		void GenerateMap(USHORT, USHORT, USHORT, USHORT, USHORT);
		BOOL AddFighter(CFighter &, CBindMove &);

		CPart &GetPart(SHORT, SHORT);
		CPart &GetRiverPart(SHORT, SHORT);
		CPart &GetSandPart(SHORT, SHORT);

		USHORT GetTotalVillages() {return m_TotalVillages;}
		USHORT GetVillagesP1() {return m_VillagesP1;}
		USHORT GetVillagesP2() {return m_VillagesP2;}
		LONG GetTurnsP1() {return m_TurnsP1;}
		LONG GetDestroyedP1() {return m_DestroyedP1;}
		LONG GetTurnsP2() {return m_TurnsP2;}
		LONG GetDestroyedP2() {return m_DestroyedP2;}

		BOOL HasWon() {return ((m_TotalVillages == m_VillagesP1)||(m_TotalVillages == m_VillagesP2));}
		BOOL HasWonP1() {return (m_TotalVillages == m_VillagesP1);}
		BOOL HasWonP2() {return (m_TotalVillages == m_VillagesP2);}

		void Move(USHORT, CBindMove &);
		void Choose(USHORT);
		CFighter *FindNext(USHORT, CFighter *);

		void DisplayPiece(SHORT, SHORT, HPS, LONG, LONG);
		void DisplayActionPiece(HPS, LONG, LONG, ULONG);

		~CBoard() {FreeBoard();}
};

class CFighter {
		USHORT m_Control;

		BOOL m_GroundFighter;
		BOOL m_Plane;

		USHORT m_Attack;
		USHORT m_Defense;

		USHORT m_ODefense;

		USHORT m_SpeedGround;
		USHORT m_SpeedLightForest;
		USHORT m_SpeedDenseForest;
		USHORT m_SpeedSand;
		USHORT m_SpeedSwamp;
		USHORT m_SpeedWater;

		SHORT m_DestX;
		SHORT m_DestY;

		SHORT m_XPos;
		SHORT m_YPos;

		CFighter *m_Attached;
		ULONG m_Piece;

		USHORT m_Moves;
		BOOL CheckMove(USHORT);

		BOOL m_Destroyed;
	public:
		CFighter();
		BOOL Still();
		void SetStill() {m_DestX = m_XPos; m_DestY = m_YPos; m_Attached = NULL;}

		SHORT GetXPos() {return m_XPos;}
		SHORT GetYPos() {return m_YPos;}

		SHORT GetDestX() {return m_DestX;}
		SHORT GetDestY() {return m_DestY;}

		USHORT GetAttack() {return m_Attack;}
		USHORT GetDefense() {return m_Defense;}

		USHORT GetControl() {return m_Control;}
		void SetControl(USHORT Control) {m_Control = Control;}

		void SetXPos(SHORT XPos) {m_XPos = XPos;}
		void SetYPos(SHORT YPos) {m_YPos = YPos;}

		void SetDestX(SHORT XPos) {m_DestX = XPos;}
		void SetDestY(SHORT YPos) {m_DestY = YPos;}

		void SetAttack(USHORT Attack) {m_Attack = Attack;}
		void SetDefense(USHORT Defense) {m_Defense = Defense; m_ODefense = Defense;}
		void Repair() {m_Defense = m_ODefense;}

		void AddMoves() {m_Moves += TOTAL_MOVES;}

		BOOL MoveGround() {return CheckMove(m_SpeedGround);}
		BOOL MoveLightForest() {return CheckMove(m_SpeedLightForest);}
		BOOL MoveDenseForest() {return CheckMove(m_SpeedDenseForest);}
		BOOL MoveSand() {return CheckMove(m_SpeedSand);}
		BOOL MoveSwamp() {return CheckMove(m_SpeedSwamp);}
		BOOL MoveWater() {return CheckMove(m_SpeedWater);}

		void SetSpeedGround(USHORT Speed) {m_SpeedGround = Speed;}
		void SetSpeedLightForest(USHORT Speed) {m_SpeedLightForest = Speed;}
		void SetSpeedDenseForest(USHORT Speed) {m_SpeedDenseForest = Speed;}
		void SetSpeedSand(USHORT Speed) {m_SpeedSand = Speed;}
		void SetSpeedSwamp(USHORT Speed) {m_SpeedSwamp = Speed;}
		void SetSpeedWater(USHORT Speed) {m_SpeedWater = Speed;}

		BOOL IsGroundFighter() {return m_GroundFighter;}
		BOOL IsPlane() {return m_Plane;}

		void SetGroundFighter() {m_Plane = FALSE; m_GroundFighter = TRUE;}
		void SetPlane() {m_Plane = TRUE; m_GroundFighter = FALSE;}

		BOOL IsDestroyed() {return m_Destroyed;}
		void SetDestroyed() {m_Destroyed = TRUE;}

		BOOL HasFighter() {return m_Attached != NULL;}
		BOOL SmartDecide(CBoard &);
		BOOL SmartFindDecide(CBoard &);

		CFighter *GetAttached() {return m_Attached;}
		void SetAttached(CFighter *Attached) {m_Attached = Attached; UpdateLink();}

		void Fight(CFighter &);

		void FlushMoves() {m_Moves = 0;}

		void UpdateLink();

		ULONG GetPiece() {return m_Piece;}
		void SetPiece(ULONG Piece) {m_Piece = Piece;}
};

class CBuild {
		ULONG *m_FighterClass;
		USHORT *m_Expected;
		USHORT *m_Invested;

		USHORT m_DatFighters[TOTALWEAPONS][11];
		USHORT m_Total;
		USHORT m_Player;

		USHORT m_Round;
		USHORT m_Position;
		USHORT m_FoundZero;

		USHORT m_PerGroundForces;
		USHORT m_PerTank;
		USHORT m_PerFastTank;
		USHORT m_PerBomberPlane;
		USHORT m_PerFighterPlane;
		USHORT m_PerMissile;
		USHORT m_PerSandBuggy;
		USHORT m_PerWaterForces;
		USHORT m_PerRescueRepair;  // HERE WEAPONS

		USHORT MakeTotal(USHORT);
		void ResetWeapons();
	public:
		CBuild() {m_FighterClass = NULL; m_Expected = NULL; m_Invested = NULL; m_Round = 0; m_Position = 0; ResetWeapons(); m_FoundZero = FALSE;}

		void SetPerGroundForces(USHORT Value) {m_PerGroundForces = Value;}
		void SetPerTank(USHORT Value) {m_PerTank = Value;}
		void SetPerFastTank(USHORT Value) {m_PerFastTank = Value;}
		void SetPerBomberPlane(USHORT Value) {m_PerBomberPlane = Value;}
		void SetPerFighterPlane(USHORT Value) {m_PerFighterPlane = Value;}
		void SetPerMissile(USHORT Value) {m_PerMissile = Value;}
		void SetPerSandBuggy(USHORT Value) {m_PerSandBuggy = Value;}
		void SetPerWaterForces(USHORT Value) {m_PerWaterForces = Value;}
		void SetPerRescueRepair(USHORT Value) {m_PerRescueRepair = Value;}  // HERE WEAPONS

		USHORT GetPerGroundForces() {return m_PerGroundForces;}
		USHORT GetPerTank() {return m_PerTank;}
		USHORT GetPerFastTank() {return m_PerFastTank;}
		USHORT GetPerBomberPlane() {return m_PerBomberPlane;}
		USHORT GetPerFighterPlane() {return m_PerFighterPlane;}
		USHORT GetPerMissile() {return m_PerMissile;}
		USHORT GetPerSandBuggy() {return m_PerSandBuggy;}
		USHORT GetPerWaterForces() {return m_PerWaterForces;}
		USHORT GetPerRescueRepair() {return m_PerRescueRepair;}  // HERE WEAPONS

		USHORT GetTotalGroundForces() {return MakeTotal(m_PerGroundForces);}
		USHORT GetTotalTank() {return MakeTotal(m_PerTank);}
		USHORT GetTotalFastTank() {return MakeTotal(m_PerFastTank);}
		USHORT GetTotalBomberPlane() {return MakeTotal(m_PerBomberPlane);}
		USHORT GetTotalFighterPlane() {return MakeTotal(m_PerFighterPlane);}
		USHORT GetTotalMissile() {return MakeTotal(m_PerMissile);}
		USHORT GetTotalSandBuggy() {return MakeTotal(m_PerSandBuggy);}
		USHORT GetTotalWaterForces() {return MakeTotal(m_PerWaterForces);}
		USHORT GetTotalRescueRepair() {return MakeTotal(m_PerRescueRepair);}  // HERE WEAPONS

		void Rebalance();
		ULONG GetNextBuild();

		void SetTotal(USHORT);
		USHORT GetTotal() {return m_Total;}
		USHORT GetTotal(ULONG);
		USHORT GetPlayer() {return m_Player;}
		void SetPlayer(USHORT Player) {m_Player = Player;}

		ULONG GetResource(USHORT);

		void SetBuild(USHORT, ULONG);
		void SetBuild(USHORT, ULONG, USHORT);

		void BuildFighters(CBoard &, CBindMove &, BOOL);
		~CBuild();
};
#endif