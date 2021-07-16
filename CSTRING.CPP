#include "stdtypes.h"
#include "memory.h"
#include "cstring.h"
#include "citem.h"

// Last checked Dec 6, 1995

STRING _str_test_ = "";
BOOL _case_sensitive_ = TRUE;

CHAR _UPPERLETTERS[256] = {
0,1,2,3,4,5,6,7,8,9,
10,11,12,13,14,15,16,17,18,19,
20,21,22,23,24,25,26,27,28,29,
30,31,32,33,34,35,36,37,38,39,
40,41,42,43,44,45,46,47,48,49,
50,51,52,53,54,55,56,57,58,59,
60,61,62,63,64,65,66,67,68,69,
70,71,72,73,74,75,76,77,78,79,
80,81,82,83,84,85,86,87,88,89,
90,91,92,93,94,95,96,65,66,67,
68,69,70,71,72,73,74,75,76,77,
78,79,80,81,82,83,84,85,86,87,
88,89,90,123,124,125,126,127,128,154,
144,131,142,133,143,128,136,137,138,139,
140,141,142,143,144,146,146,147,153,149,
150,151,152,153,154,155,156,157,158,159,
160,161,162,163,164,166,166,167,168,169,
170,171,172,173,174,175,176,177,178,179,
180,181,182,183,184,185,186,187,188,189,
190,191,192,193,194,195,196,197,198,199,
200,201,202,203,204,205,206,207,208,209,
210,211,212,213,214,215,216,217,218,219,
220,221,222,223,224,225,226,227,228,229,
230,231,232,233,234,235,236,237,238,239,
240,241,242,243,244,245,246,247,248,249,
250,251,252,253,254,255
};

CHAR _LOWERLETTERS[256] = {
0,1,2,3,4,5,6,7,8,9,
10,11,12,13,14,15,16,17,18,19,
20,21,22,23,24,25,26,27,28,29,
30,31,32,33,34,35,36,37,38,39,
40,41,42,43,44,45,46,47,48,49,
50,51,52,53,54,55,56,57,58,59,
60,61,62,63,64,97,98,99,100,101,
102,103,104,105,106,107,108,109,110,111,
112,113,114,115,116,117,118,119,120,121,
122,91,92,93,94,95,96,97,98,99,
100,101,102,103,104,105,106,107,108,109,
110,111,112,113,114,115,116,117,118,119,
120,121,122,123,124,125,126,127,135,129,
130,131,132,133,134,135,136,137,138,139,
140,141,132,134,130,145,145,147,148,149,
150,151,152,148,129,155,156,157,158,159,
160,161,162,163,164,164,166,167,168,169,
170,171,172,173,174,175,176,177,178,179,
180,181,182,183,184,185,186,187,188,189,
190,191,192,193,194,195,196,197,198,199,
200,201,202,203,204,205,206,207,208,209,
210,211,212,213,214,215,216,217,218,219,
220,221,222,223,224,225,226,227,228,229,
230,231,232,233,234,235,236,237,238,239,
240,241,242,243,244,245,246,247,248,249,
250,251,252,253,254,255
};

CHAR _COMPARELETTERS[256] = {
0,1,2,3,4,5,6,7,8,9,
10,11,12,13,14,15,16,17,18,19, // 10
20,21,22,23,24,25,26,27,28,29, // 20
30,31,32,33,34,35,36,37,38,39, // 30
40,41,42,43,44,45,46,47,48,49, // 40
50,51,52,53,54,55,56,57,58,59, // 50
60,61,62,63,64,65,69,70,72,73, // 60
75,76,77,78,79,80,81,82,83,85, // 70
87,88,89,90,91,92,94,95,96,97, // 80
98,99,100,101,102,103,104,105,112,113, // 90
115,116,121,122,123,124,129,130,131,132, //100
133,135,140,141,142,145,144,145,150,151, //110
152,153,155,156,157,158,159,160,71,146, //120
117,106,107,108,109,114,118,119,120,125, //130
126,127,66,67,74,110,68,136,137,138, //140
147,148,154,86,93,161,162,163,164,165, //150
111,128,139,149,134,84,166,167,168,169, //160
170,171,172,173,174,175,176,177,178,179, //170
180,181,182,183,184,185,186,187,188,189, //180
190,191,192,193,194,195,196,197,198,199, //190
200,201,202,203,204,205,206,207,208,209, //200
210,211,212,213,214,215,216,217,218,219, //210
220,221,222,223,224,225,226,227,228,229, //220
230,231,232,233,234,235,236,237,238,239, //230
240,241,242,243,244,245,246,247,248,249, //240
250,251,252,253,254,255                  //250
};

