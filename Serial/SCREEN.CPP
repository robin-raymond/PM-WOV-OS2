/***
*
* Contains screen, pull down menu functions
*
*****************************************************************************/
#include "all.h"
#include "alloc.h"
#include "string.h"
#include "keyboard.h"
#include "screen.h"
#include <time.h>

uc columns = 80;
uc rows = 25;
us far *base = 0;
uc monomode = 0;

uc tline[40][12] = {{197,180,195,179,179,179,216,181,198,186,186,186},
						  {197,180,197,180,180,180,216,181,216,182,182,182},
						  {197,180,197,181,181,181,216,181,216,185,185,185},
						  {215,182,215,180,180,180,206,185,206,182,182,182},
						  {210,183,210,180,191,180,203,187,203,182,183,182},
						  {194,191,194,181,184,181,209,184,209,185,187,185},
						  {215,182,215,181,181,181,206,185,206,185,185,185},
						  {215,182,199,179,179,179,206,185,204,186,186,186},
						  {210,183,210,181,184,181,203,187,203,185,187,185},
						  {208,189,208,181,181,190,202,188,202,185,185,188},
						  {208,189,208,180,180,217,202,188,202,182,182,189},
						  {193,217,193,181,181,190,207,190,207,185,185,188},
						  {194,191,194,180,191,180,209,184,209,182,183,182},
						  {193,193,192,195,195,192,207,207,212,199,199,211},
						  {193,193,193,197,197,193,207,207,207,215,215,208},
						  {194,194,194,197,194,197,209,209,209,215,210,215},
						  {197,197,195,195,195,195,216,216,198,199,199,199},
						  {196,196,196,197,194,193,205,205,205,215,210,208},
						  {197,197,197,197,197,197,216,216,216,215,215,215},
						  {197,197,195,198,198,198,216,216,198,204,204,204},
						  {215,215,199,195,195,195,206,206,204,199,199,199},
						  {208,208,211,198,198,212,202,202,200,204,204,200},
						  {210,210,214,198,213,198,203,203,201,204,201,204},
						  {208,208,208,216,216,207,202,202,202,206,206,202},
						  {210,210,210,216,209,216,203,203,203,206,203,206},
						  {215,215,199,198,198,198,206,206,204,204,204,204},
						  {196,196,196,216,209,207,205,205,205,206,203,202},
						  {215,215,215,216,216,216,206,206,206,206,206,206},
						  {193,193,193,216,216,207,207,207,207,206,206,202},
						  {208,208,208,197,197,193,202,202,202,215,215,208},
						  {194,194,194,216,209,216,209,209,209,206,203,206},
						  {210,210,210,197,194,197,203,203,203,215,210,215},
						  {208,208,211,195,195,192,202,202,200,199,199,211},
						  {193,193,192,198,198,212,207,207,212,204,204,200},
						  {194,194,218,198,213,198,209,209,213,204,201,204},
						  {210,210,214,195,218,195,203,203,201,199,214,199},
						  {215,215,215,197,197,197,206,206,206,215,215,215},
						  {197,197,197,216,216,216,216,216,216,206,206,206},
						  {193,217,193,180,180,217,207,190,207,182,182,189},
						  {194,194,218,195,218,195,209,209,213,199,214,199}};

uc lined[4][6] = {{218, 196, 191, 179, 192, 217},{201, 205, 187, 186, 200, 188},
		 {213, 205, 184, 179, 212, 190},{214, 196, 183, 186, 211, 189}};
				//LT, RT, UP, DN, SCL, SCB, MX
uc winc[7] = {17, 16, 30, 31, 177, 254, 23};

#define UP_EDIT 0
#define DOWN_EDIT 1
#define ENTER_EDIT 3
#define TIME_OUT 5
#define KEY_BREAK 6

