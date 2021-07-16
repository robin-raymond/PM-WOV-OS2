#define INCL_DOSERRORS
#include "nfile.hpp"

BOOL NFile::Open(PSZ FName, ULONG &Action, ULONG OpenMode, PEAOP2 pEABuf)
{
	m_Error = DosOpen(FName, &m_File, &Action, 0, FILE_NORMAL, OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
						  OpenMode, pEABuf);
	return (m_Error == NO_ERROR);
}

BOOL NFile::Create(PSZ FName, ULONG &Action, ULONG FSize, ULONG FType, ULONG OpenMode, PEAOP2 pEABuf)
{
	m_Error = DosOpen(FName, &m_File, &Action, FSize, FType, OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_FAIL_IF_EXISTS,
						  OpenMode, pEABuf);
	return (m_Error == NO_ERROR);
}

BOOL NFile::OpenCreate(PSZ FName, ULONG &Action, ULONG FSize, ULONG FType, ULONG OpenMode, PEAOP2 pEABuf)
{
	m_Error = DosOpen(FName, &m_File, &Action, FSize, FType, OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
						  OpenMode, pEABuf);
	return (m_Error == NO_ERROR);
}

BOOL NFile::OpenReplace(PSZ FName, ULONG &Action, ULONG FSize, ULONG FType, ULONG OpenMode, PEAOP2 pEABuf)
{
	m_Error = DosOpen(FName, &m_File, &Action, FSize, FType, OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_REPLACE_IF_EXISTS,
						  OpenMode, pEABuf);
	return (m_Error == NO_ERROR);
}

BOOL NFile::OpenCreateReplace(PSZ FName, ULONG &Action, ULONG FSize, ULONG FType, ULONG OpenMode, PEAOP2 pEABuf)
{
	m_Error = DosOpen(FName, &m_File, &Action, FSize, FType, OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_REPLACE_IF_EXISTS,
						  OpenMode, pEABuf);
	return (m_Error == NO_ERROR);
}

BOOL NFile::Open(PSZ FName, ULONG OpenMode)
{
	ULONG Junk;

	return Open(FName, Junk, OpenMode | OPEN_FLAGS_NOINHERIT, NULL);
}

BOOL NFile::Create(PSZ FName, ULONG OpenMode)
{
	ULONG Junk;

	return Create(FName, Junk, 0, FILE_NORMAL, OpenMode | OPEN_FLAGS_NOINHERIT, NULL);
}

BOOL NFile::OpenCreate(PSZ FName, ULONG OpenMode)
{
	ULONG Junk;

	return OpenCreate(FName, Junk, 0, FILE_NORMAL, OpenMode | OPEN_FLAGS_NOINHERIT, NULL);
}

BOOL NFile::OpenReplace(PSZ FName, ULONG OpenMode)
{
	ULONG Junk;

	return OpenReplace(FName, Junk, 0, FILE_NORMAL, OpenMode | OPEN_FLAGS_NOINHERIT, NULL);
}

BOOL NFile::OpenCreateReplace(PSZ FName, ULONG OpenMode)
{
	ULONG Junk;

	return OpenCreateReplace(FName, Junk, 0, FILE_NORMAL, OpenMode | OPEN_FLAGS_NOINHERIT, NULL);
}

BOOL NFile::Close()
{
	if (m_File != NULLHANDLE)
	{
		m_Error = DosClose(m_File);
		m_File = NULLHANDLE;
	}
	else
		m_Error = ERROR_FILE_NOT_FOUND;
	return (m_Error == NO_ERROR);
}

ULONG NFile::Read(PVOID Buffer, ULONG Size)
{
	ULONG BytesRead;

	m_Error = DosRead(m_File, Buffer, Size, &BytesRead);
	return BytesRead;
}

ULONG NFile::Write(PVOID Buffer, ULONG Size)
{
	ULONG BytesWritten;

	m_Error = DosWrite(m_File, Buffer, Size, &BytesWritten);
	return BytesWritten;
}