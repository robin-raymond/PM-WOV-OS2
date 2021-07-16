#if !defined(__SCREEN__)
#define __SCREEN__
#include "string.h"

#define R50 0
#define R43 1
#define R25 2
#define CGAMEM 0xB8000000
#define MONMEM 0xB0000000
#define REMEM 1
#define NOMEM 0
#define NO 0
#define YES 1
#define TRUE 1
#define FALSE 0

#define SINGLE 0
#define DOUBLE 1
#define DOUTOP 2
#define SINTOP 3

#define HORZ 0
#define VERT 1

#define CUR_OFF 32,0
#define CUR_ON  6,7
#define CUR_BLOCK 0,15

#define __COLORS

enum __COLOR_PAL {
 BLACK = 0,BLUE,GREEN,CYAN,RED,MAGENTA,BROWN,LGRAY,
 GRAY,LBLUE,LGREEN,LCYAN,LRED,LMAGENTA,YELLOW,WHITE, LEAVE_COLOR
};

#define BA(a)    (((a) << 4) & 127)

extern uc columns;
extern uc rows;
extern uc monomode;

us getblock(uc,uc);
void setblock(uc,uc,us);
void _fillpattern(uc,uc,uc,uc,uc,uc);
void _backcolour(uc, uc, uc, uc, uc);
void _dpa(uc, uc, string &, uc, uc);
void _dpa(uc, uc, char *, uc, uc);
void _dpa(uc, uc, string &, uc);
void _dpa_overlay(uc, uc, string &, uc, uc);
void textline(uc, uc, uc, uc, uc, uc);
void textline(uc, uc, uc, uc, uc);
void upplet(uc &);
void _setupscreen(uc);
void cursor(uc,uc);
void cursorat(uc,uc);

class SETUP;

class SETUPSCREEN {
	public:
		SETUPSCREEN();
		void cleanscreen();
};

class BOX {
	protected:
		uc x1,y1,x2,y2;
		uc WC;
		us *mem;
		uc rem;
		void setbox(uc,uc,uc,uc,uc,uc,uc);
	public:
		BOX(uc a,uc b,uc c,uc d,uc e,uc f,uc g) {reset();setbox(a,b,c,d,e,f,g);}
		~BOX() {freeup();}
		void reset() {rem = 0;}
		void freeup_base();
		void scroll_up(uc);
		void scroll_down(uc);
		void scroll_right(uc);
		void scroll_left(uc);
		void freeup() {clear(); freeup_base();}
		virtual void clear() {;}
		virtual void displaybox(uc a,uc b,uc c,uc d,uc e,uc f,uc g) {setbox(a,b,c,d,e,f,g);}
};

class SHADEBOX:public BOX {
		uc shade;
		us *shadow;
		void createshadow();
		void removeshadow();
	public:
		SHADEBOX(uc a,uc b,uc c,uc d,uc e,uc f,uc g):BOX(a,b,c,d,e,f,g) {shade = 0; createshadow();}
		~SHADEBOX() {freeup();}
		void clear() {removeshadow();}
		void displaybox(uc a,uc b,uc c,uc d,uc e,uc f,uc g) {setbox(a,b,c,d,e,f,g); createshadow();}
};

class MenuHeader;
class keypress;

class PullDown {
		uc x, y;
		uc WC, LC, IWC, ILC, NC, INC;
		us num;
		MenuHeader **list;
		uc curx;
		uc totbreak;
		uc *fbreak;
		uc quittime;
	public:
		PullDown(uc,uc,uc,uc,uc,uc,uc,uc);
		void setcolours(uc,uc,uc,uc,uc,uc);
		void operator+=(string &);
		void operator+=(char *);
		void addsub(uc, string &, uc);
		void addsub(uc, char *, uc);
		void setenter(uc, uc, uc);
		void addsub(uc);
		void addbreak(uc);
		void display();
		void setquittime(uc a) {quittime = a;}
		void setletter(uc,uc);
		uc getx() {return curx;}
		uc gety();
		uc invoke(keypress &, uc);
		~PullDown();
};
#endif