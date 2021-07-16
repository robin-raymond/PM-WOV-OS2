#include "all.h"
#include "string.h"
#include "alloc.h"
#include "nfile.h"
#include "screen.h"
#include "field.h"
#include <time.h>
#include <stdlib.h>

#define BLOCKSIZE 10000
#define FILESIZE 1000000

uc problembox(uc x1, uc y1, uc x2, uc y2, uc num, char *n, char *o)
{
	SHADEBOX main(x1,y1,x2,y2,LGRAY|BA(RED),SINGLE,TRUE);
	char *tmp = n;
	uc t;
	for(t = 0; *tmp; ++tmp, ++t);
	_dpa(x1+2,y1+2,n,WHITE|BA(LRED),t);
	uc tp = 4;
	if(o != NULL)
	{
		tmp = o;
		for(t = 0; *tmp; ++tmp, ++t);
		_dpa(x1+2,y1+4,o,WHITE|BA(LRED),t);
		tp = 6;
	}
	if(num)
		t = 8;
	else
		t = 6;
	uc pos = 0;
	BUTTON entry(pos,t);
	EDITOR okay;
	entry.add(x1+7,y1+tp,GRAY|BA(BLACK),LCYAN|BA(WHITE),BLACK|BA(RED)," OKAY");
	if(num)
		entry.add(x1+18,y1+tp,GRAY|BA(BLACK),LCYAN|BA(WHITE),BLACK|BA(RED)," CREATE");
	okay += entry;
	keypress key;
	okay.settime(255);
	okay.addbreak(S_ESC);
	if(okay.invoke(key) == ENTER_EDIT)
		return pos;
	return 0;
}

void message(string &m, uc rw)
{
	if(rw)
		_dpa(12,12,"Reading:",WHITE|BA(RED),30);
	else
		_dpa(12,12,"Writing:",WHITE|BA(RED),30);
	_dpa(21,12,m,WHITE|BA(RED),40);
}

#define ULONG ul
#define CHAR char
#define UCHAR uc

ULONG toserialno(string &ser)
{
	uc *Source = (char *)ser.display();

	ULONG Number = 1500;
	if ((*Source) == 0)
		return 1294854921;

	ULONG Pos = 0;
	ULONG Old = (*Source);

	for(; *Source != 0; ++Pos, ++Source)
	{
		if (((ULONG)*Source >= (ULONG)'a') && ((ULONG)*Source <= (ULONG)'z'))
			*Source = (UCHAR)((ULONG)*Source - (ULONG)(UCHAR)'a' + (ULONG)(UCHAR)'A');

		Number += (Pos + 1) * (((ULONG)*Source)^((ULONG)134));
		if (Old != 0)
			Number *= Old;

		Old = *Source;
	}
	return Number;
}

ul retries = 0;

ul maxul = 4294967295;

uc checkmaster(NFILE &f,ul newserial, string &filename)
{
	ul ch = (maxul / (FILESIZE*8))+1;
	ul byte = newserial / (ch*8);
	uc left = 1 << (newserial % 8);
	uc readbyte;
	message(filename,1);
	f.nseek(byte,BEG_POS);
	f >> readbyte;
	if(readbyte & left)
		return 0;
	else
	{
		message(filename,0);
		f.nseek(byte,BEG_POS);
		readbyte |= left;
		f << readbyte;
	}
	return 1;
}

