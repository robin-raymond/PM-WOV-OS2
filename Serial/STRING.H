#include "all.h"
#include "alloc.h"

#if !defined(__STRING__)
#define __STRING__
class NFILE;

class string {
		uc *str;
		uc info;
		us allocsiz;
		us siz;
		void ultos(ul);
		void sltos(sl);
		ul stoul();
		sl stosl();
		void putletter(us,uc);
	public:
		string() {info = 0;}
		string(char *a) {info = 0; *this = a;}
		string(string &a) {info = 0; *this = a;}
		string &operator=(us);
		string &operator=(char *);
		string &operator=(string &);
		string &operator+=(string &);
		string &operator+=(char *);
		string &trail(uc);
		string &lead(uc);
		string &upper();
		string &lower();
		string &end(uc);
		string &deleteletter(us);
		string &appendat(us,string &);
      string &insertletter(us,uc);
		string &overwriteletter(us,uc);
		string &trunc(us size) {if(info) {if(size <= allocsiz){*(str + size) = 0; if(size < siz) siz = size; }} return *this;}
		void resize();
		string &setalloc(us);
		uc *display();
		us getsize() {if(info) return siz; return 0;}
		void setsize(us s) {siz = s;}
		string &remove(uc);
		us read(NFILE &);
		us write(NFILE &);
		us writespecial(NFILE &);
		us writeeof(NFILE &);
		sc operator^=(string &);
		sc operator==(string &);
		sc operator==(char *);
		sc operator^=(char *);
		uc operator>(string &op2) {return ((*this == op2) > 0);}
		uc operator<(string &op2) {return ((*this == op2) < 0);}
		uc operator>=(string &op2) {return ((*this == op2) >= 0);}
		uc operator<=(string &op2) {return ((*this == op2) <= 0);}
		uc operator>>=(string &op2) {return ((*this ^= op2) >= 0);}
		uc operator<<=(string &op2) {return ((*this ^= op2) <= 0);}
		string &operator<<(uc a) {ultos(a); return *this;}
		string &operator<<(us a) {ultos(a); return *this;}
		string &operator<<(ul a) {ultos(a); return *this;}
		string &operator<<(sc a) {sltos(a); return *this;}
		string &operator<<(si a) {sltos(a); return *this;}
		string &operator<<(sl a) {sltos(a); return *this;}
		string &operator<<(string &a) {return *this = a;}
		string &operator<<(char *a) {return *this = a;}
		string &operator>>(uc &a) {ul tmp = stoul(); a = tmp; return *this;}
		string &operator>>(us &a) {ul tmp = stoul(); a = tmp; return *this;}
		string &operator>>(ul &a) {ul tmp = stoul(); a = tmp; return *this;}
		string &operator>>(sc &a) {sl tmp = stosl(); a = tmp; return *this;}
		string &operator>>(si &a) {sl tmp = stosl(); a = tmp; return *this;}
		string &operator>>(sl &a) {sl tmp = stosl(); a = tmp; return *this;}
		string &operator>>(string &a) {a = *this; return *this;}
		us operator!() {if(info) {return siz;} return 0;}
		void operator--() {clear();}
		void clear() {if (info) {_free(str); info = 0;}}
		~string() {clear();}
};
#endif

NFILE &operator<<(NFILE &, string *);
NFILE &operator<<(NFILE &, string **);