void SHADEBOX::createshadow()
{
	uc need = shade = 0;
	us *pos, i;
	us far *dest;
	if(x2 < columns - 1)
		need += y2 - y1;
	if(y2 < rows - 1)
		need += x2 - x1;
	if((x2 < columns - 1)&&(y2 < rows - 1))
		need += 1;
	if((need)&&(rem))
	{
		pos = shadow = (us *)_alloc(need*sizeof(us));
		if(x2 < columns - 1)
		{
			dest = base + ((y1 + 1) * columns) + x2 + 1;
			for(i = y1 + 1; i <= y2; ++i, ++pos, dest += columns)
			{
				_check(shadow,pos);
				*pos = *dest;
			}
		}
		if(y2 < rows - 1)
		{
			dest = base + ((y2 + 1) * columns) + x1 + 1;
			for(i = x1 + 1; i <= x2; ++i, ++pos, ++dest)
			{
				_check(shadow,pos);
				*pos  = *dest;
			}
		}
		if((x2 < columns - 1)&&(y2 < rows - 1))
		{
			dest = base + ((y2 + 1) * columns) + x2 + 1;
			_check(shadow,pos);
			*pos  = *dest;
		}
		shade = 1;
	}
	if(x2 < columns - 1)
	{
		dest = base + ((y1 + 1) * columns) + x2 + 1;
		for(i = y1 + 1; i <= y2; ++i, dest += columns)
			*dest = (*dest & 2047);
	}
	if(y2 < rows - 1)
	{
		dest = base + ((y2 + 1) * columns) + x1 + 1;
		for(i = x1 + 1; i <= x2; ++i, ++dest)
			*dest = (*dest & 2047);
	}
	if((x2 < columns - 1)&&(y2 < rows - 1))
	{
		dest = base + ((y2 + 1) * columns) + x2 + 1;
		*dest = (*dest & 2047);
	}
}

void SHADEBOX::removeshadow()
{
	if(shade)
	{
		us *pos, i;
		us far *dest;
		pos = shadow;
		if(x2 < columns - 1)
		{
			dest = base + ((y1 + 1) * columns) + x2 + 1;
			for(i = y1 + 1; i <= y2; ++i, ++pos, dest += columns)
			{
				_check(shadow,pos);
				*dest = *pos;
			}
		}
		if(y2 < rows - 1)
		{
			dest = base + ((y2 + 1) * columns) + x1 + 1;
			for(i = x1 + 1; i <= x2; ++i, ++pos, ++dest)
			{
				_check(shadow,pos);
				*dest = *pos;
			}
		}
		if((x2 < columns - 1)&&(y2 < rows - 1))
		{
			dest = base + ((y2 + 1) * columns) + x2 + 1;
			_check(shadow,pos);
			*dest = *pos;
		}
		_free(shadow);
		shade = 0;
	}
}

void swap( uc &nb1, uc &nb2)
{
	uc tmp;
	tmp = nb1;
	nb1 = nb2;
	nb2 = tmp;

}

void BOX::setbox(uc ax1, uc ay1, uc ax2, uc ay2, uc col, uc typ, uc remember)
{
	freeup();
	uc x;
	uc y;
	us rea = col << 8;

	x1 = ax1;
	y1 = ay1;
	x2 = ax2;
	y2 = ay2;
	WC = col;
	rem = remember;
	us far *pos, far *pos2;
	if(rem)
	{
		us *sub;
		if((sub = mem = (us *)_alloc((x2-x1+1)*(y2-y1+1)*sizeof(us))) != 0)
		{
			rem = 1;
			for(y = y1; y <= y2; ++y)
			{
				pos = base + (y * columns) + x1;
				for(x = x1; x <= x2; ++x, ++pos, ++sub)
					*sub = *pos;
			}
		}
		else
			rem = 0;
	}
	pos = base + x1 + y1 * 80;
	*pos = lined[typ][0] | rea;
	pos = base + x1 + y2 * 80;
	*pos = lined[typ][4] | rea;
	pos = base + x2 + y1 * 80;
	*pos = lined[typ][2] | rea;
	pos = base + x2 + y2 * 80;
	*pos = lined[typ][5] | rea;
	rea = lined[typ][1] | (col << 8);
	pos  = base + x1 + 1 + y1 * 80;
	pos2 = base + x1 + 1 + y2 * 80;
	for(x = x1+1; x < x2; ++x,++pos,++pos2)
		*pos = *pos2 = rea;
	rea = lined[typ][3] | (col << 8);
	for(y = y1+1; y < y2; ++y)
	{
		pos  = base + x1 + y * 80;
		pos2 = base + x2 + y * 80;
		*pos = *pos2 = rea;
	}
	rea = 32 | (WC << 8);
	for(y = y1+1; y < y2; ++y)
	{
		pos = base + x1 + 1 + y * 80;
		for(x = x1 + 1; x < x2; ++x, ++pos)
			*pos = rea;
	}
}