uc allocserial(string &numserial, string &masterfile, string &written, string &subfile)
{
	NFILE m, w;

	uc added = 0;
	uc error = m.nopen(masterfile,READWRITE|D_READWRITE,TRUE);
	if(error == NO_FILE)
	{
		uc *chunks = (uc *)_alloc(BLOCKSIZE);
		uc *s1 = chunks;
		us clear = 0;
		for(; clear < BLOCKSIZE; ++clear, ++s1)
			*s1 = 0;
		error = m.ncreate(masterfile,NORMAL,TRUE);
		if(error == NO_ERROR)
		{
			message(masterfile,0);
			ul tmp = FILESIZE / BLOCKSIZE;
			for(; tmp; --tmp)
				m.nwrite(chunks,sizeof(uc),BLOCKSIZE);
			m.nclose();
			error = m.nopen(masterfile,READWRITE|D_READWRITE,TRUE);
		}
		_free(chunks);
	}

	if(error == NO_ERROR)
	{
		ul newserial = toserialno(numserial);
		ul v2 = (newserial + newserial) ^ ((ul)8478372345);

		added = checkmaster(m,newserial,masterfile);

		if (added)
		{
			error = w.nopen(written,WRITE|D_READWRITE,TRUE,NORMAL);
			if(error == NO_ERROR)
			{
				NFILE ss;
				error = ss.ncreate(subfile,NORMAL,FALSE);
				if (error == NO_ERROR)
				{
					w.nappend();
					string section = "------------------------SECTION--------------";
					w << (&section);
					string line;
					line = numserial;
					line += "\t";
					string tnum;
					tnum << newserial;
					line += tnum;
					line += "\t";
					tnum << v2;
					line += tnum;
					w << (&line);
					ss << (&line);
					ss.nclose();
				}
				w.nclose();
			}
		}
		m.nclose();
	}
	return added;
}

void createserials(ul numserials,string &masterfile, string &subserial, string &written)
{
	NFILE m, s, w;
	string sername = "RAB#1234567";
	string outstring;
	uc error = m.nopen(masterfile,READWRITE|D_READWRITE,TRUE);
	if(error == NO_FILE)
	{
		uc *chunks = (uc *)_alloc(BLOCKSIZE);
		uc *s1 = chunks;
		us clear = 0;
		for(; clear < BLOCKSIZE; ++clear, ++s1)
			*s1 = 0;
		error = m.ncreate(masterfile,NORMAL,TRUE);
		if(error == NO_ERROR)
		{
			message(masterfile,0);
			ul tmp = FILESIZE / BLOCKSIZE;
			for(; tmp; --tmp)
				m.nwrite(chunks,sizeof(uc),BLOCKSIZE);
			m.nclose();
			error = m.nopen(masterfile,READWRITE|D_READWRITE,TRUE);
		}
		_free(chunks);
	}
	ul newserial;
	uc last = 0;
	string number = "99999999999999999999";
	if(error == NO_ERROR)
	{
		error = s.ncreate(subserial,NORMAL,TRUE);
		if(error == NO_ERROR)
		{
			error = w.nopen(written,WRITE|D_READWRITE,TRUE,NORMAL);
			if(error == NO_ERROR)
			{
				w.nappend();
				string section = "------------------------SECTION--------------";
				w << (&section);
				for(; numserials; --numserials)
				{
					us added = 0;
					do
					{
						us start = 4;
						us stop = 11;
						for(; start <= stop; ++start)
						{
							if((rand()%3)||(start == 4))
							{
								uc tmplet = rand()%26 + 'A';
								if (tmplet == 'O')
									tmplet = rand()%11 | 'A';
								sername.overwriteletter(start,tmplet);
							}
							else
							{
								if((rand()%5)||(start == stop)||(last))
								{
									sername.overwriteletter(start,rand()%9+'1');
									last = 0;
								}
								else
								{
									sername.overwriteletter(start,'-');
									last = 1;
								}
							}
						}
						newserial = toserialno(sername);
						added = checkmaster(m,newserial,masterfile);
					} while(!added);
					message(subserial,0);
//					s << newserial;
//					s << sername;
					number << newserial;
					outstring = sername;
					outstring += "\t";
					outstring += number;
					ul v2 = (newserial + newserial) ^ ((ul)8478372345);
					number << v2;
					outstring += "\t";
					outstring += number;
					message(written,0);
					w << (&outstring);
					s << (&outstring);
				}
				w.nclose();
			}
			s.nclose();
		}
		m.nclose();
	}
}

