#define _COMMON_CWINDOW_H_
#define _COMMON_NFILE_H_
#include "common.h"

#include "hiscore.hpp"

#define TOTAL_HIGHS 7

char *g_HighFile = "HALLFAME.SCR";

class CHighScore {
		LONG m_Scores[TOTAL_HIGHS];
		char m_Initials[TOTAL_HIGHS][4];

		void Load();
		void Save();
	public:
		CHighScore();
		USHORT AddRank(LONG, PSZ);
		BOOL InRank(LONG);

		LONG GetPoints(USHORT);
		PSZ GetInitials(USHORT);
};

CHighScore g_HighScore;

USHORT AddRank(LONG Value, PSZ Initials)
{
	return g_HighScore.AddRank(Value, Initials);
}

BOOL InRank(LONG Value)
{
	return g_HighScore.InRank(Value);
}

LONG GetPoints(USHORT Index)
{
	return g_HighScore.GetPoints(Index);
}

PSZ GetInitials(USHORT Index)
{
	return g_HighScore.GetInitials(Index);
}

CHighScore::CHighScore()
{
	USHORT Loop;

	for(Loop = 0; Loop < TOTAL_HIGHS; ++Loop)
	{
		m_Scores[Loop] = 0;
		m_Initials[Loop][0] = 'B';
		m_Initials[Loop][1] = 'I';
		m_Initials[Loop][2] = 'G';
		m_Initials[Loop][3] = 0;
	}
	Load();
}

LONG CHighScore::GetPoints(USHORT Index)
{
	return m_Scores[Index];
}

PSZ CHighScore::GetInitials(USHORT Index)
{
	return m_Initials[Index];
}

USHORT CHighScore::AddRank(LONG Value, PSZ Initials)
{
	Load();
	SHORT Loop, MLoop;

	for(Loop = 0; Loop < TOTAL_HIGHS; ++Loop)
	{
		if (Value > m_Scores[Loop])
		{
			for(MLoop = TOTAL_HIGHS - 2; MLoop >= Loop; --MLoop)
			{
				m_Scores[MLoop + 1] = m_Scores[MLoop];
				m_Initials[MLoop + 1][0] = m_Initials[MLoop][0];
				m_Initials[MLoop + 1][1] = m_Initials[MLoop][1];
				m_Initials[MLoop + 1][2] = m_Initials[MLoop][2];
				m_Initials[MLoop + 1][3] = m_Initials[MLoop][3];
			}
			m_Scores[Loop] = Value;
			m_Initials[Loop][0] = Initials[0];
			m_Initials[Loop][1] = Initials[1];
			m_Initials[Loop][2] = Initials[2];
			m_Initials[Loop][3] = Initials[3];
			Save();
			return Loop;
		}
	}
	return TOTAL_HIGHS;
}

BOOL CHighScore::InRank(LONG Value)
{
	Load();
	SHORT Loop;

	for(Loop = 0; Loop < TOTAL_HIGHS; ++Loop)
	{
		if (Value > m_Scores[Loop])
			return TRUE;
	}
	return FALSE;
}

void CHighScore::Load()
{
	NFILE HighFile;

	if (HighFile.OpenBlocking(g_HighFile, NFILE_READ | NFILE_BLOCKWRITE) == FERROR_NO_ERROR)
	{
		USHORT Loop;

		for(Loop = 0; Loop < TOTAL_HIGHS; ++Loop)
		{
			HighFile >> (m_Scores[Loop]);
			HighFile >> (m_Initials[Loop][0]);
			HighFile >> (m_Initials[Loop][1]);
			HighFile >> (m_Initials[Loop][2]);
			HighFile >> (m_Initials[Loop][3]);
		}

		HighFile.Close();
	}
}

void CHighScore::Save()
{
	NFile HighFile;

	if (HighFile.OpenBlocking(g_HighFile, NFILE_WRITE | NFILE_BLOCKREADWRITE | NFILE_CREATE) == FERROR_NO_ERROR)
	{
		USHORT Loop;

		for(Loop = 0; Loop < TOTAL_HIGHS; ++Loop)
		{
			HighFile << (m_Scores[Loop]);
			HighFile << (m_Initials[Loop][0]);
			HighFile << (m_Initials[Loop][1]);
			HighFile << (m_Initials[Loop][2]);
			HighFile << (m_Initials[Loop][3]);
		}

		HighFile.Close();
	}
}