void BOX::scroll_down(uc col)
{
	us far *sour, far *dest;
	uc x, y;
	for(y = y2 - 1; y > y1 + 1; --y)
	{
		dest = base + y * columns + x1 + 1;
		sour = base + (y-1)*columns + x1 + 1;
		for(x = x1 + 1; x < x2; ++x, ++dest, ++sour)
			*dest = *sour;
	}
	dest = base + (y1+1) * columns + x1 + 1;
	if(col > 15)
	{
		for(x = x1 + 1; x < x2; ++x, ++dest)
			*dest = (*dest & 65280) | 32;
	}
	else
	{
		us let = (col << 255) | 32;
		for(x = x1 + 1; x < x2; ++x, ++dest)
			*dest = let;
	}
}

void BOX::scroll_up(uc col)
{
	us far *sour, far *dest;
	uc x, y;
	for(y = y1 + 1; y < y2 - 1; ++y)
	{
		dest = base + y * columns + x1 + 1;
		sour = base + (y+1)*columns + x1 + 1;
		for(x = x1 + 1; x < x2; ++x, ++dest, ++sour)
			*dest = *sour;
	}
	dest = base + (y2-1) * columns + x1 + 1;
	if(col > 15)
	{
		for(x = x1 + 1; x < x2; ++x, ++dest)
			*dest = (*dest & 65280) | 32;
	}
	else
	{
		us let = (col << 255) | 32;
		for(x = x1 + 1; x < x2; ++x, ++dest)
			*dest = let;
	}
}

void BOX::scroll_right(uc col)
{
	us far *sour, far *dest;
	uc x, y;
	for(x = x2 - 1; x > x1 + 1; --x)
	{
		dest = base + (y1+1) * columns + x;
		sour = base + (y1+1) * columns + x - 1;
		for(y = y1 + 1; y < y2; ++y, dest += columns, sour += columns)
			*dest = *sour;
	}
	dest = base + (y1+1) * columns + x1 + 1;
	if(col > 15)
	{
		for(y = y1 + 1; y < y2; ++y, dest += columns)
			*dest = (*dest & 65280) | 32;
	}
	else
	{
		us let = (col << 255) | 32;
		for(y = y1 + 1; y < y2; ++y, dest += columns)
			*dest = let;
	}
}

void BOX::scroll_left(uc col)
{
	us far *sour, far *dest;
	uc x, y;
	for(x = x1 + 1; x < x2 - 1; ++x)
	{
		dest = base + (y1+1) * columns + x;
		sour = base + (y1+1) * columns + x + 1;
		for(y = y1 + 1; y < y2; ++y, dest += columns, sour += columns)
			*dest = *sour;
	}
	dest = base + (y1+1) * columns + x2 - 1;
	if(col > 15)
	{
		for(y = y1 + 1; y < y2; ++y, dest += columns)
			*dest = (*dest & 65280) | 32;
	}
	else
	{
		us let = (col << 255) | 32;
		for(y = y1 + 1; y < y2; ++y, dest += columns)
			*dest = let;
	}
}


void BOX::freeup_base()
{
	if(rem)
	{
		us far *pos;
		us *sub = mem;
		uc y, x;
		rem = 1;
		for(y = y1; y <= y2; ++y)
		{
			pos = base + x1 + (y * columns);
			for(x = x1; x <= x2; ++x, ++pos, ++sub)
				*pos = *sub;
		}
		rem = 0;
		_free(mem);
	}
}

void setmode(uc mode)
{
	uc m;
	columns = 80;
	asm { mov ah,0xf;
			int 0x10;
			mov m,al};
	if(m == 7)
	{
		asm {	mov ah,0;
				mov al,7;
				int 0x10};
		rows = 25;
		base = (us far *)MONMEM;
		monomode = 1;
	}
	else
	{
		if(mode != R25)
		{
			if(mode == R50)
			{
				asm { mov al,2;
						mov ah,0x12;
						mov bl,0x30;
						int 0x10;
						mov m,al};
				if(m == 0x12)
					rows = 50;
				else
					mode = R43;
			}
			if(mode == R43)
			{
				asm {	mov al,1;
						mov ah,0x12;
						mov bl,0x30;
						int 0x10;
						mov m,al};
				if(m == 0x12)
					rows = 43;
				else
					rows = 25;
			}
		}
		else
			rows = 25;
		asm { mov ah,0;
				mov al,3;
				int 0x10};
		if(rows > 25)
		{
			asm { mov ah,0x11;
					mov al,0x12;
					mov bl,0;
					int 0x10};
		}
		base = (us far *)CGAMEM;
		monomode = 0;
	}
}

