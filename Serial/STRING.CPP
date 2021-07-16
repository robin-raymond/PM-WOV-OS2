#include "all.h"
#include "alloc.h"
#include <iostream.h>
#include "string.h"
#include "nfile.h"

uc *_str_test_ = (uc *)"";

ul string::stoul()
{
	_pcheck(str);
	uc *s1 = str;
	ul num = 0;
	for(;*s1; ++s1)
	{
		_check(str,s1);
		if((*s1 >= '0')&&(*s1 <= '9'))
		{
			num *= 10;
			num += (*s1) - 48;
		}
	}
	return num;
}

sl string::stosl()
{
	_pcheck(str);
	uc *s1 = str;
	sl num = 0;
	for(;*s1; ++s1)
	{
		_check(str,s1);
		if(*s1 == '-')
			num *= -1;
		else
		{
			if((*s1 >= '0')&&(*s1 <= '9'))
			{
				num *= 10;
				num += (*s1) - 48;
			}
		}
	}
	return num;
}

void string::ultos(ul num)
{
	char tmp[20];
	uc pos = 0;
	if(num)
	{
		while(num)
		{
			tmp[pos] = '0' + num % 10;
			num /= 10;
			++pos;
		}
	}
	else
	{
		++pos;
		tmp[0] = '0';
	}
	if(info)
	{
		if(allocsiz < pos)
			*this = pos;
	}
	else
		*this = pos;
	uc *s1 = str;
	uc *s2 = (uc *)tmp + pos - 1;
	siz = pos;
	for(;pos; --pos, ++s1, --s2)
	{
		_check(str,s1);
		*s1 = *s2;
	}
	_check(str,s1);
	*s1 = 0;
}

void string::sltos(sl num)
{
	char tmp[20];
	uc pos = 0;
	if(num < 0)
	{
		++pos;
		num *= -1;
	}
	if(num)
	{
		while(num)
		{
			tmp[pos] = '0' + num % 10;
			num /= 10;
			++pos;
		}
	}
	else
	{
		tmp[pos] = '0';
		++pos;
	}
	if(info)
	{
		if(allocsiz < pos)
			*this = pos;
	}
	else
		*this = pos;
	uc *s1 = str;
	uc *s2 = (uc *)tmp + pos - 1;
	siz = pos;
	for(;pos; --pos, ++s1, --s2)
	{
		_check(str,s1);
		*s1 = *s2;
	}
	_check(str,s1);
	*s1 = 0;
}

string &string::deleteletter(us num)
{
	if(info)
	{
		if(num < siz)
		{
			uc *s1 = str + num;
			uc *s2 = str + num + 1;
			us t = siz - num - 1;
			for(;t; --t, ++s1, ++s2)
			{
				_check(str,s1);
				_check(str,s2);
				*s1 = *s2;
			}
			--siz;
			_check(str,str+siz);
			*(str+siz) = 0;
		}
	}
	return *this;
}

void string::putletter(us num, uc let)
{
	setalloc(num+1);
	uc *s1 = str + siz;
	us t = num - siz;
	for(;t; --t, ++s1)
	{
		_check(str,s1);
		*s1 = ' ';
	}
	_check(str,str+num);
	*(str + num) = let;
	_check(str,str+num+1);
	*(str + num + 1) = 0;
	siz = num + 1;
}

string &string::insertletter(us num, uc let)
{
	if(info)
	{
		if(num >= siz)
			putletter(num,let);
		else
		{
			setalloc(siz+1);
			us t = siz - num;
			uc *s1 = str + siz;
			uc *s2 = str + siz - 1;
			for(;t;--t, --s1, --s2)
			{
				_check(str,s1);
				_check(str,s2);
				*s1 = *s2;
			}
			*(str+num) = let;
			++siz;
			*(str+siz) = 0;
		}
	}
	else
	{
		*this = 1;
		siz = 0;
		putletter(num,let);
	}
	return *this;
}

string &string::overwriteletter(us num, uc let)
{
	if(info)
	{
		if(num >= siz)
			putletter(num,let);
		else
		{
			_check(str,str+num);
			*(str+num) = let;
		}
	}
	else
	{
		*this = 1;
		_pcheck(str);
		*str = let;
		_check(str,str+1);
		*(str + 1) = 0;
		siz = 1;
	}
	return *this;
}

string &string::appendat(us num, string &op2)
{
	us ssiz;
	if(info)
	{
		setalloc(num);
		ssiz = siz;
	}
	else
	{
		(*this) = num;
		ssiz = num;
	}
	for(;ssiz < num; ++ssiz)
	{
		_check(str,str+ssiz);
		*(str+ssiz) = ' ';
	}
	siz = num;
	_check(str,str+num);
	*(str+num) = 0;
	(*this) += op2;
	return *this;
}

