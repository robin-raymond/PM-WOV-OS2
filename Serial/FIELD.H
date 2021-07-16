#include "keyboard.h"

#define UP_EDIT 0
#define DOWN_EDIT 1
#define ENTER_EDIT 3
#define TIME_OUT 5
#define KEY_BREAK 6

class b_convert {
	public:
		virtual void operator<<(string &)=0;
		virtual void operator>>(string &)=0;
};

class c_uc : public b_convert {
		uc *num;
	public:
		void set(uc &n) {num = &n;}
		void operator<<(string &a) {a >> (*num);}
		void operator>>(string &a) {a << (*num);}
};

class c_us : public b_convert {
		us *num;
	public:
		void set(us &n) {num = &n;}
		void operator<<(string &a) {a >> (*num);}
		void operator>>(string &a) {a << (*num);}
};

class c_ul : public b_convert {
		ul *num;
	public:
		void set(ul &n) {num = &n;}
		void operator<<(string &a) {a >> (*num);}
		void operator>>(string &a) {a << (*num);}
};


class c_sc : public b_convert {
		sc *num;
	public:
		void set(sc &n) {num = &n;}
		void operator<<(string &a) {a >> (*num);}
		void operator>>(string &a) {a << (*num);}
};


class c_si : public b_convert {
		si *num;
	public:
		void set(si &n) {num = &n;}
		void operator<<(string &a) {a >> (*num);}
		void operator>>(string &a) {a << (*num);}
};

class c_sl : public b_convert {
		sl *num;
	public:
		void set(sl &n) {num = &n;}
		void operator<<(string &a) {a >> (*num);}
		void operator>>(string &a) {a << (*num);}
};

class c_string : public b_convert {
		string *num;
	public:
		void set(string &n) {num = &n;}
		void operator<<(string &a) {a >> (*num);}
		void operator>>(string &a) {a << (*num);}
};


class c_letter : public b_convert {
		uc *letter;
	public:
		c_letter(uc &n) {letter = &n;}
		c_letter(c_letter &n) {letter = n.letter;}
		void operator=(uc &n) {letter = &n;}
		void set(c_letter &n) {letter = n.letter;}
		void operator<<(string &a) {a >> (*this);}
		void operator>>(string &a) {a << (*this);}
		friend void operator>>(string &, c_letter &);
		friend void operator<<(string &, c_letter &);
};

class INPUT {
	public:
		virtual void addbreak(uc) = 0;
		virtual uc invoke(uc, keypress &) = 0;
		virtual void settime(uc) = 0;
};

class EDITOR {
		INPUT **list;
		uc total;
		uc last;
	public:
		EDITOR() {total = 0; last = 1;}
		void addbreak(uc);
		void settime(uc);
		uc invoke(keypress &);
		void breakonlast(uc l) {last = l;}
		void operator+=(INPUT &);
		~EDITOR();
};

class BOUNCE : public INPUT {
		uc **_tag;
		us *_total;
		string ***_list;
		us top;
		us cur;
		uc x1,y1,x2,y2;
		uc WC;
		uc HC;
		uc TWC;
		uc THC;
		uc totbreak;
		uc *fbreak;
		uc quittime;
		uc allowtag;
		void display(us, uc *&, us &, string **&);
		void display(us);
		void displaylist(uc *&, us &, string **&);
		void displaylist();
	public:
		BOUNCE(uc _x1, uc _y1, uc _x2, uc _y2, uc _WC, uc _HC, us &t, string **&l)
		{
			totbreak = 0;
			allowtag = 0;
			cur = 0;
			top = 0;
			_list = &l;
			_total = &t;
			x1 = _x1;
			x2 = _x2;
			y1 = _y1;
			y2 = _y2;
			WC = _WC;
			HC = _HC;
			TWC = _WC;
			THC = _HC;
			quittime = 1;
		}
		BOUNCE(uc _x1, uc _y1, uc _x2, uc _y2, uc _WC, uc _HC, uc _TWC, uc _THC, uc *&_t, us &t, string **&l)
		{
			totbreak = 0;
			allowtag = 1;
			cur = 0;
			top = 0;
			_list = &l;
			_total = &t;
			_tag = &_t;
			x1 = _x1;
			x2 = _x2;
			y1 = _y1;
			y2 = _y2;
			WC = _WC;
			HC = _HC;
			TWC = _TWC;
			THC = _THC;
			quittime = 1;
		}
		void addbreak(uc);
		void settime(uc t) {quittime = t;}
		us getcurrent() {return cur;}
		uc usecurrent();
		void clear();
		uc invoke(uc n, keypress &key) {return invoke(n,key,*_tag,*_total,*_list);}
		uc invoke(uc, keypress &key,uc *&,us &, string **&);
		~BOUNCE() {clear();}
};