void CaseSensitive(BOOL Answer)
{
	_case_sensitive_ = Answer;
}

BOOL CaseSensitive()
{
	return _case_sensitive_;
}


WORD GetCount(const STRING String)
{
	WORD Count;

	STRING NewString = String;
	for (Count = 0; *NewString; ++NewString, ++Count);
	return Count;
}

DEFINECITEM(CString)
COPYCITEM(CString)
MATCHCITEM(CString)

CString::~CString()
{
	Clear();
}

void CString::Upload(const STRING String)
{
	m_String = String;
	m_Size = GetCount(String);
	m_AllocSize = m_Size;
}

void CString::Flush()
{
	ResetPointer((BINARY)m_String);
	m_Size = 0;
	m_AllocSize = 0;
}

CString &CString::Trunc(WORD Size)
{
	if (Size == 0)
	{
		if (m_AllocSize > 0)
		{
			Free((BINARY)m_String);
			m_Size = 0;
			m_AllocSize = 0;
		}
	}
	else
	{
		if (Size < m_AllocSize)
		{
			*(m_String + Size) = 0;
			if (Size < m_Size)
				m_Size = Size;
		}
	}
	return *this;
// returns this object
}

ULONG CString::StringToULONG()
{
// this function converts a string to an unsigned long
	if (m_AllocSize == 0)
		return 0;

	STRING TmpString;
	ULONG Number;

	for (TmpString = m_String, Number = 0; *TmpString; ++TmpString)
	{
		if ((CBYTE(*TmpString) >= CBYTE('0')) && (CBYTE(*TmpString) <= CBYTE('9')))
		{
			Number *= 10;
			Number += CBYTE(*TmpString) - CBYTE('0');
		}
	}
	return Number;
// returns unsigned long for the number the string represents
}

SLONG CString::StringToSLONG()
{
// this function converts a string to a signed long
	if (m_AllocSize == 0)
		return 0;

	STRING TmpString;
	SLONG Number;
	BOOL WasNegative = FALSE;

	for (TmpString = m_String, Number = 0; *TmpString; ++TmpString)
	{
		if((*TmpString) == '-')
		{
			if (WasNegative)
				WasNegative = FALSE;
			else
				WasNegative = TRUE;
		}
		else
		{
			if((CBYTE(*TmpString) >= CBYTE('0'))&&(CBYTE(*TmpString) <= CBYTE('9')))
			{
				Number *= 10;
				Number += CBYTE(*TmpString) - CBYTE('0');
			}
		}
	}
	if (WasNegative)
		return Number * -1;
	return Number;
}

void CString::NewAllocSize(WORD Size)
{
// the new size passed should exclude the null pointer
	if (Size == 0)
	{
		if (m_AllocSize > 0)
		{
			Free((BINARY)m_String);
			m_AllocSize = 0;
			m_Size = 0;
		}
	}
	if (Size <= m_AllocSize)
		return;
	Alloc((BINARY)m_String,Size+1);

	*(m_String + Size) = 0;
	*(m_String + m_Size) = 0;
	m_AllocSize = Size;
}

void CString::ULONGToString(ULONG Number)
{
// this function converts unsigned long to this string
	CHAR TmpString[40];

	WORD Position = 0;
	if (Number > 0)
	{
		while (Number > 0)
		{
			TmpString[Position] = CCHAR(CBYTE('0') + (Number % 10));
			Number /= 10;
			++Position;
		}
	}
	else
	{
		++Position;
		TmpString[0] = '0';
	}

	NewAllocSize(Position);

	STRING Dest = m_String;
	STRING Sour = TmpString + Position - 1;

	m_Size = Position;

	for(; Position > 0; --Position, --Sour, ++Dest)
	{
		*Dest = *Sour;
	}
	*Dest = 0;
}