uc *string::display()
{
	if(info)
		return str;
	else
		return _str_test_;
}

NFILE &operator<<(NFILE &f, string *any)
{
	any->writespecial(f);
	return f;
}

NFILE &operator<<(NFILE &f, string **any)
{
	(**any).writeeof(f);
	return f;
}

us string::writespecial(NFILE &f)
{
	if(!info)
		siz = 0;
	us numwrite = 0;
	if(siz)
	{
		_pcheck(str);
		numwrite = f.nwrite(str,sizeof(uc),siz);
	}
	uc num[2];
	num[0] = 13;
	num[1] = 10;
	numwrite += f.nwrite(num,sizeof(uc),2);
	return numwrite;
}

us string::writeeof(NFILE &f)
{
	if(!info)
		siz = 0;
	us numwrite = 0;
	if(siz)
	{
		_pcheck(str);
		numwrite = f.nwrite(str,sizeof(uc),siz);
	}
	uc num;
	num = 26;
	numwrite += f.nwrite(&num,sizeof(uc),1);
	return numwrite;
}

us string::read(NFILE &f)
{
	us numread = f.nread(&siz,sizeof(us),1);
	if(numread == sizeof(us))
	{
		if(siz)
		{
			uc ssiz = siz;
			*this   = siz;
			siz     = ssiz;
			_check(str,str+siz);
			numread += f.nread(str,sizeof(uc),siz);
			*(str + siz) = 0;
		}
		else
			--(*this);
	}
	else
		--(*this);
	return numread;
}

us string::write(NFILE &f)
{
	if(!info)
		siz = 0;
	us numwrite = f.nwrite(&siz,sizeof(us),1);
	if((numwrite == sizeof(us))&&(siz))
	{
		_pcheck(str);
		numwrite += f.nwrite(str,sizeof(uc),siz);
	}
	return numwrite;
}


void string::resize()
{
	if(info)
	{
		if(siz)
		{
			allocsiz = siz;
			str = (uc *)_realloc(str,siz+1);
		}
		else
		{
			_free(str);
			info = FALSE;
		}
	}
}

string &string::end(uc let)
{
	if((info)&&(siz))
	{
		_pcheck(str);
		uc *s1 = str + siz - 1;
		us lcount = 0;
		us tmps = siz;
		_check(str,s1);
		for(; tmps && (*s1 != let); --s1, --tmps, ++lcount)
		{
#if defined(__DEBUG__)
			if(tmps > 1)
				_check(str,s1-1);
#else
			;
#endif
		}
		if(tmps)
		{
			_check(str,str + (siz - lcount));
			*(str + (siz - lcount)) = 0;
			siz -= lcount;
		}
	}
	return *this;
}

string &string::trail(uc let)
{
	if((info)&&(siz))
	{
		uc *s1 = str + siz - 1;
		_check(str,s1);
		us lcount = 0;
		us tmps = siz;
		for(; tmps &&(*s1 == let); --s1, --tmps, ++lcount)
		{
#if defined(__DEBUG__)
			if(tmps > 1)
				_check(str,s1-1);
#else
			;
#endif
		}
		if(lcount)
		{
			_check(str,str + (siz - lcount));
			*(str + (siz - lcount)) = 0;
			siz -= lcount;
		}
	}
	return *this;
}

string &string::upper()
{
	if(info)
	{
		uc *s1 = str;
		uc su;
		for(;*s1;++s1)
		{
			if((*s1 >= 'a')&&(*s1 <= 'z'))
			{
				_check(str,s1);
				*s1 &= 223;
			}
		}
	}
	return *this;
}

string &string::lower()
{
	if(info)
	{
		uc *s1 = str;
		uc su;
		for(;*s1;++s1)
		{
			if((*s1 >= 'A')&&(*s1 <= 'Z'))
			{
				_check(str,s1);
				*s1 |= 32;
			}
		}
	}
	return *this;
}

string &string::lead(uc let)  // let can not be null character
{
	if((info)&&(siz))
	{
		uc *s2 = str;
		uc lcount = 0;
		for(; *s2 == let; ++s2, ++lcount);
		uc *s1 = str;
		for(; *s2; ++s1, ++s2)
		{
			_check(str,s1);
			*s1 = *s2;
		}
		_check(str,s1);
		*s1 = 0;
		siz -= lcount;
	}
	return *this;
}

