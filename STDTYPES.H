#ifndef _STANDARD_TYPES_
#define _STANDARD_TYPES_

//#define _DEBUG_VERSION_

//#define TRUE 1
//#define FALSE 0
//#define NULL 0
#define D_MAXWORD ((WORD)65000)

//typedef unsigned char BOOL;
//typedef unsigned char BYTE;
//typedef signed char CHAR;

typedef signed int SWORD;
typedef unsigned int WORD;

class CString;

//typedef unsigned int UINT;
//typedef signed int SINT;

//typedef unsigned long ULONG;
//typedef signed long SLONG;

typedef char* STRING;
typedef unsigned char * BINARY;

//typedef float FLOAT;

#define CBYTE(a) ((BYTE)a)
#define CBOOL(a) ((BOOL)(a != FALSE))
#define CCHAR(a) ((CHAR)a)
#define CSWORD(a) ((SWORD)a)
#define CWORD(a) ((WORD)a)
#define CUINT(a) ((UINT)a)
#define CSINT(a) ((SINT)a)
#define CULONG(a) ((ULONG)a)
#define CSLONG(a) ((SLONG)a)
#define CSTRING(a) ((STRING)a)
#define CBINARY(a) ((BINARY)a)
#define CFLOAT(a) ((FLOAT)a)
#define CMATCH(a) (&a)
#include <os2.h>

#endif