void CString::SLONGToString(SLONG Number)
{
// this function converts a signed long to this string
	CHAR TmpString[40];
	BYTE Position = 0;

	BOOL WasNegative = FALSE;

	if (Number < 0)
	{
		Number *= -1;
		WasNegative = TRUE;
	}
	if (Number > 0)
	{
		while (Number > 0)
		{
			TmpString[Position] = CCHAR(CBYTE('0') + (Number % 10));
			Number /= 10;
			++Position;
		}
	}
	else
	{
		TmpString[0] = '0';
		++Position;
	}
	if (WasNegative == TRUE)
	{
		TmpString[Position] = '-';
		++Position;
	}

	NewAllocSize(Position);

	STRING Dest = m_String;
	STRING Sour = TmpString + Position - 1;

	m_Size = Position;

	for(; Position > 0; --Position, --Sour, ++Dest)
	{
		*Dest = *Sour;
	}
	*Dest = 0;
}

CString &CString::DeleteLetter(WORD Number)
{
// this function deletes a letter from within a text string
	if(m_AllocSize > 0)
	{
		if(Number < m_Size)
		{
			STRING Sour = m_String + Number + 1;
			STRING Dest = m_String + Number;
			WORD t = m_Size - Number;  // correction - used to add one
			for(; t > 0; --t, ++Sour, ++Dest)
			{
				*Dest = *Sour;
			}
			--m_Size;
		}
	}
	return *this;
// returns this object
}

CString &CString::Insert(WORD Number, CHAR Letter)
{
// this function inserts a letter between text or appends it after the text
// with space padding if the letter is out of range
	if (Number < m_Size)
   {
		NewAllocSize(m_Size + 1); // corrected, was + 2
		WORD t = m_Size - Number + 1;
		STRING Sour = m_String + m_Size;
		STRING Dest = m_String + m_Size + 1; // corrected, was - 1
		for(;t;--t, --Sour, --Dest)
		{
			*Dest = *Sour;
		}
		*(m_String + Number) = Letter;
		++m_Size;
	}
	else
	{
		NewAllocSize(Number + 1); // corrected, was + 2

		STRING Dest = m_String + m_Size;
		WORD t = Number - m_Size;
		for(; t; --t, ++Dest)
		{
			*Dest = CCHAR(' ');
		}
		*(m_String + Number) = Letter;
		*(m_String + Number + 1) = 0;
		m_Size = Number + 1;
	}
	return *this;
// returns this object
}

CString &CString::Overwrite(WORD Number, CHAR Letter)
{
// this function overwrites a letter or appends the letter with space padding
// if the letter is beyond the text string's range
	if (Number < m_Size)
	{
		*(m_String + Number) = Letter;
	}
	else
	{
		NewAllocSize(Number + 1); // corrected, was + 2
		STRING Dest = m_String + m_Size;
		WORD t = Number - m_Size;
		for(; t > 0; --t, ++Dest)
		{
			*Dest = CCHAR(' ');
		}
		*(m_String + Number) = Letter;
		*(m_String + Number + 1) = 0;
		m_Size = Number + 1;
	}
	return *this;
// returns this object
}

CString &CString::Insert(WORD Number, const CString &NewString)
{
// this function inserts one string into another string and or adds
// padded space and then the string if the index is out of the original
// string's range
	if (this == (&NewString))
		return *this;

	if (NewString.m_Size == 0)
		return *this;

	if (Number < m_Size)
	{
		NewAllocSize(m_Size + NewString.m_Size);

		STRING Sour = m_String + m_Size;
		STRING Dest = m_String + m_Size + NewString.m_Size;
		WORD t = m_Size - Number + 1;
		for(; t > 0; --t, --Sour, --Dest)
		{
			*Dest = *Sour;
		}

		Sour = NewString.m_String;
		Dest = m_String + Number;
		t = NewString.m_Size;
		for(; t > 0; --t, ++Sour, ++Dest)
		{
			*Dest = *Sour;
		}
		m_Size += NewString.m_Size;
	}
	else
	{
		NewAllocSize(Number + NewString.m_Size);

		STRING Dest = m_String + m_Size;
		WORD t = Number - m_Size;
		for(; t > 0; --t, ++Dest)
		{
			*Dest = CCHAR(' ');
		}
		STRING Sour = NewString.m_String;
		Dest = m_String + Number;
		t = NewString.m_Size + 1; // must copy null byte from source
		for(; t > 0; --t, ++Sour, ++Dest)
		{
			*Dest = *Sour;
		}
		m_Size = Number + NewString.m_Size;
	}

	return *this;
// returns this object
}

