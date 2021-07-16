#include "all.h"
#include "nfile.h"
#include <dos.h>
#include <mem.h>
#include <stdlib.h>
#include "alloc.h"
#include "string.h"

us nrename(string &str1, string &str2)
{
	uc error;
	struct REGPACK regs;

	regs.r_ax = (0x56 << 8) | 0x56;
	regs.r_ds = FP_SEG(str1.display());
	regs.r_dx = FP_OFF(str1.display());
	regs.r_es = FP_SEG(str2.display());
	regs.r_di = FP_OFF(str2.display());
	intr(0x21,&regs);
	if(regs.r_flags & 1)
		error = regs.r_ax;
	else
	error = 0;
	return error;
}

uc networknum()
{
	char str[16];
	struct REGPACK regs;

	*str = 0;
	regs.r_ax = (0x5e << 8);
	regs.r_ds  = FP_SEG(str);
	regs.r_dx = FP_OFF(str);
	regs.r_es  = FP_SEG(str);
	regs.r_di = FP_OFF(str);
	intr(0x21,&regs);
	return ((uc)regs.r_cx);
}

ul nspace(uc drive)
{
	union REGS regs;
	struct SREGS sregs;
	ul space;

	regs.h.ah = 0x36;
	regs.h.dl = drive+1;
	int86x(0x21,&regs,&regs,&sregs);
	if(regs.x.ax == 0xFFFF)
		space = 0;
	else
		space = regs.x.bx * regs.x.ax * regs.x.cx;
	return space;
}

uc validdrive(uc drive)
{
	union REGS regs;
	struct SREGS sregs;

	regs.h.ah = 0x32;
	regs.h.dl = drive+1;
	int86x(0x21,&regs,&regs,&sregs);
	if((regs.x.ax & 255) == 0xFF)
		return 0;
	return 1;
}

us nmkdir(string &str)
{
	return ndir(str,MKDIR);
}

us nrmdir(string &str)
{
	return ndir(str,RMDIR);
}

us nchdir(string &str)
{
	return ndir(str,CHDIR);
}

us ngetdirectory(string &str, uc drive)
{
	str = 120;
	str += "A:\\";
	*(str.display()) = drive + 'A';
	union REGS regs;
	struct SREGS sregs;
	uc error;

	regs.h.ah = 0x47;
	regs.h.dl = drive + 1;
	sregs.ds  = FP_SEG(str.display()+3);
	regs.x.si = FP_OFF(str.display()+3);
	int86x(0x21,&regs,&regs,&sregs);
	if(regs.x.flags & 1)
		error = regs.x.ax;
	else
		error = 0;
	return error;
}

us ndir(string &str, uc what)
{
	union REGS regs;
	struct SREGS sregs;
	uc error;

	regs.h.ah = 0x39 + what;
	sregs.ds  = FP_SEG(str.display());
	regs.x.dx = FP_OFF(str.display());
	int86x(0x21,&regs,&regs,&sregs);
	if(regs.x.flags & 1)
		error = regs.x.ax;
	else
		error = 0;
	return error;
}

us nerase(string &str)
{
	union REGS regs;
	struct SREGS sregs;
	uc error;

	regs.h.ah = 0x41;
	sregs.ds  = FP_SEG(str.display());
	regs.x.dx = FP_OFF(str.display());
	int86x(0x21,&regs,&regs,&sregs);
	if(regs.x.flags & 1)
		error = regs.x.ax;
	else
		error = 0;
	return error;
}

ul NFILE::nseek(long pos,uc method)
{
	union REGS regs;
	ul curpos;

	regs.h.ah = 0x42;
	regs.h.al = method;
	regs.x.bx = handle;
	regs.x.cx = (unsigned)(pos >> 16);
	regs.x.dx = (unsigned)pos;
	int86(0x21,&regs,&regs);
	if(regs.x.flags & 1)
	{
		exterror = regs.x.ax;
		curpos   = 0;
	}
	else
	{
		curpos = (((ul)regs.x.dx) << 8) | regs.x.ax;
		exterror = 0;
	}
	return curpos;
}

us NFILE::n_read(void *buf, us size, us elements)
{
	us n = 0;
	exterror = 0;
	if(size * elements > 0)
	{
		union REGS regs;
		struct SREGS sregs;

		regs.h.ah = 0x3f;
		regs.x.bx = handle;
		regs.x.cx = size * elements;
		sregs.ds  = FP_SEG(buf);
		regs.x.dx = FP_OFF(buf);
		int86x(0x21,&regs,&regs,&sregs);
		if(regs.x.flags & 1)
		{
			n = 0;
			exterror = regs.x.ax;
		}
		else
		{
			n = regs.x.ax;
			exterror = 0;
		}
	}
	totalrw += n;
	return n;
}