SETUPSCREEN::SETUPSCREEN()
{
	setmode(R25);
}

void SETUPSCREEN::cleanscreen()
{
	setmode(R25);
}

void textline(uc sta, uc sto, uc at, uc col, uc dir)
{
	uc x;
	us rea = col << 8;
	us far *pos;

	if(dir == HORZ)
		pos = base + at * 80 + sta - 1;
	for(x = sta; x <= sto; ++x)
	{
		if(dir == VERT)
			pos = base + x * 80 + at;
		else
			++pos;
		*pos = ((uc)*pos) | rea;
	}
}

void textline(uc sta, uc sto, uc at, uc bord, uc dir, uc cross)
{
	if(sta > sto)
		swap(sta, sto );

	us far *pos;
	uc under;
	us hole;
	uc i;
	uc def;
	uc over;

	if(!dir)
		pos = base + at * 80 + sta - cross;
	def = lined[bord][dir*2+1];
	for(i = sta + !cross; i < sto + cross; ++i)
	{
		if(dir == VERT)
			pos = base + i * 80 + at;
		else
			++pos;
		hole  = *pos;
		under = (uc)hole;
		if((under >= 179)&&(under <= 218))
			over = tline[under-179][3*dir+6*bord];
		else
			over = def;
		*pos = ((hole >> 8) << 8) | over;
	}
	if(!cross)
	{
		if(dir == HORZ)
			pos = base + at * 80 + sta;
		else
			pos = base + sto * 80 + at;
		if((((uc)*pos) >= 179)&&(((uc)*pos) <= 218))
			over = tline[((uc)*pos)-179][3*dir+6*bord+2];
		else
			over = def;
		*pos = ((*pos >> 8) << 8) | over;
		if(dir == HORZ)
			pos = base + at * 80 + sto;
		else
			pos = base + sta * 80 + at;
		if((((uc)*pos) >= 179)&&(((uc)*pos) <= 218))
			over = tline[((uc)*pos)-179][3*dir+6*bord+1];
		else
			over = def;
		*pos = ((*pos >> 8) << 8) | over;
	}
}

us getblock(uc x, uc y)
{
	return *(base + (y * 80) + x);
}

void setblock(uc x, uc y, us block)
{
	*(base + (y * 80) + x) = block;
}


void _fillpattern(uc x1, uc y1, uc x2, uc y2, uc let, uc col)
{
	if(x1 >= columns)
		return;
	if(x2 >= columns)
		return;
	if(y1 >= rows)
		return;
	if(y2 >= rows)
		return;
	us block = let | (((uc)col)<<8);
	us far *pos;
	uc x, y;
	for(y = y1; y <= y2; ++y)
	{
		pos = base + (y * 80) + x1;
		for(x = x1; x <= x2; ++x, ++pos)
			*pos = block;
	}
}

void _backcolour(uc x1, uc y1, uc x2, uc y2, uc col)
{
	if(x1 >= columns)
		return;
	if(x2 >= columns)
		return;
	if(y1 >= rows)
		return;
	if(y2 >= rows)
		return;
	us block = (((us)col) << 12);
	us far *pos;
	uc x, y;
	for(y = y1; y <= y2; ++y)
	{
		pos = base + (y * 80) + x1;
		for(x = x1; x <= x2; ++x, ++pos)
			*pos = ((*pos) & 4095) | block;
	}
}

void _dpa(uc x, uc y, string &any, uc WC, uc maxsize)
{
	if(y >= rows)
		return;
	if(x + maxsize > columns)
		maxsize = columns - x;
	us let = WC << 8;
	us far *pos = base + (y * 80) + x;
	us max = !any;
	us left = 0;
	if(max > maxsize)
		max = maxsize;
	else
		left = maxsize - max;
	uc *sour = any.display();
	for(; max; --max, ++pos, ++sour)
		*pos = *sour | let;
	let = (WC << 8) | 32;
	for(; left; --left, ++pos, ++sour)
		*pos = let;
}