CString &CString::Insert(WORD Number, const STRING NewString)
{
	CString Tmp;
	Tmp.Upload(NewString);

	Insert(Number, Tmp);

	Tmp.Flush();
	return *this;
}

CString &CString::Overwrite(WORD Number, const CString &NewString)
{
	if (this == (&NewString))
		return *this;

	if (NewString.m_Size == 0)
		return *this;

	if (Number < m_Size)
	{
		WORD TmpNewSize = m_Size;

		if (Number + NewString.m_Size > m_Size)
		{
			NewAllocSize(Number + NewString.m_Size);
			*(m_String + Number + NewString.m_Size) = 0;

			TmpNewSize = Number + NewString.m_Size;
		}

		STRING Sour = NewString.m_String;
		STRING Dest = m_String + Number;
		WORD t = NewString.m_Size;
		for(; t > 0; --t, ++Sour, ++Dest)
		{
			*Dest = *Sour;
		}

		m_Size = TmpNewSize;
	}
	else
	{
		NewAllocSize(Number + NewString.m_Size);

		STRING Dest = m_String + m_Size;
		WORD t = Number - m_Size;
		for(; t > 0; --t, ++Dest)
		{
			*Dest = CCHAR(' ');
		}
		STRING Sour = NewString.m_String;
		Dest = m_String + Number;
		t = NewString.m_Size + 1; // must copy null byte from source
		for(; t > 0; --t, ++Sour, ++Dest)
		{
			*Dest = *Sour;
		}
		m_Size = Number + NewString.m_Size;
	}

	return *this;
}

CString &CString::Overwrite(WORD Number, const STRING NewString)
{
	CString Tmp;
	Tmp.Upload(NewString);

	Overwrite(Number, Tmp);

	Tmp.Flush();
	return *this;
}

CString &CString::Left(WORD Count)
{
// this function truncates the text string after the index
	Trunc(Count);

	return *this;
// returns this object
}

CString &CString::Right(WORD Number)
{
// this function removes text to the left given an index from the
// right of the string, 0 = NULL BYTE
	if (m_Size == 0)
		return *this;

	if (Number <= m_Size)
	{
		STRING Sour = m_String + (m_Size - Number);
		STRING Dest = m_String;
		WORD t = Number + 1;
		for(; t > 0; --t, ++Sour, ++Dest)
		{
			*Dest = *Sour;
		}
		m_Size = Number;
	}

	return *this;
// returns this object
}

CString &CString::Mid(WORD Start, WORD Count)
{
// given an index and a count this function clips out text from within
// the larger string
	if (Start > m_Size)
		Clear();
	else
	{
		Left(Start + Count);
		if (Start + Count > m_Size)
			Right(m_Size - Start);
		else
			Right(Count);
	}

	return *this;
// returns this object
}

CString &CString::CutOut(WORD Start, WORD Count)
{
	if (m_Size == 0)
		return *this;

	if (Start >= m_Size)
		return *this;

	if (Start + Count > m_Size)
		Count = m_Size - Start;

	STRING Sour = m_String + Start + Count;
	STRING Dest = m_String + Start;

	WORD t = m_Size - Start - Count + 1;
	for(; t > 0; --t, ++Sour, ++Dest)
	{
		*Dest = *Sour;
	}

	m_Size -= Count;

	return *this;
}

CString::operator const STRING()
{
	if(m_AllocSize > 0)
		return (const STRING)m_String;
	else
		return (const STRING)_str_test_;
}

CString &CString::CutEndUntilChar(CHAR Letter)
{
	if (Letter == 0)
		return *this;

	if (!CaseSensitive())
	{
		ICutEndUntilChar(Letter);
		return *this;
	}

	if ((m_AllocSize > 0)&&(m_Size > 0))
	{
		STRING Dest = m_String + m_Size - 1;
		WORD CutCount = m_Size;
		for(; (CutCount > 0) && (*Dest != Letter); --CutCount, --Dest, --m_Size)
		{
			*Dest = 0;
		}
		if (m_Size == 0)
			Clear();
	}
	return *this;
}