void NFILE::scramread()
{
	scramron = 0;
	us size = n_read(scramr,sizeof(uc),SCRAM_SIZE);
	totalrw -= size;
	scramron = 1;
}

void NFILE::scramwrite()
{
	uc tmp = 0;
	randomize();
	for(; tmp < SCRAM_SIZE; ++tmp)
		scramw[tmp] = rand();
	scramwon = 0;
	us size = n_write(scramw,sizeof(uc),SCRAM_SIZE);
	totalrw -= size;
	scramwon = 1;
}

us NFILE::nwrite(void *buf,us size, us elements)
{
	if(scramwon)
	{
		uc pos = 0;
		us tot = size * elements;
		uc *tmp = (uc *)_alloc(tot);
		uc *l = tmp;
		_fmemcpy(tmp,buf,tot);
		for(; tot; --tot, ++l)
		{
			*l ^= scramw[pos];
			if(pos + 1 >= SCRAM_SIZE)
			{
				scramw[pos] += scramw[0];
				pos = 0;
			}
			else
			{
				scramw[pos] += scramw[pos+1];
				++pos;
			}
		}
		us siz = n_write(tmp,size,elements);
		_free(tmp);
		return siz;
	}
	else
		return n_write(buf,size,elements);
}

us NFILE::nread(void *buf,us size, us elements)
{
	if(scramron)
	{
		uc pos = 0;
		us tot = size * elements;
		us siz = n_read(buf,size,elements);
		uc *l = (uc *)buf;
		for(; tot; --tot, ++l)
		{
			*l ^= scramr[pos];
			if(pos + 1 >= SCRAM_SIZE)
			{
				scramr[pos] += scramr[0];
				pos = 0;
			}
			else
			{
				scramr[pos] += scramr[pos+1];
				++pos;
			}
		}
		return siz;
	}
	else
		return n_read(buf,size,elements);
}

us NFILE::n_write(void *buf, us size, us elements)
{
	us n = 0;
	if(size * elements > 0)
	{
		union REGS regs;
		struct SREGS sregs;
		us flags;

		regs.h.ah = 0x40;
		regs.x.bx = handle;
		regs.x.cx = size * elements;
		sregs.ds  = FP_SEG(buf);
		regs.x.dx = FP_OFF(buf);
		int86x(0x21,&regs,&regs,&sregs);
		if(regs.x.flags & 1)
		{
			n = 0;
			exterror = regs.x.ax;
		}
		else
		{
			n = regs.x.ax;
			exterror = 0;
		}
	}
	totalrw += n;
	return n;
}

us NFILE::n_open(string &str, uc access, uc wait)
{
	union REGS regs;
	struct SREGS sregs;
	uc error;

	do
	{
		created = 0;
		handle = 0;
		regs.h.ah = 0x3d;
		regs.h.al = access;
		sregs.ds  = FP_SEG(str.display());
		regs.x.dx = FP_OFF(str.display());
		int86x(0x21,&regs,&regs,&sregs);
	} while((wait)&&(regs.x.flags & 1)&&(regs.x.ax == NO_ACCESS));
	if(regs.x.flags & 1)
	{
		error = regs.x.ax;
		if((error == NO_FILE)&&(createf))
			exterror = error = ncreate(str,filetype,wait);
	}
	else
	{
		handle = regs.x.ax;
		exterror = error = 0;
	}
	if(error != NO_ERROR)
		newfile();
	return error;
}

us NFILE::ncreate(string &str, uc mode, uc wait)
{
	union REGS regs;
	struct SREGS sregs;
	uc error;

	do
	{
		handle = 0;
		regs.h.ah = 0x3c;
		regs.x.cx = mode;
		sregs.ds  = FP_SEG(str.display());
		regs.x.dx = FP_OFF(str.display());
		int86x(0x21,&regs,&regs,&sregs);
	} while((wait)&&(regs.x.flags & 1)&&(regs.x.ax == NO_ACCESS));
	if(regs.x.flags & 1)
		exterror = error = regs.x.ax;
	else
	{
		created = 1;
		handle  = regs.x.ax;
		exterror = error = 0;
	}
	if(error != NO_ERROR)
		newfile();
	return error;
}


us NFILE::nclose()
{
	if(handle)
	{
		uc error;
		union REGS regs;

		regs.h.ah = 0x3e;
		regs.x.bx = handle;
		int86(0x21,&regs,&regs);
		if(regs.x.flags & 1)
			exterror = error = regs.x.ax;
		else
			exterror = error = 0;
		handle = 0;
		newfile();
		return error;
	}
	newfile();
	return NO_ERROR;
}
