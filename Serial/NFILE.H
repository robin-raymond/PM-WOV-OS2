#if !defined(__NFILE__)
#define __NFILE__
#include "string.h"

#define READ 0
#define WRITE 1
#define READWRITE 2
#define D_READ 48
#define D_WRITE 32
#define D_READWRITE 16
#define SHARE 64

#define CREATE 1
#define NO_CREATE 0

#define MKDIR 0
#define RMDIR 1
#define CHDIR 2

#define NO_FILE 2
#define NO_PATH 3
#define NO_HANDLES 4
#define NO_ACCESS 5
#define INV_HANDLE 6
#define WRONG_CODE 12
#define INV_FUNC 1
#define NO_ERROR 0

#define NORMAL 0
#define HIDDEN 2
#define SYSTEM 4
#define MAXHANDLE 40

#define BEG_POS 0
#define CUR_POS 1
#define END_POS 2

#define CUR_DRIVE 0

#define SCRAM_SIZE 5

us nerase(string &);
us ndir(string &, uc);
us nmkdir(string &);
us nrmdir(string &);
us nchdir(string &);
ul nspace(uc);
uc validdrive(uc);
uc networknum();
us nrename(string &, string &);
us ngetdirectory(string &, uc);

class NFILE {
		us handle;
		uc createf;
		uc filetype;
		uc scramron;
		uc scramwon;
		uc scramr[SCRAM_SIZE];
		uc scramw[SCRAM_SIZE];
		ul totalrw;
		us n_open(string &, uc,uc);
		us n_read(void *,us,us);
		us n_write(void *,us,us);
	public:
		NFILE() {exterror = 0; newfile();}
		uc created;
		us exterror;
		void newfile() {scramwon = scramron = handle = totalrw = created = 0;}
		us geterror() {return exterror;}
		ul getrw() {ul m = totalrw; totalrw = 0; return m;}
		void setrw(ul m) {totalrw = m;}
		uc iscreated() {return created;}
		us nopen(string &str, uc acc, uc wait) {createf = NO_CREATE; return n_open(str,acc,wait);}
		us nopen(string &str, uc acc, uc wait, uc mode) {createf = CREATE; filetype = mode; return n_open(str,acc,wait);}
		us ncreate(string &, uc,uc);
		us nclose();
		void scramread();
		void scramwrite();
		void scramoff() {scramwon = scramron = 0;}
		void scramroff() {scramron = 0;}
		void scramwoff() {scramwon = 0;}
		us nread(void *,us,us);
		us nwrite(void *,us,us);
		ul nseek(long,uc);
		us nappend() {nseek(0,END_POS); return exterror;}
		us nrewind() {nseek(0,BEG_POS); return exterror;}
		NFILE &operator<<(uc n) {nwrite(&n,sizeof(uc),1); return *this;}
		NFILE &operator<<(us n) {nwrite(&n,sizeof(us),1); return *this;}
		NFILE &operator<<(ul n) {nwrite(&n,sizeof(ul),1); return *this;}
		NFILE &operator<<(sc n) {nwrite(&n,sizeof(sc),1); return *this;}
		NFILE &operator<<(int n) {nwrite(&n,sizeof(int),1); return *this;}
		NFILE &operator<<(long n) {nwrite(&n,sizeof(long),1); return *this;}
		NFILE &operator<<(string &n) {n.write(*this); return *this;}
		NFILE &operator>>(uc &n) {nread(&n,sizeof(uc),1); return *this;}
		NFILE &operator>>(us &n) {nread(&n,sizeof(us),1); return *this;}
		NFILE &operator>>(ul &n) {nread(&n,sizeof(ul),1); return *this;}
		NFILE &operator>>(sc &n) {nread(&n,sizeof(sc),1); return *this;}
		NFILE &operator>>(int &n) {nread(&n,sizeof(int),1); return *this;}
		NFILE &operator>>(long &n) {nread(&n,sizeof(long),1); return *this;}
		NFILE &operator>>(string &n) {n.read(*this); return *this;}
		~NFILE() {nclose();}
};
#endif