void runinstall()
{
	ul numserials = 100;
	string masterfile = "MSERIAL.FIL";
	string subserial = "SSERIAL.TXT";
	string written = "WRITTEN.TXT";
	uc WC = WHITE|BA(BLUE);
	BOX blank(0,0,columns-1,rows-1,WC,SINGLE,TRUE);
	uc x1 = 3;
	uc y1 = 2;
	_dpa(x1,y1,"Welcome to AcSYS v2.0 serial creation utility...",WC,60);
	_dpa(x1,y1+5,"How many serial numbers to create?",WC,60);
	_dpa(x1,y1+10,"   Name of sub serial file:",WC,60);
	_dpa(x1,y1+12,"Name of master serial file:",WC,60);
	_dpa(x1,y1+14,"      Name of written file:",WC,60);
	_dpa(x1,y1+18,"Serial number retries:",WC,60);
	EDITOR logging;
	FIELD entry;
	entry.addfield(numserials,x1+35,y1+5,20,WC&15,WC&15,FALSE,TRUE,FALSE);
	entry.addfield(subserial,x1+28,y1+10,40,WC&15,WC&15,FALSE,TRUE,FALSE);
	entry.addfield(masterfile,x1+28,y1+12,40,WC&15,WC&15,FALSE,TRUE,FALSE);
	entry.addfield(written,x1+28,y1+14,40,WC&15,WC&15,FALSE,TRUE,FALSE);
	logging += entry;
	keypress key;
	entry.settime(255);
	logging.addbreak(S_ESC);
	uc mode;
	mode = (logging.invoke(key) == ENTER_EDIT);
	if(mode)
	{
		masterfile.trail(' ');
		subserial.trail(' ');
		written.trail(' ');
		if(numserials)
		{
			SHADEBOX blank2(10,10,70,14,WHITE|BA(RED),SINGLE,TRUE);
			createserials(numserials,masterfile,subserial,written);
		}
		_dpa(x1,y1+20,"Press any key to exit...",WC,60);
		key.getscan();
	}
}

