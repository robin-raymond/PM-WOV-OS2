#if !defined(__KEYBOARD__)
#define __KEYBOARD__

#define K_SHIFT 1
#define K_CTRL  2
#define K_ALT   4

void install_kbd();
void remove_kbd();

enum K_SCAN {
	S_ESC = 1, S_BACK_SPACE = 0xE, S_TAB, S_ENTER = 0x1C, S_SPACE = 0x39,
	S_F1 = 0x3B, S_F2, S_F3, S_F4, S_F5, S_F6, S_F7, S_F8, S_F9, S_F10,
	S_HOME = 0x47, S_CUR_UP, S_PG_UP, S_CUR_LEFT = 0x4B, S_NO_MOVE, S_CUR_RIGHT,
	S_END = 0x4F, S_CUR_DOWN, S_PG_DOWN, S_INSERT, S_DELETE,
	S_F11 = 0x57, S_F12
};

class keypress {
	public:
		uc asc;
		uc scan;
		uc status;
		keypress() {reset();}
		void reset() {asc = 0; scan = 0; status = 0;}
		void quick_getscan();
		void getscan();
		void operator=(keypress &);
		uc insertmode();
		void setinsert();
};

class mouse {
		uc installed;
		int x;
		int y;
		uc m;
		uc b;
	public:
		mouse();
		void motion(keypress &);
		void getxy(uc &, uc &);
		void limit(uc,uc,uc,uc);
		void at(us,us);
		void nomotion();
		void motionoff() {m = 0; x = y = 0; nomotion();}
		void motionon() {m = 1; x = y = 0; nomotion();}
		void setpos(uc,uc);
};

extern mouse _mouse;


#endif