CString &CString::ICutEndUntilChar(CHAR Letter)
{
	if (Letter == 0)
		return *this;

	Letter = _LOWERLETTERS[CBYTE(Letter)];

	if ((m_AllocSize > 0)&&(m_Size > 0))
	{
		STRING Dest = m_String + m_Size - 1;
		WORD CutCount = m_Size;
		for(; (CutCount > 0) && (_LOWERLETTERS[CBYTE(*Dest)] != Letter); --CutCount, --Dest, --m_Size)
		{
			*Dest = 0;
		}
		if (m_Size == 0)
			Clear();
	}
	return *this;
}

CString &CString::CutEndChar(CHAR Letter)
{
	if (Letter == 0)
		return *this;

	if (!CaseSensitive())
	{
		ICutEndChar(Letter);
		return *this;
	}

	if ((m_AllocSize > 0)&&(m_Size > 0))
	{
		STRING Dest = m_String + m_Size - 1;
		WORD CutCount = m_Size;
		for(; (CutCount > 0) && (*Dest == Letter); --CutCount, --Dest, --m_Size)
		{
			*Dest = 0;
		}
		if (m_Size == 0)
			Clear();
	}
	return *this;
}

CString &CString::ICutEndChar(CHAR Letter)
{
	if (Letter == 0)
		return *this;

	Letter = _LOWERLETTERS[CBYTE(Letter)];

	if ((m_AllocSize > 0)&&(m_Size > 0))
	{
		STRING Dest = m_String + m_Size - 1;
		WORD CutCount = m_Size;
		for(; (CutCount > 0) && (_LOWERLETTERS[CBYTE(*Dest)] == Letter); --CutCount, --Dest, --m_Size)
		{
			*Dest = 0;
		}
		if (m_Size == 0)
			Clear();
	}
	return *this;
}

CString &CString::CutStartUntilChar(CHAR Letter)
{
	if (Letter == 0)
		return *this;

	if (!CaseSensitive())
	{
		ICutStartUntilChar(Letter);
		return *this;
	}

	if (m_Size > 0)
	{
		STRING Sour = m_String;
		WORD Count;
		for(Count = 0; (*Sour != Letter) && (*Sour != 0); ++Sour, ++Count);
		Right(m_Size - Count);
	}

	return *this;
}

CString &CString::ICutStartUntilChar(CHAR Letter)
{
	if (Letter == 0)
		return *this;

	Letter = _LOWERLETTERS[CBYTE(Letter)];

	if (m_Size > 0)
	{
		STRING Sour = m_String;
		WORD Count;
		for(Count = 0; (_LOWERLETTERS[CBYTE(*Sour)] != Letter) && (*Sour != 0); ++Sour, ++Count);
		Right(m_Size - Count);
	}

	return *this;
}

CString &CString::CutStartChar(CHAR Letter)
{
	if (Letter == 0)
		return *this;

	if (!CaseSensitive())
	{
		ICutStartChar(Letter);
		return *this;
	}

	if (m_Size > 0)
	{
		STRING Sour = m_String;
		WORD Count;
		for(Count = 0; (*Sour == Letter) && (*Sour != 0); ++Sour, ++Count);
		Right(m_Size - Count);
	}

	return *this;
}

CString &CString::ICutStartChar(CHAR Letter)
{
	if (Letter == 0)
		return *this;

	Letter = _LOWERLETTERS[CBYTE(Letter)];

	if (m_Size > 0)
	{
		STRING Sour = m_String;
		WORD Count;
		for(Count = 0; (_LOWERLETTERS[CBYTE(*Sour)] == Letter) && (*Sour != 0); ++Sour, ++Count);
		Right(m_Size - Count);
	}

	return *this;
}

CString &CString::ToUpper()
{
	if (m_Size > 0)
	{
		STRING Dest = m_String;
		for(; *Dest; ++Dest)
		{
			*Dest = _UPPERLETTERS[CBYTE(*Dest)];
		}
	}
	return *this;
}

CString &CString::ToLower()
{
	if (m_Size > 0)
	{
		STRING Dest = m_String;
		for(; *Dest; ++Dest)
		{
			*Dest = _LOWERLETTERS[CBYTE(*Dest)];
		}
	}
	return *this;
}

CString &CString::operator+=(const CString &NewString)
{
	if (this == (&NewString))
		return *this;

	Insert(m_Size, NewString);

	return *this;
}

CString &CString::operator+=(const STRING NewString)
{
	CString Tmp;
	Tmp.Upload(NewString);

	(*this) += Tmp;

	Tmp.Flush();
	return *this;
}