/*
void batchcreate()
{
	string sourcedrive = "A:\\";
	string subserial = "SSERIAL.FIL";
	uc WC = WHITE|BA(BLUE);
	BOX blank(0,0,columns-1,rows-1,WC,SINGLE,TRUE);
	uc x1 = 3;
	uc y1 = 2;
	_dpa(x1,y1,"Welcome to AcSYS v2.0 batch creation utility...",WC,60);
	_dpa(x1,y1+10,"      Destination drive:",WC,60);
	_dpa(x1,y1+12,"Name of sub serial file:",WC,60);
	EDITOR logging;
	FIELD entry;
	entry.addfield(sourcedrive,x1+25,y1+10,40,WC&15,WC&15,FALSE,TRUE,FALSE);
	entry.addfield(subserial,x1+25,y1+12,40,WC&15,WC&15,FALSE,TRUE,FALSE);
	logging += entry;
	keypress key;
	entry.settime(255);
	logging.addbreak(S_ESC);
	uc mode;
	mode = (logging.invoke(key) == ENTER_EDIT);
	if(mode)
	{
		subserial.trail(' ');
		sourcedrive.trail(' ');
		NFILE sour, dest, check;
		uc error = sour.nopen(subserial,READ|D_READWRITE,TRUE);
		if(error == NO_ERROR)
		{
			string trick = "RAB#-EIEIO";
			string curnum;
			BOX blank1(0,0,columns-1,rows-1,WC,SINGLE,FALSE);
			_dpa(x1,y1,"Please insert destination diskette into drive: ",WC,60);
			_dpa(x1+47,y1,sourcedrive,WC,20);
			_dpa(x1,y1+4,"Destination #:",WC,60);
			_dpa(x1,y1+8,"Press <SPACE> to continue...",WC,60);
			us _cur = 0;
			ul newserial;
			string actualser;
			string number;
			sour.getrw();
			sour >> newserial;
			us tot = sour.getrw();
			actualser;
			sour >> actualser;
			sour.getrw();
			while(tot == sizeof(ul))
			{
				++_cur;
				number << _cur;
				_dpa(x1+15,y1+4,number,WC,60);
				uc done = 0;
				do
				{
					_dpa(x1,y1+16,"Creating serial: ",LGRAY|BA(BLUE),60);
					_dpa(x1,y1+18,"  Actual serial: ",LGRAY|BA(BLUE),60);
					_dpa(x1+25,y1+16,(char *)actualser.display()+4,WC,20);
					number << newserial;
					_dpa(x1+25,y1+18,number,WC,20);
					key.getscan();
					while((key.scan != S_SPACE)&&(key.scan != S_ESC))
					{
						key.getscan();
					}
					if(key.scan == S_SPACE)
					{
						string cur = sourcedrive;
						cur += "INSTALL.EXE";
						error = check.nopen(cur,READ|D_READWRITE,TRUE);
						check.nclose();
						if(error == NO_ERROR)
						{
							cur = sourcedrive;
							cur += "_____.___";
							error = dest.nopen(cur,WRITE|D_READWRITE,TRUE,NORMAL);
							if(error == NO_ERROR)
							{
								BOX blank1(0,0,columns-1,rows-1,LGRAY|BA(BLUE),SINGLE,TRUE);
								_dpa(x1,y1,"Creating serial: ",LGRAY|BA(BLUE),60);
								_dpa(x1,y1+2,"  Actual serial: ",LGRAY|BA(BLUE),60);
								_dpa(x1,y1+16,"Creating serial: ",LGRAY|BA(BLUE),60);
								_dpa(x1,y1+18,"  Actual serial: ",LGRAY|BA(BLUE),60);
								_dpa(x1+25,y1,actualser,WC,20);
								_dpa(x1+25,y1+16,(char *)actualser.display()+4,WC,20);
								number << newserial;
								_dpa(x1+25,y1+2,number,WC,20);
								_dpa(x1+25,y1+18,number,WC,20);
								dest << newserial;
								us tmp;
								uc j;
								for(tmp = 0; tmp < 495; ++tmp)
								{
									if(rand()%2)
										j = rand();
									else
										j = rand()%26+'A';
									dest << j;
									if(tmp == 100)
										dest << trick;
								}
								dest.nclose();
								_dpa(x1,y1+8,"CREATED!!!",WC,60);
								_dpa(x1,y1+10,"Press <ENTER> to continue...",WC,60);
								key.getscan();
								while(key.scan != S_ENTER)
								{
									key.getscan();
								}
								done = 1;
							}
						}
						else
						{
							SHADEBOX blank2(10,10,70,14,WHITE|BA(RED),SINGLE,TRUE);
							_dpa(12,12,"Wrong diskette!",WHITE|BA(RED),30);
							key.getscan();
						}
						sour >> newserial;
						tot = sour.getrw();
						sour >> actualser;
						sour.getrw();
					}
					else
					{
						tot = 0;
						done = 1;
					}
				} while(!done);
			}
			sour.nclose();
		}
	}
}
*/