void _dpa(uc x, uc y, char *any, uc WC, uc maxsize)
{
	if(y > rows - 1)
		return;
	if(x + maxsize > columns)
		maxsize = columns - x;
	us let = WC << 8;
	us far *pos = base + (y * 80) + x;
	us max = 0;
	uc *sour = (uc *)any;
	for(; *sour; ++sour, ++max);
	us left = 0;
	if(max > maxsize)
		max = maxsize;
	else
		left = maxsize - max;
	sour = (uc *)any;
	for(; max; --max, ++pos, ++sour)
		*pos = *sour | let;
	let = (WC << 8) | 32;
	for(; left; --left, ++pos, ++sour)
		*pos = let;
}

void _dpa(uc x, uc y, string &any, uc maxsize)
{
	if(y > rows - 1)
		return;
	if(x + maxsize > columns)
		maxsize = columns - x;
	us far *pos = base + (y * 80) + x;
	us max = !any;
	us left = 0;
	if(max > maxsize)
		max = maxsize;
	else
		left = maxsize - max;
	uc *sour = any.display();
	for(; max; --max, ++pos, ++sour)
		*pos = *sour | (*pos & 65280);
	for(; left; --left, ++pos, ++sour)
		*pos = 32 | (*pos & 65280);
}

void _dpa_overlay(uc x, uc y, string &any, uc WC, uc maxsize)
{
	if(y > rows - 1)
		return;
	if(x + maxsize > columns)
		maxsize = columns - x;
	us let = WC << 8;
	us far *pos = base + (y * 80) + x;
	us max = !any;
	if(max > maxsize)
		max = maxsize;
	uc *sour = any.display();
	for(; max; --max, ++pos, ++sour)
	{
		if(*sour != ' ')
			*pos = *sour | let;
	}
}

void cursor(uc top,uc bottom)
{
	asm {
		mov ch,top;
		mov cl,bottom;
		mov ah,1;
		int 0x10;
	}
}

void cursorat(uc x, uc y)
{
	asm {
		mov ah,2;
		mov bh,0;
		mov dh,y;
		mov dl,x;
		int 0x10;
	}
}

void upplet(uc &let)
{
	if((let >= 'a')&&(let <= 'z'))
		let &= 223;
}

class MenuItem {
	protected:
		uc x, y;
		uc WC, LC, IWC, ILC, NC, INC;
		uc siz;
	public:
		void setcolor(uc a, uc b, uc c, uc d, uc e, uc f) {WC = a; LC = b; IWC = c; ILC = d; NC = e; INC = f;}
		void setxy(uc _x, uc _y) {x = _x; y = _y;}
		void setcolours(uc,uc,uc,uc,uc,uc);
		virtual uc skip() = 0;
		virtual void display(uc) = 0;
		virtual uc getenter() = 0;
		virtual void clean(MenuItem *) = 0;
		virtual void setenter(uc) = 0;
		virtual uc getletter() = 0;
};

void MenuItem::setcolours(uc _WC, uc _LC, uc _IWC, uc _ILC, uc _NC, uc _INC)
{
	WC = _WC;
	LC = _LC;
	IWC = _IWC;
	ILC = _ILC;
	NC = _NC;
	INC = _INC;
}

class MenuSkipper : public MenuItem {
	public:
		MenuSkipper(uc c) {siz = c;}
		uc skip() {return 1;}
		void display(uc);
		void clean(MenuItem *a) {_remove(a); delete (MenuSkipper *)a;}
		uc getenter() {return 0;}
		void setenter(uc) {;}
		uc getletter() {return 0;}
};

void MenuSkipper::display(uc)
{
	textline(x-2,x+siz+1,y,SINGLE,HORZ,FALSE);
}

class MenuSelection : public MenuItem {
		string title;
		uc enter;
		uc letter;
	public:
		MenuSelection(string &a, uc b, uc c) {title = a; letter = b; enter = 1; siz = c;}
		uc skip() {return 0;}
		uc getenter() {return enter;}
		void setenter(uc a) {enter = a;}
		void clean(MenuItem *a) {_remove(a); delete (MenuSelection *)a;}
		void display(uc);
		uc getletter() {return *(title.display()+letter);}
		~MenuSelection() {;}
};

