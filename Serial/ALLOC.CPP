#include "all.h"
#include "alloc.h"
#include "keyboard.h"
#include "process.h"

#if defined(__DEBUG__)

#include <iostream.h>
#include <process.h>
#include <string.h>

#define MAXALLOC 4000

void *allpoint[MAXALLOC];
ul allsize[MAXALLOC];
ul allline[MAXALLOC];

char call_file[50];
us call_line;
uc initted = 0;
uc go_error = 0;

class init_alloc {
		uc des;
	public:
		init_alloc();
		init_alloc(uc);
		~init_alloc();
} do_init_alloc(1);

init_alloc::init_alloc(uc)
{
	des = 1;
	if(!initted)
	{
		us loop = 0;
		for(; loop < MAXALLOC; ++loop)
			allpoint[loop] = NULL;
		initted = 1;
	}
}

init_alloc::init_alloc()
{
	des = 0;
	if(!initted)
	{
		us loop = 0;
		for(; loop < MAXALLOC; ++loop)
			allpoint[loop] = NULL;
		initted = 1;
	}
}

init_alloc::~init_alloc()
{
	if((initted)&&(des)&&(!go_error))
	{
		us loop = 0;
		for(; loop < MAXALLOC; ++loop)
		{
			if(allpoint[loop])
			{
				cout << "Didn't free: " << allline[loop] << " ";
			}
		}
		initted = 0;
	}
}

void check_init()
{
	if(!initted)
	{
		init_alloc any;
	}
}

void *_galloc(ul size,us lines, char *f)
{
	check_init();
	void *any;
	us loop = 0;
	for(; loop < MAXALLOC && allpoint[loop]; ++loop);
	if(loop < MAXALLOC)
	{
		any = farmalloc(size);
		allpoint[loop] = any;
		allsize[loop] = size;
		allline[loop] = lines;
		if(!((long)any))
		{
			cout << "NULL POINTER ALLOC: " << lines << " " << f << "\n";
			_doexit(1);
		}
	}
	else
	{
		cout << "INCREASE POINTER TABLE ALLOC: " << lines << " " << f << "\n";
		_doexit(1);
	}
	return any;
}

void *_grealloc(void *block, ul size,us lines, char *f)
{
	check_init();
	us loop = 0;
	uc found = 0;
	for(; loop < MAXALLOC && !found; ++loop)
	{
		if(allpoint[loop] == block)
		{
			found = 1;
			block = farrealloc(block,size);
			allpoint[loop] = block;
			allsize[loop] = size;
			allline[loop] = lines;
			if(!block)
			{
				cout << "NULL POINTER REALLOC: " << lines << " " << f << "\n";
				_doexit(1);
			}
		}
	}
	if(!found)
	{
		cout << "REALLOC POINTER NOT FOUND: " << lines << " " << f << "\n";
		_doexit(1);
	}
	return block;
}

void *_gadd(void *any, ul size,us lines, char *f)
{
	check_init();
	us loop = 0;
	for(; loop < MAXALLOC && allpoint[loop]; ++loop);
	if(loop < MAXALLOC)
	{
		allpoint[loop] = any;
		allsize[loop] = size;
		allline[loop] = lines;
		if(!any)
		{
			cout << "NULL POINTER ADD: " << lines << " " << f << "\n";
			_doexit(1);
		}
	}
	else
	{
		cout << "INCREASE NULL POINTER TABLE: " << lines << " " << f << "\n";
		_doexit(1);
	}
	return any;
}

void _gremove(void *any, us lines, char *f)
{
	check_init();
	us loop = 0;
	uc found = 0;
	for(; loop < MAXALLOC && !found; ++loop)
	{
		if(allpoint[loop]==any)
		{
			allpoint[loop] = 0;
			allsize[loop] = 0;
			allline[loop] = 0;
			found = 1;
		}
	}
	if(!found)
	{
		cout << "REMOVE POINTER NOT FOUND: " << lines << " " << f << "\n";
		_doexit(1);
	}
}

void _gcall(us lines, char *f)
{
	check_init();
	strcpy(call_file,f);
	call_line = lines;
}

void _gfree(void *any, us lines, char *f)
{
	check_init();
	us loop = 0;

	if(any == NULL)
	{
		cout << "NULL POINTER FREE: " << lines << " " << f << "\n";
		_doexit(1);
	}
	for(; loop < MAXALLOC && allpoint[loop] != any; ++loop);
	if(loop >= MAXALLOC)
	{
		cout << "FREE POINTER NOT FOUND: " << lines << " " << f << "\n";
		_doexit(1);
	}
	else
	{
		farfree(any);
		allpoint[loop] = 0;
		allsize[loop] = 0;
		allline[loop] = 0;
	}
}

void _gcheck(void *__base, void *any, us lines, char *f)
{
	check_init();
	us loop = 0;
	uc found = 0, found2 = 0;
	long check = 0, check2 = 0;

	if(any == NULL)
	{
		cout << "NULL POINTER CHECK: " << lines << " " << f << "\n";
		_doexit(1);
	}
	for(; !found && loop < MAXALLOC; ++loop)
	{
		if(allpoint[loop])
		{
			if(__base == allpoint[loop])
			{
				found2 = 1;
				check  = (long)any;
				check2 = (long)allpoint[loop];
				if((check >= check2)&&(check < check2 + allsize[loop]))
					found = 1;
			}
		}
	}
	if(!found2)
	{
		cout << "CHECK BASE NOT FOUND: " << lines << " " << f << "\n";
		_doexit(1);
	}
	if(!found)
	{
		cout << "CHECK ARRAY OUT OF BOUNDS: " << lines << " " << f << "\n";
		_doexit(1);
	}
}

void _acheck(void far *any, us lines, char *f)
{
	check_init();
	us loop = 0;
	uc found = 0;
	long check = 0, check2 = 0;

	if(any == NULL)
	{
		cout << "NULL POINTER CHECK 2: " << lines << " " << f << "\n";
		_doexit(1);
	}
	for(; !found && loop < MAXALLOC; ++loop)
	{
		if(allpoint[loop])
		{
			check  = (long)any;
			check2 = (long)allpoint[loop];
			if(check == check2)
				found = 1;
		}
	}
	if(!found)
	{
		cout << "CHECK BASE NOT FOUND 2: " << lines << " " << f << "\n";
		_doexit(1);
	}
}

#endif

void _doexit(uc n)
{
#if defined(__DEBUG__)
	check_init();
	go_error = 1;
#endif
	remove_kbd();
	exit(n);
}
