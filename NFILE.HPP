#ifndef _NFILE_HPP_
#define _NFILE_HPP_


#include "cwindow.h"
#define INCL_DOSFILEMGR
#include <os2.h>

class NFile {
		HFILE m_File;
		APIRET m_Error;
	public:
		NFile() {m_File = NULLHANDLE;}
		BOOL Open(PSZ, ULONG &, ULONG, PEAOP2);
		BOOL Create(PSZ, ULONG &, ULONG, ULONG, ULONG, PEAOP2);
		BOOL OpenCreate(PSZ, ULONG &, ULONG, ULONG, ULONG, PEAOP2);
		BOOL OpenReplace(PSZ, ULONG &, ULONG, ULONG, ULONG, PEAOP2);
		BOOL OpenCreateReplace(PSZ, ULONG &, ULONG, ULONG, ULONG, PEAOP2);

		BOOL Open(PSZ, ULONG);
		BOOL Create(PSZ, ULONG);
		BOOL OpenCreate(PSZ, ULONG);
		BOOL OpenReplace(PSZ, ULONG);
		BOOL OpenCreateReplace(PSZ, ULONG);

		BOOL Close();

		APIRET GetError() {return m_Error;}
		void SetError(APIRET Error) {m_Error = Error;}

		ULONG Read(PVOID, ULONG);
		ULONG Write(PVOID, ULONG);

		NFile &operator<<(char Value) {Write(&Value, sizeof(char)); return *this;}
		NFile &operator<<(unsigned char Value) {Write(&Value, sizeof(unsigned char)); return *this;}
		NFile &operator<<(USHORT Value) {Write(&Value, sizeof(USHORT)); return *this;}
		NFile &operator<<(ULONG Value) {Write(&Value, sizeof(ULONG)); return *this;}
		NFile &operator<<(SHORT Value) {Write(&Value, sizeof(SHORT)); return *this;}
		NFile &operator<<(LONG Value) {Write(&Value, sizeof(LONG)); return *this;}

		NFile &operator>>(char &Value) {Read(&Value, sizeof(char)); return *this;}
		NFile &operator>>(unsigned char &Value) {Read(&Value, sizeof(unsigned char)); return *this;}
		NFile &operator>>(USHORT &Value) {Read(&Value, sizeof(USHORT)); return *this;}
		NFile &operator>>(ULONG &Value) {Read(&Value, sizeof(ULONG)); return *this;}
		NFile &operator>>(SHORT &Value) {Read(&Value, sizeof(SHORT)); return *this;}
		NFile &operator>>(LONG &Value) {Read(&Value, sizeof(LONG)); return *this;}
};

#endif