void MenuSelection::display(uc hig)
{
	if(hig)
	{
		if(enter)
		{
			_dpa(x,y,"",IWC,1);
			_dpa(x+1,y,title,IWC,siz+1);
			_dpa(x+1+letter,y,(char *)title.display()+letter,ILC,1);
		}
		else
		{
			_dpa(x,y,"",INC,1);
			_dpa(x+1,y,title,INC,siz+1);
		}
	}
	else
	{
		if(enter)
		{
			_dpa(x,y,"",WC,1);
			_dpa(x+1,y,title,WC,siz+1);
			_dpa(x+1+letter,y,(char *)title.display()+letter,LC,1);
		}
		else
		{
			_dpa(x,y,"",NC,1);
			_dpa(x+1,y,title,NC,siz+1);
		}
	}
}

class MenuHeader {
		string title;
		uc x, y;
		uc WC, LC, IWC, ILC, NC, INC;
		uc num;
		uc siz;
		uc cury;
		uc letter;
		MenuItem **list;
		void display(uc);
	public:
		MenuHeader(string &tmp) {title = tmp; num = 0; siz = 0; cury = 0;}
		void setxy(uc _x, uc _y) {x = _x; y = _y;}
		void operator<<(MenuHeader &op2) {x = op2.x + (!(op2.title)) + 2; y = op2.y;}
		void setcolor(uc a, uc b, uc c, uc d, uc e, uc f) {WC = a; LC = b; IWC = c; ILC = d; NC = e; INC = f;}
		void setcolours(uc,uc,uc,uc,uc,uc);
		void addsub(string &, uc);
		void addsub();
		void display();
		void displayheader(uc);
		void setenter(uc a, uc b) {_check(list,list + a); (**(list + a)).setenter(b);}
		void setletter(uc a) {letter = a;}
		uc getletter() {return letter;}
		uc getsize() {return siz;}
		uc getcury() {return cury;}
		uc getx() {return x;}
		uc gettotal() {return num;}
		uc getenter() {_check(list,list + cury); return (**(list+cury)).getenter();}
		void operator++();
		void operator--();
		uc operator=(uc);
		void home();
		void end();
		~MenuHeader();
};

void MenuHeader::setcolours(uc _WC, uc _LC, uc _IWC, uc _ILC, uc _NC, uc _INC)
{
	WC = _WC;
	LC = _LC;
	IWC = _IWC;
	ILC = _ILC;
	NC = _NC;
	INC = _INC;
	if(num)
	{
		us tmp;
		for(tmp = 0; tmp < num; ++tmp)
		{
			_check(list, list + tmp);
			(**(list + tmp)).setcolours(WC,LC,IWC,ILC,NC,INC);
		}
	}
}


void MenuHeader::home()
{
	(**(list + cury)).display(0);
	cury = num - 1;
	++(*this);
}

void MenuHeader::end()
{
	(**(list + cury)).display(0);
	cury = 0;
	--(*this);
}

uc MenuHeader::operator=(uc let)
{
	uc tmp = 0;
	uc found = 0;
	if((let >= 'A')&&(let <= 'Z'))
		let |= 32;
	for(; tmp < num; ++tmp)
	{
		_check(list,list+tmp);
		uc let2 = (**(list + tmp)).getletter();
		if((let2 >= 'A')&&(let2 <= 'Z'))
			let2 |= 32;
		if(let == let2)
		{
			_check(list,list+cury);
			(**(list+cury)).display(0);
			cury = tmp;
			(**(list+cury)).display(1);
			found = 1;
		}
	}
	return found;
}

void MenuHeader::operator++()
{
	_check(list, list + cury);
	uc ly = cury;
	do
	{
		++cury;
		if(cury >= num)
			cury = 0;
		_check(list, list + cury);
	} while((**(list + cury)).skip());
	(**(list + ly)).display(cury==ly);
	(**(list + cury)).display(1);
}

void MenuHeader::operator--()
{
	_check(list, list + cury);
	uc ly = cury;
	do
	{
		if(cury)
			--cury;
		else
			cury = num - 1;
		_check(list, list + cury);
	} while((**(list + cury)).skip());
	(**(list + ly)).display(cury==ly);
	(**(list + cury)).display(1);
}

void MenuHeader::display(uc pos)
{
	_check(list, list + pos);
	(**(list+pos)).display(pos==cury);
}

