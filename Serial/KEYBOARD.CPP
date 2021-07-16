/***
*
* used to add new keyboard routines
*
*****************************************************************************/
#include "all.h"
#include "keyboard.h"
#include "screen.h"
#include <dos.h>
#include <conio.h>

#define _NORMAL 0
#define _SHIFT  1
#define _CTRL   2
#define _ALT    3
#define _SCTRL  4
#define _SALT   5

#define M_L 89         // max list
#define K_B_SIZE 20    // keyboard buffer size

void interrupt far (*old_intr9)();
void interrupt far (*old_break)();

void interrupt far new_key_intr();
void interrupt far new_break();

us far *_sta_buf, far *_end_buf;
ul far *_cur_buf;
uc far *_key_flags;

us sta_buf;
us end_buf;
ul cur_buf;

class keypress key_buffer[K_B_SIZE];
uc cur_key_pos;
uc last_key_pos;
uc installed_kbd = 0;
uc insert = 1;

uc keypress::insertmode()
{
	return insert;
}

void keypress::setinsert()
{
	insert = !insert;
}

						 //array is used for converting keypress into an ASCII character
						 //NOR,SHI,CTR,ALT,CRT,ALT
uc layout[M_L][6] = {{255,  0,  0,  0,  0,  0}, // 00
							{  0,  0,  0,  0,  0,  0}, // 01
							{'1','!','!','ñ','Û','Ü'},
							{'2','@','"','@','İ','Ş'},
							{'3','#','/','œ','ß','œ'},
							{'4','$','$','›','$','›'},
							{'5','%','%','˜','%','˜'},
							{'6','^','?','ª','?','ª'},
							{'7','&','&','|','&','|'},
							{'8','*','*','ı','*','ı'},
							{'9','(','(','¦','(','¦'},
							{'0',')',')','¬',')','¬'},
							{'-','_','_','«','_','«'},
							{'=','+','+','­','+','­'},
							{  0,  0,  0,  0,  0,  0}, // 0E
							{  0,  0,  0,  0,  0,  0}, // 0F
							{'q','Q','¨','', 19, 21},
							{'w','W','é','ç', 11, 12},
							{'e','E','Š','‚',  3,  4},
							{'r','R','ê','û',  5,  6},
							{'t','T', 20,'©', 20,'©'},
							{'y','Y','','š','','š'},
							{'u','U','—','£','—','£'},
							{'i','I','','¡','ï', 15},
							{'o','O','•','¢','•','¢'},
							{'p','P','','™','è','í'},
							{'[','{','^','[',  7,  8},
							{']','}','¤',']',  9, 10},
							{  0,  0,  0,  0,  0,  0}, // 1C
							{255,  0,  0,  0,  0,  0}, // 1D
							{'a','A','…',' ','®','¯'},
							{'s','S','„','ƒ',  1,  2},
							{'d','D','‰','ˆ','ù','ú'},
							{'f','F','Ÿ','à','Ÿ','à'},
							{'g','G','ò','ó','â','ó'},
							{'h','H','ö','ä','ô','õ'},
							{'j','J','','–','','–'},
							{'k','K','‹','Œ','‹','Œ'},
							{'l','L','”','“','”','“'},
							{';',':',':','~',':','~'},
							{ 39,'"','`','{','`','{'},
							{'`','~','|', 92,'|', 92},
							{255,  0,  0,  0,  0,  0}, // 2A
							{ 92,'|','>','}','>','}'},
							{'z','Z','†','','†',''},
							{'x','X','‘','’','î','’'},
							{'c','C','‡','€','ø', 28},
							{'v','V','÷','ğ','÷','ğ'},
							{'b','B','á','ã','ş', 22},
							{'n','N','¤','¥', 13, 14},
							{'m','M',127,'æ','ë','°'},
							{',','<', 39,'§','±','²'},
							{'.','>','.','ì','Ñ','Ï'},
							{'/','?','','¡','Ç','¶'},
							{255,  0,  0,  0,  0,  0}, // 36
							{'*','*','Ñ','Ï','Ç','¶'},
							{255,  0,  0,  0,  0,  0}, // 38
							{' ',' ',  0,  0,  0,  0},
							{255,  0,  0,  0,  0,  0}, // 3A
							{  0,  0,  0,  0,  0,  0}, // 3B
							{  0,  0,  0,  0,  0,  0}, // 3C
							{  0,  0,  0,  0,  0,  0}, // 3D
							{  0,  0,  0,  0,  0,  0}, // 3E
							{  0,  0,  0,  0,  0,  0}, // 3F
							{  0,  0,  0,  0,  0,  0}, // 40
							{  0,  0,  0,  0,  0,  0}, // 41
							{  0,  0,  0,  0,  0,  0}, // 42
							{  0,  0,  0,  0,  0,  0}, // 43
							{  0,  0,  0,  0,  0,  0}, // 44
							{  0,  0,  0,  0,  0,  0}, // 45
							{255,  0,  0,  0,  0,  0}, // 46
							{  0,'7','Ú','É','Ö','Â'}, // 47
							{  0,'8','³','º', 30, 24}, // 48
							{  0,'9','¿','»','·','Á'}, // 49
							{'-','-','Ò','Ğ','Æ','µ'}, // 4A
							{  0,'4','Ä','Í', 17, 27}, // 4B
							{  0,'5', 23, 23, 29, 18}, // 4C
							{  0,'6','Å','Î', 16, 26}, // 4D
							{'+','+','Å','Î','Å','Î'}, // 4E
							{  0,'1','À','È','Ó','Ã'}, // 4F
							{  0,'2','×','Ø', 31, 25}, // 50
							{  0,'3','Ù','¼','½','´'}, // 51
							{  0,'0','Õ','¸','Ë','Ê'}, // 52
							{  0,'.','Ô','¾','Ì','¹'}, // 53
							{255,  0,  0,  0,  0,  0}, // 54
							{255,  0,  0,  0,  0,  0}, // 55
							{'®','¯', 19,'ø', 19,'ø'}, // 56
							{  0,  0,  0,  0,  0,  0}, // 57
							{  0,  0,  0,  0,  0,  0}}; // 58