string &string::operator+=(string &op2)
{
	if(op2.info)
	{
		if(info)
		{
			if(siz+op2.siz > allocsiz)
			{
				allocsiz = siz+op2.siz;
				str = (uc *)_realloc(str,allocsiz+1);
			}
		}
		else
		{
			allocsiz = op2.siz;
			str = (uc *)_alloc(op2.siz+1);
			siz = 0;
		}
		uc *s1 = str + siz, *s2 = op2.str;
		for(;*s2; ++s1, ++s2)
		{
			_check(str,s1);
			*s1 = *s2;
		}
		_check(str,s1);
		*s1  = 0;
		siz += op2.siz;
		info = TRUE;
	}
	return *this;
}

string &string::operator+=(char *op2)
{
	string temp;
	temp.str = (uc *)op2;
	temp.info = TRUE;
	temp.siz  = 0;
	for(; *op2; ++op2, ++temp.siz);
	*this += temp;
	temp.info = FALSE;
	return *this;
}

sc string::operator==(char *op2)
{
	string temp;
	temp.str  = (uc *)op2;
	temp.info = TRUE;
	sc ret = (*this == temp);
	temp.info = FALSE;
	return ret;
}

sc string::operator^=(char *op2)
{
	string temp;
	temp.str  = (uc *)op2;
	temp.info = TRUE;
	sc ret = (*this ^= temp);
	temp.info = FALSE;
	return ret;
}

sc string::operator^=(string &op2)
{
	uc *s1 = str, *s2 = op2.str;
	if(!info)
		s1 = (uc *)"";
	if(!op2.info)
		s2 = (uc *)"";
	si si1 = *s1;
	si si2 = *s2;
	if((si1 >= 'a')&&(si1 <= 'z'))
		si1 &= 223;
	if((si2 >= 'a')&&(si2 <= 'z'))
		si2 &= 223;
	while((si1 == si2)&&(*s1)&&(*s2))
	{
		++s1;
		++s2;
		si1 = *s1;
		si2 = *s2;
		if((si1 >= 'a')&&(si1 <= 'z'))
			si1 &= 223;
		if((si2 >= 'a')&&(si2 <= 'z'))
			si2 &= 223;
	}
	return (si1 - si2);
}

sc string::operator==(string &op2)
{
	uc *s1 = str, *s2 = op2.str;
	if(!info)
		s1 = (uc *)"";
	if(!op2.info)
		s2 = (uc *)"";
	for(; (*s1 == *s2) && (*s1) && (*s2); ++s1, ++s2);
	return (((si)*s1)-((si)*s2));
}


string &string::operator=(us ssiz)
{
	if(info)
	{
		if(ssiz >= allocsiz)
			str = (uc *)_realloc(str,ssiz+1);
	}
	else
		str  = (uc *)_alloc(ssiz+1);
	info = TRUE;
	allocsiz = ssiz;
	siz = 0;
	_pcheck(str);
	*str = 0;
	return *this;
}

string &string::setalloc(us ssiz)
{
	if(info)
	{
		if(ssiz >= allocsiz)
			str = (uc *)_realloc(str,ssiz+1);
	}
	else
	{
		str = (uc *)_alloc(ssiz+1);
		_pcheck(str);
		*str = 0;
		siz = 0;
	}
	_check(str,str + siz);
	*(str + siz) = 0;
	info = TRUE;
	allocsiz = ssiz;
	_pcheck(str);
	return *this;
}

string &string::operator=(string &op2)
{
	if(op2.info)
	{
		if(info)
		{
			if(op2.siz >= allocsiz)
			{
				str = (uc *)_realloc(str,op2.siz+1);
				allocsiz = op2.siz;
			}
		}
		else
			*this = op2.siz;
		siz = op2.siz;
		uc *s1 = str, *s2 = op2.str;
		for(; *s2; ++s1, ++s2)
		{
			_check(str,s1);
			*s1 = *s2;
		}
		_check(str,s1);
		*s1 = 0;
	}
	else
		--*this;
	return *this;
}

string &string::operator=(char *op2)
{
	uc *s2 = (uc *)op2;
	us tsiz = 0;
	for(; *s2; ++s2, ++tsiz);
	if(info)
	{
		if(tsiz >= allocsiz)
		{
			str = (uc *)_realloc(str,tsiz+1);
			allocsiz = tsiz;
		}
	}
	else
	{
		*this = tsiz;
	}
	siz = tsiz;
	uc *s1 = str;
	s2 = (uc *)op2;
	for(; *s2; ++s1, ++s2)
	{
		_check(str,s1);
		*s1 = *s2;
	}
	_check(str,s1);
	*s1 = 0;
	return *this;
}