void MenuHeader::display()
{
	uc pos = 0;
	for(; pos < num; ++pos)
	{
		_check(list, list + pos);
		(**(list+pos)).display(pos==cury);
	}
}


void MenuHeader::displayheader(uc hig)
{
	if(hig)
	{
		_dpa(x,y,"",IWC,1);
		_dpa(x+1,y,title,IWC,!title+1);
		_dpa(x+1,y,(char *)title.display(),ILC,1);
	}
	else
	{
		_dpa(x,y,"",WC,1);
		_dpa(x+1,y,title,WC,!title+1);
		_dpa(x+1,y,(char *)title.display(),LC,1);
	}
}


void MenuHeader::addsub(string &tmp, uc highletter)
{
	if(num)
		list = (MenuItem **)_realloc(list,sizeof(MenuItem *)*(num + 1));
	else
		list = (MenuItem **)_alloc(sizeof(MenuItem *));
	_check(list,list + num);
	if((!tmp) > siz)
		siz = !tmp;
	*(list + num) = (MenuItem *)_add(new MenuSelection(tmp,highletter,siz),sizeof(MenuSelection));
	(**(list + num)).setxy(x+1,y+2+num);
	(**(list + num)).setcolor(WC,LC,IWC,ILC,NC,INC);
	++num;
}

void MenuHeader::addsub()
{
	if(num)
		list = (MenuItem **)_realloc(list,sizeof(MenuItem *)*(num + 1));
	else
		list = (MenuItem **)_alloc(sizeof(MenuItem *));
	_check(list,list + num);
	*(list + num) = (MenuItem *)_add(new MenuSkipper(siz),sizeof(MenuSkipper));
	(**(list + num)).setxy(x+2,y+2+num);
	(**(list + num)).setcolor(WC,LC,IWC,ILC,NC,INC);
	++num;
}

MenuHeader::~MenuHeader()
{
	if(num)
	{
		us tmp;
		for(tmp = 0; tmp < num; ++tmp)
		{
			_check(list, list + tmp);
			(**(list + tmp)).clean(*(list + tmp));
		}
		_free(list);
		num = 0;
	}
}

uc PullDown::gety()
{
	_check(list,list+curx);
	return (**(list + curx)).getcury();
}

void PullDown::addbreak(uc b)
{
	if(totbreak)
		fbreak = (uc *)_realloc(fbreak,sizeof(uc)*(totbreak+1));
	else
		fbreak = (uc *)_alloc(sizeof(uc));
	_check(fbreak,fbreak + totbreak);
	*(fbreak + totbreak) = b;
	++totbreak;
}

void PullDown::display()
{
	uc tmp = 0;
	for(; tmp < num; ++tmp)
	{
		_check(list,list + tmp);
		(**(list + tmp)).displayheader(0);
	}
}

void PullDown::setletter(uc a,uc l)
{
	_check(list,list+a);
	(**(list+a)).setletter(l);
}