CString &CString::operator=(const CString &NewString)
{
	if (this == (&NewString))
		return *this;

	Trunc(0);
	Insert(0,NewString);

	return *this;
}

CString &CString::operator=(const STRING NewString)
{
	CString Tmp;
	Tmp.Upload(NewString);

	(*this) = Tmp;

	Tmp.Flush();
	return *this;
}

BOOL CString::Find(WORD &Position, const STRING StringToFind)
{
	CString Tmp;
	Tmp.Upload(StringToFind);

	BOOL Return;
	Return = Find(Position, Tmp);

	Tmp.Flush();
	return Return;
}

BOOL CString::Find(WORD &Position, const CString &StringToFind)
{
	if ((StringToFind.m_Size == 0)||(m_Size == 0))
		return FALSE;

	if (Position >= m_Size)
		return FALSE;

	STRING Sour = m_String + Position;
	if (CaseSensitive())
	{
		for(; *Sour; ++Sour, ++Position)
		{
				STRING SourIndex = Sour;
				STRING SearchIndex = StringToFind.m_String;
				for(; ((*SourIndex) == (*SearchIndex)) && (*SourIndex) && (*SearchIndex); ++SourIndex, ++SearchIndex);
				if (*SearchIndex == 0)
					return TRUE;
		}
	}
	else
	{
		for(; *Sour; ++Sour, ++Position)
		{
				STRING SourIndex = Sour;
				STRING SearchIndex = StringToFind.m_String;
				for(; ((_LOWERLETTERS[CBYTE(*SourIndex)]) == (_LOWERLETTERS[CBYTE(*SearchIndex)])) && (*SourIndex) && (*SearchIndex); ++SourIndex, ++SearchIndex);
				if ((*SearchIndex) == 0)
					return TRUE;
		}
	}
	return FALSE;
}

BOOL CString::Replace(WORD &Position, WORD &AfterPosition, const CString &StringToFind, const CString &Replacement)
{
	if (this == (&Replacement))
		return FALSE;

	if ((StringToFind.m_Size == 0)||(m_Size == 0))
		return FALSE;

	if (Find(Position, StringToFind))
	{
		CutOut(Position, StringToFind.m_Size);
		if (Replacement.m_Size > 0)
		{
			Insert(Position, Replacement);
			AfterPosition = Position + Replacement.m_Size;
			return TRUE;
		}
		AfterPosition = Position;
		return TRUE;
	}
	return FALSE;
}

BOOL CString::Replace(WORD &Position, WORD &AfterPosition, const STRING StringToFind, const CString &Replacement)
{
	CString Tmp;
	Tmp.Upload(StringToFind);

	BOOL Return;
	Return = Replace(Position, AfterPosition, Tmp, Replacement);

	Tmp.Flush();
	return Return;
}

BOOL CString::Replace(WORD &Position, WORD &AfterPosition, const CString &StringToFind, const STRING Replacement)
{
	CString Tmp;
	Tmp.Upload(Replacement);

	BOOL Return;
	Return = Replace(Position, AfterPosition, StringToFind, Tmp);

	Tmp.Flush();
	return Return;
}

BOOL CString::Replace(WORD &Position, WORD &AfterPosition, const STRING StringToFind, const STRING Replacement)
{
	CString Tmp, Tmp2;
	Tmp.Upload(StringToFind);
	Tmp2.Upload(Replacement);

	BOOL Return;
	Return = Replace(Position, AfterPosition, Tmp, Tmp2);

	Tmp.Flush();
	Tmp2.Flush();
	return Return;
}

BOOL CString::ReplaceAll(const CString &StringToFind, const CString &Replacement)
{
	if (this == (&Replacement))
		return FALSE;

	if ((StringToFind.m_Size == 0)||(m_Size == 0))
		return FALSE;

	BOOL FoundSomething, Return;
	WORD Start = 0;
	WORD After = 0;
	Return = FoundSomething = Replace(Start, After, StringToFind, Replacement);
	while (FoundSomething)
	{
		Start = After;
		FoundSomething = Replace(Start, After, StringToFind, Replacement);
	}

	return Return;
}

BOOL CString::ReplaceAll(const STRING StringToFind, const CString &Replacement)
{
	CString Tmp;
	Tmp.Upload(StringToFind);

	BOOL Return;
	Return = ReplaceAll(Tmp, Replacement);

	Tmp.Flush();
	return Return;
}