class TOGGLE : public INPUT {
		uc total;
		uc **list;
		uc x;
		uc y;
		uc lb, rb, check;
		uc WC;
		uc SC;
		uc totbreak;
		uc *fbreak;
		uc quittime;
		void display();
		void display(uc);
	public:
		TOGGLE(uc,uc,uc,uc,uc,uc,uc);
		void addbreak(uc);
		void operator+=(uc &);
		void settime(uc t) {quittime = t;}
		uc invoke(uc, keypress &);
		void clear();
		~TOGGLE() {clear();}
};

class BUTTON : public INPUT {
		uc total;
		string **list;
		uc *selection;
		uc *x;
		uc *y;
		uc *WC;
		uc *SC;
		uc *BG;
		uc siz;
		uc pos;
		uc totbreak;
		uc *fbreak;
		uc quittime;
		void display();
		void display(uc);
		void display_blank(uc n) {if(*selection==n) {uc tmp = *selection; *selection = !*selection; display(n); *selection = tmp;} else display(n);}
		void display_blank();
	public:
		BUTTON(uc &a, uc b) {selection = &a; siz = b; totbreak = total = 0; quittime = 1;}
		void addbreak(uc);
		void add(uc,uc,uc,uc,uc,char *);
		void settime(uc t) {quittime = t;}
		uc invoke(uc, keypress &);
		uc getpos() {return pos;}
		void clear();
		~BUTTON() {clear();}
};

class SELECT : public INPUT {
		uc total;
		uc *list;
		uc x;
		uc y;
		uc lb, rb, check;
		uc WC;
		uc SC;
		uc totbreak;
		uc *fbreak;
		uc quittime;
		void display();
	public:
		SELECT(uc,uc,uc,uc,uc,uc,uc,uc,uc &);
		void addbreak(uc);
		void settime(uc t) {quittime = t;}
		uc invoke(uc, keypress &);
		void clear();
		~SELECT() {clear();}
};

class FIELD : public INPUT {
		uc total;
		b_convert **val;
		uc *x1, *y1;
		uc *siz;
		uc *WC, *DC;
		uc *retauto;
		uc *def;
		uc *sec;
		uc totbreak;
		uc *fbreak;
		uc quittime;
		uc field;
		void do_addfield(b_convert *, uc, uc, uc, uc, uc, uc, uc, uc);
	public:
		FIELD() {totbreak = field = total = 0; quittime = 1;}
		void addbreak(uc);
		void clearbreak();
		void redisplay();
		uc invoke(uc, keypress &);
		void settime(uc t) {quittime = t;}
		void addfield(uc &, uc, uc, uc, uc, uc, uc, uc, uc);
		void addfield(us &, uc, uc, uc, uc, uc, uc, uc, uc);
		void addfield(ul &, uc, uc, uc, uc, uc, uc, uc, uc);
		void addfield(sc &, uc, uc, uc, uc, uc, uc, uc, uc);
		void addfield(si &, uc, uc, uc, uc, uc, uc, uc, uc);
		void addfield(sl &, uc, uc, uc, uc, uc, uc, uc, uc);
		void addfield(string &, uc, uc, uc, uc, uc, uc, uc, uc);
		void addfield(c_letter &, uc, uc, uc, uc, uc, uc, uc, uc);
		void clear();
		~FIELD() {clear();}
};

class LIST : public INPUT {
		us *_total_;
		string ***_list_;
		uc x1, y1;
		uc x2, y2;
		uc WC;
		uc totbreak;
		uc *fbreak;
		uc quittime;
		us top;
		us cur;
		us shift;
		void redisplay();
		void redisplaylist();
	public:
		LIST(uc _x1,uc _y1,uc _x2,uc _y2,uc _WC, us &_total, string **&_list) {
				shift = top = totbreak = 0;
				quittime = 1;
				x1 = _x1;
				y1 = _y1;
				x2 = _x2;
				y2 = _y2;
				WC = _WC;
				_total_ = &_total;
				_list_ = &_list;
				redisplaylist();
		}
		void addbreak(uc);
		uc invoke(uc a, keypress &b) {return invoke(a,b,*_total_,*_list_);}
		uc invoke(uc, keypress &, us &, string **&);
		void settime(uc t) {quittime = t;}
		void clear();
		~LIST() {clear();}
};