uc PullDown::invoke(keypress &key, uc which)
{
	curx = which;
	if(!num)
		_doexit(1);
	uc reason = 0;
	uc quitting = 0;
	time_t lasttime;
	lasttime = time(NULL);
	display();
	do
	{
		uc editting = 1;
		quitting = 0;
		uc tmp;
		_check(list,list+curx);
		(**(list+curx)).displayheader(1);
		SHADEBOX main((**(list+curx)).getx(),y+1,(**(list+curx)).getx()+(**(list+curx)).getsize()+3,y+(**(list+curx)).gettotal()+2,WC,SINGLE,TRUE);
		(**(list+curx)).display();
		do
		{
			key.quick_getscan();
			if(key.scan)
			{
				lasttime = time(NULL);
				uc found = 0;
				for(tmp = 0; (!found) && (tmp < totbreak); ++tmp)
				{
					if((*(fbreak+tmp))==(key.scan))
					{
						found = 1;
						quitting = 1;
						editting = 0;
						reason = KEY_BREAK;
					}
				}
				if(!found)
				{
					if(key.status & K_ALT)
					{
						for(tmp = 0; tmp < num; ++tmp)
						{
							_check(list,list+tmp);
							if(((**(list+tmp)).getletter()) == key.asc)
							{
								if(tmp != curx)
								{
									editting = 0;
									(**(list+curx)).displayheader(0);
									curx = tmp;
								}
							}

						}
					}
					else
					{
						switch(key.scan)
						{
							case S_HOME:
										(**(list + curx)).home();
										break;
							case S_END:
										(**(list + curx)).end();
										break;
							case S_SPACE:
							case S_ENTER:
										if((**(list+curx)).getenter())
										{
											quitting = 1;
											editting = 0;
											reason = ENTER_EDIT;
										}
										break;
							case S_CUR_LEFT:
										editting = 0;
										(**(list+curx)).displayheader(0);
										if(curx)
											--curx;
										else
											curx = num - 1;
										break;
							case S_CUR_UP:
										--(**(list+curx));
										break;
							case S_CUR_RIGHT:
										editting = 0;
										(**(list+curx)).displayheader(0);
										++curx;
										if(curx >= num)
											curx = 0;
										break;
							case S_CUR_DOWN:
										++(**(list+curx));
										break;
							default:
								if(key.asc)
								{
									uc l = (**(list+curx)) = key.asc;
									if(((**(list+curx)).getenter())&&(l))
									{
										quitting = 1;
										editting = 0;
										reason = ENTER_EDIT;
									}
								}
								break;
						}
					}
				}
			}
			else
			{
				time_t curtime = time(NULL);
				if(curtime > (lasttime + (((ul)quittime) * 60)))
				{
					quitting = 1;
					editting = 0;
					reason = TIME_OUT;
				}
			}
		} while(editting);
	} while(!quitting);
	cursor(CUR_OFF);
	(**(list+curx)).displayheader(0);
	return reason;
}

void PullDown::addsub(uc a, char *b, uc c)
{
	string tmp;
	tmp = b;
	addsub(a,tmp,c);
}

void PullDown::addsub(uc pos, string &sub, uc highletter)
{
	_check(list,list+pos);
	(**(list+pos)).addsub(sub,highletter);
}

void PullDown::addsub(uc pos)
{
	_check(list,list+pos);
	(**(list+pos)).addsub();
}

void PullDown::setenter(uc pos, uc sub, uc enter)
{
	_check(list,list+pos);
	(**(list+pos)).setenter(sub,enter);
}

PullDown::PullDown(uc _x, uc _y, uc _WC, uc _LC, uc _IWC, uc _ILC, uc _NC, uc _INC)
{
	x = _x;
	y = _y;
	WC = _WC;
	LC = _LC;
	IWC = _IWC;
	ILC = _ILC;
	NC = _NC;
	INC = _INC;
	num = 0;
	curx = 0;
	quittime = 1;
	totbreak = 0;
}

void PullDown::setcolours(uc _WC, uc _LC, uc _IWC, uc _ILC, uc _NC, uc _INC)
{
	WC = _WC;
	LC = _LC;
	IWC = _IWC;
	ILC = _ILC;
	NC = _NC;
	INC = _INC;
	if(num)
	{
		us tmp;
		for(tmp = 0; tmp < num; ++tmp)
		{
			_check(list, list + tmp);
			(**(list + tmp)).setcolours(WC,LC,IWC,ILC,NC,INC);
		}
	}
	display();
}

PullDown::~PullDown()
{
	if(num)
	{
		us tmp;
		for(tmp = 0; tmp < num; ++tmp)
		{
			_check(list, list + tmp);
			_remove(*(list + tmp));
			delete *(list + tmp);
		}
		_free(list);
		num = 0;
	}
	if(totbreak)
	{
		_free(fbreak);
		totbreak = 0;
	}
}

void PullDown::operator+=(char *str)
{
	string tmp;
	tmp = str;
	*this += tmp;
}

void PullDown::operator+=(string &tmp)
{
	if(num)
		list = (MenuHeader **)_realloc(list,sizeof(MenuHeader *)*(num + 1));
	else
		list = (MenuHeader **)_alloc(sizeof(MenuHeader *));
	_check(list,list + num);
	*(list + num) = (MenuHeader *)_add(new MenuHeader(tmp),sizeof(MenuHeader));
	_pcheck(*(list + num));
	if(num)
		(**(list + num)) << (**(list + num - 1));
	else
		(**(list + num)).setxy(x,y);
	(**(list + num)).setcolor(WC,LC,IWC,ILC,NC,INC);
	++num;
}