class install_keyboard {
	public:
		install_keyboard() {install_kbd();}
		friend void install_kbd();
		friend void remove_kbd();
		~install_keyboard() {remove_kbd();}
} do_install_keyboard;


void install_kbd()  // installs the new keyboard driver
/***
* Function name: install_keyboard
*
* Parameters: none
*
* Returns: nothing
*
* Purpose: installs new keyboard routines
*
*****************************************************************************/
{
	while(kbhit())
	{
		getch();
	}
	_sta_buf = (us far *)0x00400080;
	_end_buf = (us far *)0x004000082;
	_cur_buf = (ul far *)0x0040001a;
	_key_flags = (uc far *)0x00400017;
	sta_buf = *_sta_buf;
	end_buf = *_end_buf;
	cur_buf = *_cur_buf;
	cur_key_pos = 0;
	last_key_pos = 0;
	old_break = (void interrupt (far *)())getvect(0x1b);
	setvect(0x1b,(void interrupt (far*)(...))new_break);
	old_intr9 = (void interrupt (far *)())getvect(9);
	setvect(9,(void interrupt (far*)(...))new_key_intr);
	installed_kbd = 1;
}

void remove_kbd() // removes the new keyboard driver
/***
* Function name: remove_keyboard
*
* Parameters: none
*
* Returns: nothing
*
* Purpose: removes new keyboard routines
*
*****************************************************************************/
{
	if(installed_kbd)
	{
		setvect(9,(void interrupt (far*)(...))old_intr9);
		setvect(0x1b,(void interrupt (far*)(...))old_break);
		installed_kbd = 0;
	}
	while(kbhit())
	{
		getch();
	}
}

void interrupt far new_break()  // break command routed here and does nothing
/***
* Function name: new_break
*
* Parameters: none
*
* Returns: nothing
*
* Purpose: cause the break key to do nothing
*
*****************************************************************************/
{
}

void interrupt far new_key_intr()
/***
* Function name: new_key_intr
*
* Parameters: none
*
* Returns: nothing
*
* Purpose: translates keypresses as they are pressed
*
*****************************************************************************/
{
	(old_intr9)();
	uc key = inportb(96);
	*_sta_buf = sta_buf;
	*_end_buf = end_buf;
	*_cur_buf = cur_buf;
	if(key >= M_L)
		return;
	if(layout[key][0] == 255)
		return;
	uc flags = *_key_flags;
	uc act_flags = 0;
	uc what      = _NORMAL;
	if((flags & 32)&&(key >= 0x47)&&(key <= 0x53))
		what = _SHIFT;
	if(flags & 64)
	{
		uc n_key = layout[key][0];
		if((n_key >= 'a')&&(n_key <= 'z'))
			what = _SHIFT;
	}
	if((flags & 1)||(flags & 2))
	{
		act_flags = K_SHIFT;
		what = !what;
	}
	if(flags & 4)
	{
		act_flags = K_CTRL;
		what = _CTRL;
		if(flags & 16)
			what = _SCTRL;
	}
	if(flags & 8)
	{
		act_flags = K_ALT;
		what = _ALT;
		if(flags & 16)
			what = _SALT;
	}
	key_buffer[last_key_pos].status = act_flags;
	key_buffer[last_key_pos].scan   = key;
	key_buffer[last_key_pos].asc    = layout[key][what];
	++last_key_pos;
	if(last_key_pos >= K_B_SIZE)
		last_key_pos = 0;
	if(last_key_pos == cur_key_pos)  // keyboard buffer full?
	{
		if(last_key_pos == 0)
			last_key_pos = K_B_SIZE - 1;
		else
			--last_key_pos;
	}
}

void keypress::quick_getscan()
/***
* Function name: quick_getscan
*
* Parameters: none
*
* Returns: nothing
*
* Purpose: get one key press out of the new buffer, no waiting
*
*****************************************************************************/
{
	if(cur_key_pos != last_key_pos)
	{
		*this = key_buffer[cur_key_pos];
		if(cur_key_pos + 1 >= K_B_SIZE)
			cur_key_pos = 0;
		else
			++cur_key_pos;
		return;
	}
	reset();
}

void keypress::getscan()
/***
* Function name: getscan
*
* Parameters: none
*
* Returns: nothing
*
* Purpose: get one key press out of the new buffer, waits for key to arrive
*
*****************************************************************************/
{
	while (cur_key_pos == last_key_pos) {;}
	*this = key_buffer[cur_key_pos];
	if(cur_key_pos + 1 >= K_B_SIZE)
		cur_key_pos = 0;
	else
		++cur_key_pos;
}

void keypress::operator=(keypress &k)
{
	asc = k.asc;
	scan = k.scan;
	status = k.status;
}