BOOL CString::ReplaceAll(const CString &StringToFind, const STRING Replacement)
{
	CString Tmp;
	Tmp.Upload(Replacement);

	BOOL Return;
	Return = ReplaceAll(StringToFind, Tmp);

	Tmp.Flush();
	return Return;
}

BOOL CString::ReplaceAll(const STRING StringToFind, const STRING Replacement)
{
	CString Tmp, Tmp2;
	Tmp.Upload(StringToFind);
	Tmp2.Upload(Replacement);

	BOOL Return;
	Return = ReplaceAll(Tmp, Tmp2);

	Tmp.Flush();
	Tmp2.Flush();
	return Return;
}


CString &CString::Subst(CHAR Letter, CHAR Replacement)
{
	if (m_Size == 0)
		return *this;

	if (CaseSensitive())
	{
		STRING Sour = m_String;
		for(; *Sour; ++Sour)
		{
			if (*Sour == Letter)
				*Sour = Replacement;
		}
	}
	else
	{
		Letter = _LOWERLETTERS[CBYTE(Letter)];
		STRING Sour = m_String;
		for(; *Sour; ++Sour)
		{
			if (_LOWERLETTERS[CBYTE(*Sour)] == Letter)
				*Sour = Replacement;
		}
	}
	return *this;
}

SWORD CString::Compare(const CString &NewString)
{
	if (this == (&NewString))
		return 0;
	if ((m_Size == 0)&&(NewString.m_Size == 0))
		return 0;
	if (NewString.m_Size == 0)
		return 1;
	if (m_Size == 0)
		return -1;

	if (!CaseSensitive())
		return ICompare(NewString);

	STRING Sour = m_String;
	STRING Dest = NewString.m_String;
	for (; ((_COMPARELETTERS[CBYTE(*Sour)]) == (_COMPARELETTERS[CBYTE(*Dest)])) && (*Sour); ++Sour, ++Dest);

	return (((SWORD)_COMPARELETTERS[CBYTE(*Sour)]) - ((SWORD)_COMPARELETTERS[CBYTE(*Dest)]));
}

SWORD CString::ICompare(const CString &NewString)
{
	STRING Sour = m_String;
	STRING Dest = NewString.m_String;
	for (; ((_COMPARELETTERS[CBYTE(_LOWERLETTERS[CBYTE(*Sour)])]) == (_COMPARELETTERS[CBYTE(_LOWERLETTERS[CBYTE(*Dest)])])) && (*Sour); ++Sour, ++Dest);

	return (((SWORD)_COMPARELETTERS[CBYTE(_LOWERLETTERS[CBYTE(*Sour)])]) - ((SWORD)_COMPARELETTERS[CBYTE(_LOWERLETTERS[CBYTE(*Dest)])]));
}

SWORD CString::Compare(const STRING NewString)
{
	CString Tmp;
	Tmp.Upload(NewString);

	SWORD Return;
	Return = Compare(Tmp);

	Tmp.Flush();
	return Return;
}

BOOL CString::operator==(const STRING NewString)
{
	return (Compare(NewString) == 0);
}

BOOL CString::operator==(const CString &NewString)
{
	return (Compare(NewString) == 0);
}

BOOL CString::operator!=(const STRING NewString)
{
	return (Compare(NewString) != 0);
}

BOOL CString::operator!=(const CString &NewString)
{
	return (Compare(NewString) != 0);
}

BOOL CString::operator>(const STRING NewString)
{
	return (Compare(NewString) > 0);
}

BOOL CString::operator>(const CString &NewString)
{
	return (Compare(NewString) > 0);
}

BOOL CString::operator<(const STRING NewString)
{
	return (Compare(NewString) < 0);
}

BOOL CString::operator<(const CString &NewString)
{
	return (Compare(NewString) < 0);
}

BOOL CString::operator>=(const STRING NewString)
{
	return (Compare(NewString) >= 0);
}

BOOL CString::operator>=(const CString &NewString)
{
	return (Compare(NewString) >= 0);
}

BOOL CString::operator<=(const STRING NewString)
{
	return (Compare(NewString) <= 0);
}

BOOL CString::operator<=(const CString &NewString)
{
	return (Compare(NewString) <= 0);
}
