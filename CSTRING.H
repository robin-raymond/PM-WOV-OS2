#ifndef _STRING_H_
#define _STRING_H_
// Last checked Dec 6, 1995

#include "stdtypes.h"
#include "alloc.h"
#include "citem.h"
#include "memory.h"

WORD GetCount(const STRING);

void CaseSensitive(BOOL);
BOOL CaseSensitive();

class CString : public CItem {
		STRING m_String;
		WORD m_AllocSize;
		WORD m_Size;

		void ULONGToString(ULONG);
		void SLONGToString(SLONG);
		ULONG StringToULONG();
		SLONG StringToSLONG();

		void NewAllocSize(WORD);

		CString &ICutEndUntilChar(CHAR);
		CString &ICutEndChar(CHAR);
		CString &ICutStartUntilChar(CHAR);
		CString &ICutStartChar(CHAR);

		SWORD ICompare(const CString &);
		void Upload(const STRING);
		void Flush();

		IDENTIFYCITEM();
	public:
		CString() {ResetPointer((BINARY)m_String); m_AllocSize = 0; m_Size = 0;}
		CString(STRING NewString) {ResetPointer((BINARY)m_String); m_AllocSize = 0; m_Size = 0; *this = NewString;}
		CString(CString &NewString) {ResetPointer((BINARY)m_String); m_AllocSize = 0; m_Size = 0; *this = NewString;}

		WORD GetSize() {return m_Size;}
		WORD operator!() {return GetSize();}

		CString &Trunc(WORD);

		CString &DeleteLetter(WORD);

		CString &Insert(WORD, CHAR);
		CString &Overwrite(WORD, CHAR);
		CString &Insert(WORD, const CString &);
		CString &Overwrite(WORD, const CString &);
		CString &Insert(WORD, const STRING);
		CString &Overwrite(WORD, const STRING);

		CString &Left(WORD);
		CString &Right(WORD);
		CString &Mid(WORD, WORD);
		CString &CutOut(WORD, WORD);

		operator const STRING();

		CString &CutEndUntilChar(CHAR);
		CString &CutEndChar(CHAR);
		CString &CutStartUntilChar(CHAR);
		CString &CutStartChar(CHAR);

		CString &ToUpper();
		CString &ToLower();

		CString &operator=(const STRING);
		CString &operator=(const CString &);
		CString &operator+=(const CString &);
		CString &operator+=(const STRING);

		BOOL Find(WORD &, const CString &);
		BOOL Find(WORD &, const STRING);
		BOOL Replace(WORD &, WORD &, const CString &, const CString &);
		BOOL Replace(WORD &, WORD &, const STRING, const CString &);
		BOOL Replace(WORD &, WORD &, const CString &, const STRING);
		BOOL Replace(WORD &, WORD &, const STRING, const STRING);
		BOOL ReplaceAll(const CString &, const CString &);
		BOOL ReplaceAll(const STRING, const CString &);
		BOOL ReplaceAll(const CString &, const STRING);
		BOOL ReplaceAll(const STRING, const STRING);

		CString &Subst(CHAR, CHAR);

		SWORD Compare(const CString &);
		SWORD Compare(const STRING);

		BOOL operator==(const STRING);
		BOOL operator==(const CString &);
		BOOL operator!=(const STRING);
		BOOL operator!=(const CString &);
		BOOL operator>(const STRING);
		BOOL operator>(const CString &);
		BOOL operator<(const STRING);
		BOOL operator<(const CString &);

		BOOL operator>=(const STRING);
		BOOL operator>=(const CString &);
		BOOL operator<=(const STRING);
		BOOL operator<=(const CString &);

		CString &operator<<(BYTE a) {ULONGToString(CULONG(a)); return *this;}
		CString &operator<<(WORD a) {ULONGToString(CULONG(a)); return *this;}
//		CString &operator<<(UINT a) {ULONGToString(CULONG(a)); return *this;}
		CString &operator<<(ULONG a) {ULONGToString(a); return *this;}
		CString &operator<<(CHAR a) {SLONGToString(SLONG(a)); return *this;}
		CString &operator<<(SINT a) {SLONGToString(SLONG(a)); return *this;}
//		CString &operator<<(SWORD a) {SLONGToString(SLONG(a)); return *this;}
		CString &operator<<(SLONG a) {SLONGToString(a); return *this;}
		CString &operator<<(CString &a) {return *this = a;}
		CString &operator<<(STRING a) {return *this = a;}

		CString &operator>>(BYTE &a) {ULONG Tmp = StringToULONG(); a = CBYTE(Tmp); return *this;}
		CString &operator>>(WORD &a) {ULONG Tmp = StringToULONG(); a = CWORD(Tmp); return *this;}
//		CString &operator>>(UINT &a) {ULONG Tmp = StringToULONG(); a = CUINT(Tmp); return *this;}
		CString &operator>>(ULONG &a) {ULONG Tmp = StringToULONG(); a = Tmp; return *this;}
		CString &operator>>(CHAR &a) {SLONG Tmp = StringToSLONG(); a = CCHAR(Tmp); return *this;}
		CString &operator>>(SINT &a) {SLONG Tmp = StringToSLONG(); a = CSINT(Tmp); return *this;}
//		CString &operator>>(SWORD &a) {SLONG Tmp = StringToSLONG(); a = CSWORD(Tmp); return *this;}
		CString &operator>>(SLONG &a) {SLONG Tmp = StringToSLONG(); a = CSLONG(Tmp); return *this;}
		CString &operator>>(CString &a) {a = *this; return *this;}

		void Clear() {NewAllocSize(0);}

		void operator--() {Clear();}
		~CString();
};

#endif