void manualstamp()
{
	string numserial = "RAB#";
	string masterfile = "MSERIAL.FIL";
	string written = "WRITTEN.TXT";
	string subfile = "SSERIAL.TXT";
	uc WC = WHITE|BA(BLUE);
	BOX blank(0,0,columns-1,rows-1,WC,SINGLE,TRUE);
	uc x1 = 3;
	uc y1 = 2;
	_dpa(x1,y1,"Welcome to AcSYS v2.0 serial creation utility...",WC,60);
	_dpa(x1,y1+5, "             Serial number:",WC,60);
	_dpa(x1,y1+10,"   Name of sub serial file:",WC,60);
	_dpa(x1,y1+12,"Name of master serial file:",WC,60);
	_dpa(x1,y1+14,"      Name of written file:",WC,60);
	EDITOR logging;
	FIELD entry;
	entry.addfield(numserial,x1+28,y1+5,20,WC&15,WC&15,FALSE,TRUE,FALSE);
	entry.addfield(subfile,x1+28,y1+10,40,WC&15,WC&15,FALSE,TRUE,FALSE);
	entry.addfield(masterfile,x1+28,y1+12,40,WC&15,WC&15,FALSE,TRUE,FALSE);
	entry.addfield(written,x1+28,y1+14,40,WC&15,WC&15,FALSE,TRUE,FALSE);
	logging += entry;
	keypress key;
	entry.settime(255);
	logging.addbreak(S_ESC);
	uc mode;
	uc added = 0;
	int error;
	do
	{
		mode = (logging.invoke(key) == ENTER_EDIT);
		if(mode)
		{
			numserial.trail(' ');
			masterfile.trail(' ');
			subfile.trail(' ');
			written.trail(' ');

			SHADEBOX blank2(10,10,70,14,WHITE|BA(RED),SINGLE,TRUE);
			added = allocserial(numserial,masterfile,written,subfile);
		}
		else
			added = 1;
	} while(!added);

	if (mode)
	{

		BOX blank1(0,0,columns-1,rows-1,WC,SINGLE,FALSE);
		_dpa(x1,y1,"Serial number created!",WC,60);
		_dpa(x1,y1+2,"Press <SPACE> to continue...",WC,60);


		ul newserial = toserialno(numserial);
		ul v2 = (newserial + newserial) ^ ((ul)8478372345);

		_dpa(x1,y1+16,"Creating serial: ",LGRAY|BA(BLUE),60);
		_dpa(x1,y1+18,"      Verify #1: ",LGRAY|BA(BLUE),60);
		_dpa(x1,y1+20,"      Verift #2: ",LGRAY|BA(BLUE),60);
		_dpa(x1+25,y1+16,numserial,WC,20);

		string number;
		number << newserial;
		_dpa(x1+25,y1+18,number,WC,20);

		number << v2;
		_dpa(x1+25,y1+20,number,WC,20);

		key.getscan();
	}

/*		uc notfound;
		do
		{
			notfound = 0;
			key.getscan();
			while((key.scan != S_SPACE)&&(key.scan != S_ESC))
			{
				key.getscan();
			}
			if(key.scan == S_SPACE)
			{
				string cur = sourcedrive;
				cur += "INSTALL.EXE";
            NFILE check;
				error = check.nopen(cur,READ|D_READWRITE,TRUE);
				if (error == NO_ERROR)
					check.nclose();
				else
					notfound = 1;
			}
		} while(notfound);
		if (key.scan == S_SPACE)
		{
			string cur;
			cur = sourcedrive;
			cur += "_____.___";
			NFILE dest;
			error = dest.nopen(cur,WRITE|D_READWRITE,TRUE,NORMAL);
			if(error == NO_ERROR)
			{
				dest << newserial;
				us tmp;
				uc j;
				for(tmp = 0; tmp < 495; ++tmp)
				{
					if(rand()%2)
						j = rand();
					else
						j = rand()%26+'A';
					dest << j;
					if(tmp == 100)
						dest << trick;
				}
				dest.nclose();
				_dpa(x1,y1+20,"CREATED!!!",WC,60);
				_dpa(x1,y1+10,"Press <ENTER> to continue...",WC,60);
				key.getscan();
				while(key.scan != S_ENTER)
				{
					key.getscan();
				}
			}
		}
	}*/
}

int main(int n, char **argv)
{
	class SETUPSCREEN screen;
	BOX blank(0,0,columns-1,rows-1,BLACK|BA(BLACK),SINGLE,TRUE);
	cursor(CUR_OFF);
	int i;
	time_t t;

	srand((unsigned) time(&t));
	if(n > 1)
	{
		string arg = *(argv + 1);
		if (arg ^= "/MANUAL")
			runinstall();
		else
			manualstamp();
	}
	else
		runinstall();
	cursor(CUR_ON);
	screen.cleanscreen();
	return 0;
}




