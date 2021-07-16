#include "all.h"
#include "screen.h"
#include "field.h"
#include "keyboard.h"
#include "string.h"
#include "alloc.h"
#include <time.h>

#define UP_EDIT 0
#define DOWN_EDIT 1
#define ENTER_EDIT 3
#define TIME_OUT 5
#define KEY_BREAK 6

void operator>>(string &s, c_letter &l)
{
	*(l.letter) = *(s.display());
}

void operator<<(string &s, c_letter &l)
{
	char t[2];
	t[1] = 0;
	t[0] = (char)(*(l.letter));
	s = t;
}

void EDITOR::addbreak(uc which)
{
	uc cur = total;
	INPUT **l = list;
	for(;cur; --cur,++l)
	{
		_check(list,l);
		(**l).addbreak(which);
	}
}

void EDITOR::settime(uc which)
{
	uc cur = total;
	INPUT **l = list;
	for(;cur; --cur,++l)
	{
		_check(list,l);
		(**l).settime(which);
	}
}

void EDITOR::operator+=(INPUT &any)
{
	if(total)
		list = (INPUT **)_realloc(list,sizeof(INPUT *)*(total+1));
	else
		list = (INPUT **)_alloc(sizeof(INPUT *));
	_check(list,list+total);
	(*(list + total)) = &any;
	++total;
}

EDITOR::~EDITOR()
{
	if(total)
	{
		_free(list);
		total = 0;
	}
}

uc EDITOR::invoke(keypress &any)
{
	uc mode = DOWN_EDIT;
	if(total)
	{
		uc working = 1;
		uc cur = 0;
		do
		{
			_check(list,list+cur);
			mode = (**(list + cur)).invoke(mode,any);
			switch(mode)
			{
				case UP_EDIT:
									if(cur)
										--cur;
									else
										cur = total - 1;
									break;
				case ENTER_EDIT:
									if((last)&&(cur + 1 >= total))
										working = 0;
				case DOWN_EDIT:
									++cur;
									if(cur >= total)
										cur = 0;
									break;
				case TIME_OUT:
				case KEY_BREAK:
									working = 0;
									break;
			}
		} while(working);
	}
	return mode;
}

char *fill_pat = "께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께";

#define MATCH_POS 1
#define HOME_POS 2
#define END_POS 3

uc FIELD::invoke(uc edit_mode, keypress &key)
{
	if(!total)
		_doexit(1);
	switch(edit_mode)
	{
		case UP_EDIT:
							field = total - 1;
							break;
		case DOWN_EDIT: field = 0;
							 break;
		default:
			break;
	}
	if(field >= total)
		field = 0;
	string temp;
	temp = 82;

	uc xpos = 0;
	if(*(def + field))
	{
		_check(val,val+field);
		(**(val + field)) >> temp;
	}
	_check(x1,x1+field);
	_check(y1,y1+field);
	cursorat(xpos+*(x1+field),*(y1+field));
	cursor(CUR_ON);
	uc *str = temp.display();
	us xs = !temp;
	uc tmp;
	uc found;
	uc quitting;
	time_t lasttime;
	lasttime = time(NULL);
	uc reason = 0;
	do
	{
		us newfield = field;
		uc newpos = 0;
		quitting = 0;
		uc refresh = 0;
		_check(x1,x1+field);
		_check(y1,y1+field);
		cursorat(xpos+*(x1+field),*(y1+field));
		if(*(sec + field))
			_dpa(*(x1+field),*(y1+field),fill_pat+100-xs,*(WC+field),*(siz+field));
		else
			_dpa(*(x1+field),*(y1+field),(char *)str,*(WC+field),*(siz+field));
		do
		{
			key.quick_getscan();
			if(key.scan)
			{
				lasttime = time(NULL);
				found = 0;
				for(tmp = 0; (!found) && (tmp < totbreak); ++tmp)
				{
					if((*(fbreak+tmp))==(key.scan))
					{
						found = 1;
						refresh = 1;
						quitting = 1;
						reason = KEY_BREAK;
					}
				}
				if(!found)
				{
					if(key.asc)
						key.scan = 0;
					switch(key.scan)
					{
						case S_BACK_SPACE:
									uc *s1, *s2;
									if(xpos)
									{
										if(xpos <= xs)
										{
											s1 = str + xpos - 1;
											s2 = str + xpos;
											for(;*s2; ++s1, ++s2)
												*s1 = *s2;
											*s1 = 0;
											--xs;
										}
										--xpos;
										refresh = 1;
									}
									break;
						case S_HOME:
									xpos = 0;
									refresh = 1;
									break;
						case S_END:
									xpos = xs;
									refresh = 1;
									break;
						case S_INSERT:
									key.setinsert();
									break;
						case S_DELETE:
									if(xpos < xs)
									{
										s1 = str + xpos;
										s2 = str + xpos + 1;
										if(*s1)
											--xs;
										for(;*s2; ++s1, ++s2)
											*s1 = *s2;
										*s1 = 0;
										refresh = 1;
									}
									break;
						case S_ENTER:
									++newfield;
									if(newfield >= total)
									{
										quitting = 1;
										reason = ENTER_EDIT;
										newfield = field;
									}
									else
										newpos = END_POS;
									refresh = 1;
									break;
						case S_CUR_UP:
									if(field)
										--newfield;
									else
									{
										reason = UP_EDIT;
										quitting = 1;
									}
									refresh = 1;
									newpos = MATCH_POS;
									break;
						case S_TAB:
									if(field + 1 < total)
										++newfield;
									else
										newfield = 0;
									newpos = HOME_POS;
									refresh = 1;
									break;
						case S_CUR_DOWN:
									if(field + 1 < total)
										++newfield;
									else
									{
										quitting = 1;
										reason = DOWN_EDIT;
									}
									newpos = MATCH_POS;
									refresh = 1;
									break;
						case S_CUR_LEFT:
									if(xpos)
									{
										--xpos;
										refresh = 1;
									}
									break;
						case S_CUR_RIGHT:
									if(xpos < (*(siz + field)))
									{
										++xpos;
										refresh = 1;
									}
									break;
						default:
							us tmp;
							if(key.asc)
							{
								if(xpos < xs)
								{
									if(key.insertmode())
									{
										s1 = str + xs - 1;
										s2 = str + xs;
										*(s2 + 1) = 0;
										tmp = xs - xpos;
										for(;tmp;--tmp, --s1, --s2)
											*s2 = *s1;
										*s2 = key.asc;
										++xs;
										if(xs > *(siz + field))
										{
											xs = *(siz + field);
											*(str + xs) = 0;
										}
										++xpos;
										if(xpos > *(siz + field))
											xpos = *(siz + field);
										refresh = 1;
									}
									else
									{
										*(str + xpos) = key.asc;
										++xpos;
										refresh = 1;
									}
								}
								else
								{
									s1 = str + xs;
									s2 = str + xpos;
									for(; s1 != s2; ++s1)
										*s1 = ' ';
									*s2 = key.asc;
									++xpos;
									*(s2 + 1) = 0;
									if(xpos > *(siz + field))
										xpos = *(siz + field);
									xs = xpos;
									if(xs >= *(siz + field))
									{
										xs = *(siz + field);
										*(str + xs) = 0;
										if(*(retauto + field))
										{
											++newfield;
											if(newfield >= total)
											{
												quitting = 1;
												reason = ENTER_EDIT;
												newfield = field;
											}
											else
												newpos = END_POS;
										}
									}
									refresh = 1;
								}
							}
							break;
					}
				}
			}
			else
			{
				time_t curtime = time(NULL);
				if(curtime > (lasttime + (((ul)quittime) * 60)))
				{
					refresh = 1;
					quitting = 1;
					reason = TIME_OUT;
				}
			}
		} while(!refresh);
		if(newfield != field)
		{
			*(def + field) = 1;
			temp.setsize(xs);
			(**(val + field)) << temp;
			(**(val + field)) >> temp;
			if(*(sec + field))
				_dpa(*(x1+field),*(y1+field),fill_pat+100-xs,*(WC+field),*(siz+field));
			else
				_dpa(*(x1+field),*(y1+field),(char *)str,*(WC+field),*(siz+field));
			_check(def,def+newfield);
			_check(val,val+newfield);
			if(*(def + newfield))
				(**(val + newfield)) >> temp;
			else
				temp.trunc(0);
			temp.setalloc(82);
			xs = temp.getsize();
			str = temp.display();
			switch(newpos)
			{
				case MATCH_POS:
						if((*(x1 + newfield)) <= ((*(x1 + field)) + xpos))
						{
							if(((*(x1+field)) + xpos) <= ((*(x1+newfield))+(*(siz+newfield))))
							{
								if((*(x1+field)) >= (*(x1+newfield)))
									xpos += (*(x1+field)) - (*(x1+newfield));
								else
									xpos -= (*(x1+newfield)) - (*(x1+field));
							}
							else
								xpos = (*(siz+newfield));
						}
						else
							xpos = 0;
						break;
				case HOME_POS:
						xpos = 0;
						break;
				case END_POS:
						xpos = xs;
						break;
			}
			field = newfield;
		}
	} while(!quitting);
	*(def + field) = 1;
	temp.setsize(xs);
	(**(val + field)) << temp;
	(**(val + field)) >> temp;
	if(*(sec + field))
		_dpa(*(x1+field),*(y1+field),fill_pat+100-xs,*(WC+field),*(siz+field));
	else
		_dpa(*(x1+field),*(y1+field),(char *)str,*(WC+field),*(siz+field));
	cursor(CUR_OFF);
	temp.trunc(0);
	quitting = 0;
	for(; quitting < total; ++quitting)
	{
		if(!(*(def + quitting)))
		{
			(**(val + quitting)) << temp;
			*(def+quitting) = 1;
		}
	}
	return reason;
}

void FIELD::addbreak(uc b)
{
	if(totbreak)
		fbreak = (uc *)_realloc(fbreak,sizeof(uc)*(totbreak+1));
	else
		fbreak = (uc *)_alloc(sizeof(uc));
	_check(fbreak,fbreak + totbreak);
	*(fbreak + totbreak) = b;
	++totbreak;
}

void FIELD::clearbreak()
{
	if(totbreak)
	{
		_free(fbreak);
		totbreak = 0;
	}
}

void FIELD::do_addfield(b_convert *ndat, uc nx1, uc ny1, uc nsiz, uc nWC, uc nDC, uc nretauto, uc ndef, uc nsec)
{
	if(total)
	{
		++total;
		val = (b_convert **)_realloc(val,sizeof(b_convert *)*total);
		x1 = (uc *)_realloc(x1,sizeof(uc)*total);
		y1 = (uc *)_realloc(y1,sizeof(uc)*total);
		siz = (uc *)_realloc(siz,sizeof(uc)*total);
		WC = (uc *)_realloc(WC,sizeof(uc)*total);
		DC = (uc *)_realloc(DC,sizeof(uc)*total);
		retauto = (uc *)_realloc(retauto,sizeof(uc)*total);
		def = (uc *)_realloc(def,sizeof(uc)*total);
		sec = (uc *)_realloc(sec,sizeof(uc)*total);
	}
	else
	{
		++total;
		val = (b_convert **)_alloc(sizeof(b_convert *)*total);
		x1 = (uc *)_alloc(sizeof(uc)*total);
		y1 = (uc *)_alloc(sizeof(uc)*total);
		siz = (uc *)_alloc(sizeof(uc)*total);
		WC = (uc *)_alloc(sizeof(uc)*total);
		DC = (uc *)_alloc(sizeof(uc)*total);
		retauto = (uc *)_alloc(sizeof(uc)*total);
		def = (uc *)_alloc(sizeof(uc)*total);
		sec = (uc *)_alloc(sizeof(uc)*total);
	}
	--total;
	_check(val,val + total);
	_check(x1,x1 + total);
	_check(y1,y1 + total);
	_check(siz,siz + total);
	_check(WC,WC + total);
	_check(DC,DC + total);
	_check(retauto,retauto + total);
	_check(def,def + total);
	_check(sec,sec + total);
	*(val + total) = ndat;
	*(x1 + total) = nx1;
	*(y1 + total) = ny1;
	*(siz + total) = nsiz;
	*(WC + total) = nWC;
	*(DC + total) = nDC;
	*(retauto + total) = nretauto;
	*(def + total) = ndef;
	*(sec + total) = nsec;
	++total;

	if(ndef)
	{
		string temp;
		(*ndat) >> temp;
		if(nsec)
			_dpa(nx1,ny1,fill_pat+100-(!temp),nWC,nsiz);
		else
			_dpa(nx1,ny1,temp,nWC,nsiz);
	}
	else
		_dpa(nx1,ny1,"",nWC,nsiz);
}

void FIELD::redisplay()
{
	uc tmp = 0;
	for(;tmp < total; ++tmp)
	{
		if(*(def + tmp))
		{
			string temp;
			(**(val+tmp)) >> temp;
			if(*(sec+tmp))
				_dpa(*(x1+tmp),*(y1+tmp),fill_pat+100-(!temp),*(WC+tmp),*(siz+tmp));
			else
				_dpa(*(x1+tmp),*(y1+tmp),temp,*(WC+tmp),*(siz+tmp));
		}
		else
			_dpa(*(x1+tmp),*(y1+tmp),"",*(WC+tmp),*(siz+tmp));
	}
}

void FIELD::clear()
{
	quittime = 1;
	if(total)
	{
		 uc tmp = 0;
		 for(;tmp < total; ++tmp)
		 {
			 if(*(def + tmp))
			 {
				 string temp;
				 (**(val+tmp)) >> temp;
				 if(*(sec+tmp))
					 _dpa(*(x1+tmp),*(y1+tmp),fill_pat+100-(!temp),*(DC+tmp),*(siz+tmp));
				 else
					 _dpa(*(x1+tmp),*(y1+tmp),temp,*(DC+tmp),*(siz+tmp));
			 }
			 else
				 _dpa(*(x1+tmp),*(y1+tmp),"",*(DC+tmp),*(siz+tmp));
		 }
		_free(x1);
		_free(y1);
		_free(siz);
		_free(WC);
		_free(DC);
		_free(retauto);
		_free(def);
		_free(sec);
		b_convert **values = val;
		for(;total; --total,++values)
		{
			_remove(*values);
			delete *values;
		}
		_free(val);
		total = 0;
	}
	clearbreak();
}

uc LIST::invoke(uc /*edit_mode*/, keypress &key, us &total, string **&list)
{
	if(!total)
	{
		list = (string **)_alloc(sizeof(string *));
		(*list) = (string *)_add(new string, sizeof(string));
		total = 1;
	}
	us xpos = 0;
	cur = 0;
	top = 0;
	shift = 0;
	redisplaylist();
	time_t lasttime;
	lasttime = time(NULL);
	cursorat(x1,y1);
	cursor(CUR_ON);
	uc tmp;
	uc found;
	uc reason = 0;
	uc quitting = 0;
	do
	{
		key.quick_getscan();
		if(key.scan)
		{
			lasttime = time(NULL);
			found = 0;
			for(tmp = 0; (!found) && (tmp < totbreak); ++tmp)
			{
				if((*(fbreak+tmp))==(key.scan))
				{
					found = 1;
					quitting = 1;
					reason = KEY_BREAK;
				}
			}
			if(!found)
			{
				if(key.asc)
					key.scan = 0;
				switch(key.scan)
				{
					case S_CUR_UP:
									if(cur)
									{
										--cur;
										if(top > cur)
										{
											top = cur;
											redisplaylist();
										}
										cursorat(xpos-shift+x1,cur-top+y1);
									}
									break;
					case S_PG_UP:
									if(cur > (y2-y1))
										cur -= y2-y1;
									else
										cur = 0;
									if(top > cur)
									{
										top = cur;
										redisplaylist();
									}
									cursorat(xpos-shift+x1,cur-top+y1);
									break;
					case S_CUR_DOWN:
									if(cur + 1 < total)
									{
										++cur;
										if(cur > (y2-y1)+top)
										{
											top = cur - (y2-y1);
											redisplaylist();
										}
										cursorat(xpos-shift+x1,cur-top+y1);
									}
									break;
					case S_PG_DOWN:
									if(cur + (y2-y1) < total)
										cur += y2 - y1;
									else
										cur = total - 1;
									if(cur > (y2-y1)+top)
									{
										top = cur - (y2-y1);
										redisplaylist();
									}
									cursorat(xpos-shift+x1,cur-top+y1);
									break;
					case S_CUR_LEFT:
									if(xpos)
									{
										--xpos;
										if(shift > xpos)
										{
											shift = xpos;
											redisplaylist();
										}
										cursorat(xpos-shift+x1,cur-top+y1);
									}
									break;
					case S_CUR_RIGHT:
									if(xpos + 1 < 200)
									{
										++xpos;
										if(xpos > (x2-x1)+shift)
										{
											shift = xpos - (x2-x1);
											redisplaylist();
										}
										cursorat(xpos-shift+x1,cur-top+y1);
									}
									break;
					case S_HOME:
									if(xpos)
									{
										xpos = 0;
										if(shift > xpos)
										{
											shift = xpos;
											redisplaylist();
										}
										cursorat(xpos-shift+x1,cur-top+y1);
									}
									break;
					case S_END:
									if((!(**(list + cur))) > 200)
										xpos = 199;
									else
										xpos = !(**(list + cur));
									if(xpos > (x2-x1)+shift)
									{
										shift = xpos - (x2-x1);
										redisplaylist();
									}
									if(shift > xpos)
									{
										shift = xpos;
										redisplaylist();
									}
									cursorat(xpos-shift+x1,cur-top+y1);
									break;
					case S_INSERT:
									key.setinsert();
									break;
					case S_DELETE:
									_check(list,list+cur);
									if(xpos >= (!(**(list + cur))))
									{
										if(cur + 1 < total)
										{
											_check(list,list+cur+1);
											(**(list+cur)).appendat(xpos,(**(list+cur+1)));
											(**(list+cur)).trunc(400);
											_remove(*(list+cur+1));
											delete *(list+cur+1);
											us t = total - cur - 2;
											string **s1 = list + cur + 1;
											string **s2 = list + cur + 2;
											for(; t; --t, ++s1, ++s2)
											{
												_check(list,s1);
												_check(list,s2);
												*s1 = *s2;
											}
											list = (string **)_realloc(list,sizeof(string *)*(total-1));
											--total;
											redisplaylist();
										}
									}
									else
									{
										(**(list+cur)).deleteletter(xpos);
										redisplay();
									}
									break;
					case S_ENTER:
									if(total < 400)
									{
										list = (string **)_realloc(list,sizeof(string *)*(total+1));
										us _t = total - cur - 1;
										string **_s1 = list + total;
										string **_s2 = list + total - 1;
										for(;_t; --_t, --_s1, --_s2)
										{
											_check(list,_s1);
											_check(list,_s2);
											*_s1 = *_s2;
										}
										*(list + cur + 1) = (string *)_add(new string,sizeof(string));
										if((!(**(list + cur))) > xpos)
										{
											(**(list+cur+1)) = (char *)((**(list+cur)).display() + xpos);
											(**(list+cur)).trunc(xpos);
										}
										++cur;
										if(cur > (y2-y1)+top)
											top = cur - (y2-y1);
										if(xpos)
										{
											xpos = 0;
											if(shift > xpos)
												shift = xpos;
										}
										++total;
										redisplaylist();
										cursorat(xpos-shift+x1,cur-top+y1);
									}
									break;
					case S_TAB:
									us t = 3-(xpos % 3);
									us _t_ = t;
									for(;_t_;--_t_)
										(**(list+cur)).insertletter(xpos,' ');
									redisplay();
									(**(list+cur)).trunc(400);
									if(xpos + t < 200)
									{
										xpos += t;
										if(xpos > (x2-x1)+shift)
										{
											shift = xpos - (x2-x1);
											redisplaylist();
										}
										cursorat(xpos-shift+x1,cur-top+y1);
									}
									break;
					case S_BACK_SPACE:
									if(xpos)
									{
										_check(list,list+cur);
										(**(list+cur)).deleteletter(xpos-1);
										--xpos;
										if(shift > xpos)
										{
											shift = xpos;
											redisplaylist();
										}
										redisplay();
										cursorat(xpos-shift+x1,cur-top+y1);
									}
									else
									{
										if(cur)
										{
											_check(list,list+cur-1);
											_check(list,list+cur);
											us ssiz = !(**(list+cur-1));
											(**(list+cur-1)) += (**(list+cur));
											(**(list+cur-1)).trunc(400);
											_remove(*(list+cur));
											delete *(list + cur);
											string **s1 = list + cur;
											string **s2 = list + cur + 1;
											us t = total - cur - 1;
											for(;t; --t, ++s1, ++s2)
											{
												_check(list,s1);
												_check(list,s2);
												*s1 = *s2;
											}
											--cur;
											if((!(**(list + cur))) > 200)
												xpos = 199;
											else
												xpos = ssiz;
											if(xpos > (x2-x1)+shift)
												shift = xpos - (x2-x1);
											if(shift > xpos)
												shift = xpos;
											--total;
											redisplaylist();
											cursorat(xpos-shift+x1,cur-top+y1);
										}
									}
									break;
					default:
									if(key.asc)
									{
										_check(list,list+cur);
										if(key.insertmode())
											(**(list+cur)).insertletter(xpos,key.asc);
										else
											(**(list+cur)).overwriteletter(xpos,key.asc);
										redisplay();
										(**(list+cur)).trunc(400);
										if(xpos + 1 < 200)
										{
											++xpos;
											if(xpos > (x2-x1)+shift)
											{
												shift = xpos - (x2-x1);
												redisplaylist();
											}
											cursorat(xpos-shift+x1,cur-top+y1);
										}
									}
									break;
				}
			}
		}
		else
		{
			time_t curtime = time(NULL);
			if(curtime > (lasttime + (((ul)quittime) * 60)))
			{
				quitting = 1;
				reason = TIME_OUT;
			}
		}
	} while(!quitting);
	cursor(CUR_OFF);
	return reason;
}

void LIST::addbreak(uc b)
{
	if(totbreak)
		fbreak = (uc *)_realloc(fbreak,sizeof(uc)*(totbreak+1));
	else
		fbreak = (uc *)_alloc(sizeof(uc));
	_check(fbreak,fbreak + totbreak);
	*(fbreak + totbreak) = b;
	++totbreak;
}

void LIST::redisplay()
{
	if(cur < (*_total_))
	{
		if(shift >= (!(**((*_list_)+cur))))
			_dpa(x1,y1+(cur-top),"",WC,(x2-x1)+1);
		else
			_dpa(x1,y1+(cur-top),(char *)((**((*_list_)+cur)).display()+shift),WC,(x2-x1)+1);
	}
	else
		_dpa(x1,y1+(cur-top),"",WC,(x2-x1)+1);
}

void LIST::redisplaylist()
{
	us t = top;
	us y = y1;
	for(; y <= y2; ++y, ++t)
	{
		if(t < (*_total_))
		{
			if(shift >= (!(**((*_list_)+t))))
				_dpa(x1,y,"",WC,(x2-x1)+1);
			else
				_dpa(x1,y,(char *)((**((*_list_)+t)).display()+shift),WC,(x2-x1)+1);
		}
		else
			_dpa(x1,y,"",WC,(x2-x1)+1);
	}
}

void LIST::clear()
{
	quittime = 1;
	if(totbreak)
	{
		_free(fbreak);
		totbreak = 0;
	}
}

void FIELD::addfield(uc &dat, uc x1, uc y1, uc siz, uc WC, uc DC, uc retauto, uc def, uc sec)
{
	c_uc *num = (c_uc *)_add(new c_uc,sizeof(c_uc));
	num->set(dat);
	do_addfield(num,x1,y1,siz,WC,DC,retauto,def,sec);
}

void FIELD::addfield(us &dat, uc x1, uc y1, uc siz, uc WC, uc DC, uc retauto, uc def, uc sec)
{
	c_us *num = (c_us *)_add(new c_us,sizeof(c_us));
	num->set(dat);
	do_addfield(num,x1,y1,siz,WC,DC,retauto,def,sec);
}

void FIELD::addfield(ul &dat, uc x1, uc y1, uc siz, uc WC, uc DC, uc retauto, uc def, uc sec)
{
	c_ul *num = (c_ul *)_add(new c_ul,sizeof(c_ul));
	num->set(dat);
	do_addfield(num,x1,y1,siz,WC,DC,retauto,def,sec);
}

void FIELD::addfield(sc &dat, uc x1, uc y1, uc siz, uc WC, uc DC, uc retauto, uc def, uc sec)
{
	c_sc *num = (c_sc *)_add(new c_sc,sizeof(c_sc));
	num->set(dat);
	do_addfield(num,x1,y1,siz,WC,DC,retauto,def,sec);
}

void FIELD::addfield(si &dat, uc x1, uc y1, uc siz, uc WC, uc DC, uc retauto, uc def, uc sec)
{
	c_si *num = (c_si *)_add(new c_si,sizeof(c_si));
	num->set(dat);
	do_addfield(num,x1,y1,siz,WC,DC,retauto,def,sec);
}

void FIELD::addfield(sl &dat, uc x1, uc y1, uc siz, uc WC, uc DC, uc retauto, uc def, uc sec)
{
	c_sl *num = (c_sl *)_add(new c_sl,sizeof(c_sl));
	num->set(dat);
	do_addfield(num,x1,y1,siz,WC,DC,retauto,def,sec);
}

void FIELD::addfield(string &dat, uc x1, uc y1, uc siz, uc WC, uc DC, uc retauto, uc def, uc sec)
{
	c_string *num = (c_string *)_add(new c_string,sizeof(c_string));
	num->set(dat);
	do_addfield(num,x1,y1,siz,WC,DC,retauto,def,sec);
}

void FIELD::addfield(c_letter &dat, uc x1, uc y1, uc siz, uc WC, uc DC, uc retauto, uc def, uc sec)
{
	c_letter *num = (c_letter *)_add(new c_letter(dat),sizeof(c_letter));
	do_addfield(num,x1,y1,siz,WC,DC,retauto,def,sec);
}

void TOGGLE::addbreak(uc b)
{
	if(totbreak)
		fbreak = (uc *)_realloc(fbreak,sizeof(uc)*(totbreak+1));
	else
		fbreak = (uc *)_alloc(sizeof(uc));
	_check(fbreak,fbreak + totbreak);
	*(fbreak + totbreak) = b;
	++totbreak;
}

void TOGGLE::clear()
{
	quittime = 1;
	if(total)
	{
		_free(list);
		total = 0;
	}
	if(totbreak)
	{
		_free(fbreak);
		totbreak = 0;
	}
}

void TOGGLE::display()
{
	if(total)
	{
		char bracket[4];
		bracket[0] = lb;
		bracket[2] = rb;
		bracket[3] = 0;
		uc y1 = y;
		for(; y1 < y + total; ++y1)
		{
			if(**(list + (y1 - y)))
				bracket[1] = check;
			else
				bracket[1] = ' ';
			_dpa(x,y1,bracket,WC,1);
			_dpa(x+1,y1,bracket+1,SC,1);
			_dpa(x+2,y1,bracket+2,WC,1);
		}
	}
}

void TOGGLE::display(uc n)
{
	if(total)
	{
		char bracket[4];
		bracket[0] = lb;
		bracket[2] = rb;
		bracket[3] = 0;
		uc y1 = y + n;
		if(**(list + (y1 - y)))
			bracket[1] = check;
		else
			bracket[1] = ' ';
		_dpa(x,y1,bracket,WC,1);
		_dpa(x+1,y1,bracket+1,SC,1);
		_dpa(x+2,y1,bracket+2,WC,1);
	}
}

TOGGLE::TOGGLE(uc nx, uc ny, uc nlb, uc nrb, uc ncheck, uc nWC, uc nSC)
{
	quittime = 1;
	x = nx;
	y = ny;
	lb = nlb;
	rb = nrb;
	check = ncheck;
	WC = nWC;
	SC = nSC;
	total = 0;
	totbreak = 0;
}

void TOGGLE::operator+=(uc &any)
{
	if(total)
		list = (uc **)_realloc(list,sizeof(uc *)*(total + 1));
	else
		list = (uc **)_alloc(sizeof(uc *));
	*(list + total) = &any;
	++total;
	display(total-1);
}

uc TOGGLE::invoke(uc edit_mode, keypress &key)
{
	uc pos;
	if(!total)
		_doexit(1);
	switch(edit_mode)
	{
		case UP_EDIT:
							pos = total - 1;
							break;
		case DOWN_EDIT:
							pos = 0;
							break;
		default:
				pos = 0;
			break;
	}
	char bracket[2];
	bracket[0] = 0;
	bracket[1] = 0;

	cursorat(x+1,y+pos);
	cursor(CUR_ON);
	uc found;
	uc reason = 0;
	uc quitting = 0;
	time_t lasttime;
	lasttime = time(NULL);
	display();
	uc tmp;
	do
	{
		key.quick_getscan();
		if(key.scan)
		{
			lasttime = time(NULL);
			found = 0;
			for(tmp = 0; (!found) && (tmp < totbreak); ++tmp)
			{
				if((*(fbreak+tmp))==(key.scan))
				{
					found = 1;
					quitting = 1;
					reason = KEY_BREAK;
				}
			}
			if(!found)
			{
				switch(key.scan)
				{
					case S_BACK_SPACE:
								**(list + pos) = 0;
								*bracket = ' ';
								_dpa(x+1,y+pos,bracket,SC,1);
								break;
					case S_HOME:
								pos = 0;
								cursorat(x+1,y+pos);
								break;
					case S_END:
								pos = total - 1;
								cursorat(x+1,y+pos);
								break;
					case S_DELETE:
								**(list + pos) = 0;
								*bracket = ' ';
								_dpa(x+1,y+pos,bracket,SC,1);
								break;
					case S_ENTER:
								++pos;
								if(pos >= total)
								{
									quitting = 1;
									reason = ENTER_EDIT;
								}
								else
									cursorat(x+1,y+pos);
								break;
					case S_CUR_LEFT:
								reason = UP_EDIT;
								quitting = 1;
								break;
					case S_CUR_UP:
								if(pos)
								{
									--pos;
									cursorat(x+1,y+pos);
								}
								else
								{
									reason = UP_EDIT;
									quitting = 1;
								}
								break;
					case S_CUR_RIGHT:
								reason = DOWN_EDIT;
								quitting = 1;
								break;
					case S_TAB:
								if(pos + 1 < total)
									++pos;
								else
									pos = 0;
								cursorat(x+1,y+pos);
								break;
					case S_CUR_DOWN:
								if(pos + 1 < total)
								{
									++pos;
									cursorat(x+1,y+pos);
								}
								else
								{
									quitting = 1;
									reason = DOWN_EDIT;
								}
								break;
					case S_SPACE:
								if(**(list + pos))
								{
									**(list + pos) = 0;
									*bracket = ' ';
									_dpa(x+1,y+pos,bracket,SC,1);
								}
								else
								{
									**(list + pos) = 1;
									*bracket = check;
									_dpa(x+1,y+pos,bracket,SC,1);
								}
								if(pos + 1 < total)
								{
									++pos;
									cursorat(x+1,y+pos);
								}
								else
								{
									quitting = 1;
									reason = ENTER_EDIT;
								}
								break;
				}
			}
		}
		else
		{
			time_t curtime = time(NULL);
			if(curtime > (lasttime + (((ul)quittime) * 60)))
			{
				quitting = 1;
				reason = TIME_OUT;
			}
		}
	} while(!quitting);
	cursor(CUR_OFF);
	return reason;
}

void SELECT::addbreak(uc b)
{
	if(totbreak)
		fbreak = (uc *)_realloc(fbreak,sizeof(uc)*(totbreak+1));
	else
		fbreak = (uc *)_alloc(sizeof(uc));
	_check(fbreak,fbreak + totbreak);
	*(fbreak + totbreak) = b;
	++totbreak;
}

void SELECT::clear()
{
	quittime = 1;
	total = 0;
	if(totbreak)
	{
		_free(fbreak);
		totbreak = 0;
	}
}

void SELECT::display()
{
	if(total)
	{
		char bracket[4];
		bracket[0] = lb;
		bracket[2] = rb;
		bracket[3] = 0;
		uc y1 = y;
		for(; y1 < y + total; ++y1)
		{
			if((*list)==(y1-y))
				bracket[1] = check;
			else
				bracket[1] = ' ';
			_dpa(x,y1,bracket,WC,1);
			_dpa(x+1,y1,bracket+1,SC,1);
			_dpa(x+2,y1,bracket+2,WC,1);
		}
	}
}

SELECT::SELECT(uc nx, uc ny, uc nlb, uc nrb, uc ncheck, uc nWC, uc nSC, uc ntotal, uc &any)
{
	quittime = 1;
	x = nx;
	y = ny;
	lb = nlb;
	rb = nrb;
	check = ncheck;
	WC = nWC;
	SC = nSC;
	total = ntotal;
	totbreak = 0;
	list = &any;
	display();
}

uc SELECT::invoke(uc, keypress &key)
{
	if(!total)
		_doexit(1);
	if(*list >= total)
		*list = 0;
	uc pos = *list;
	char bracket[2];
	bracket[0] = 0;
	bracket[1] = 0;

	cursorat(x+1,y+pos);
	cursor(CUR_ON);
	uc found;
	uc reason = 0;
	uc quitting = 0;
	time_t lasttime;
	lasttime = time(NULL);
	display();
	uc tmp;
	do
	{
		key.quick_getscan();
		if(key.scan)
		{
			lasttime = time(NULL);
			found = 0;
			for(tmp = 0; (!found) && (tmp < totbreak); ++tmp)
			{
				if((*(fbreak+tmp))==(key.scan))
				{
					found = 1;
					quitting = 1;
					reason = KEY_BREAK;
				}
			}
			if(!found)
			{
				switch(key.scan)
				{
					case S_HOME:
								pos = 0;
								cursorat(x+1,y+pos);
								break;
					case S_END:
								pos = total - 1;
								cursorat(x+1,y+pos);
								break;
					case S_ENTER:
								++pos;
								if(pos >= total)
								{
									quitting = 1;
									reason = ENTER_EDIT;
								}
								else
									cursorat(x+1,y+pos);
								break;
					case S_CUR_LEFT:
								reason = UP_EDIT;
								quitting = 1;
								break;
					case S_BACK_SPACE:
					case S_CUR_UP:
								if(pos)
								{
									--pos;
									cursorat(x+1,y+pos);
								}
								else
								{
									reason = UP_EDIT;
									quitting = 1;
								}
								break;
					case S_CUR_RIGHT:
								reason = DOWN_EDIT;
								quitting = 1;
								break;
					case S_TAB:
								if(pos + 1 < total)
									++pos;
								else
									pos = 0;
								cursorat(x+1,y+pos);
								break;
					case S_CUR_DOWN:
								if(pos + 1 < total)
								{
									++pos;
									cursorat(x+1,y+pos);
								}
								else
								{
									quitting = 1;
									reason = DOWN_EDIT;
								}
								break;
					case S_SPACE:
								if((*list)!=pos)
								{
									*bracket = ' ';
									_dpa(x+1,y+(*list),bracket,SC,1);
									*list = pos;
									*bracket = check;
									_dpa(x+1,y+pos,bracket,SC,1);
								}
								break;
				}
			}
		}
		else
		{
			time_t curtime = time(NULL);
			if(curtime > (lasttime + (((ul)quittime) * 60)))
			{
				quitting = 1;
				reason = TIME_OUT;
			}
		}
	} while(!quitting);
	cursor(CUR_OFF);
	return reason;
}

void BUTTON::addbreak(uc b)
{
	if(totbreak)
		fbreak = (uc *)_realloc(fbreak,sizeof(uc)*(totbreak+1));
	else
		fbreak = (uc *)_alloc(sizeof(uc));
	_check(fbreak,fbreak + totbreak);
	*(fbreak + totbreak) = b;
	++totbreak;
}

void BUTTON::clear()
{
	quittime = 1;
	if(total)
	{
		_free(x);
		_free(y);
		_free(WC);
		_free(SC);
		_free(BG);
		for(uc tmp = 0; tmp < total; ++tmp)
		{
			_remove(*(list + tmp));
			delete *(list + tmp);
		}
		_free(list);
		total = 0;
	}
	if(totbreak)
	{
		_free(fbreak);
		totbreak = 0;
	}
}

void BUTTON::display()
{
	if(total)
	{
		uc y1 = 0;
		for(; y1 < total; ++y1)
			display(y1);
	}
}

void BUTTON::display_blank()
{
	if(total)
	{
		uc y1 = 0;
		for(; y1 < total; ++y1)
			display_blank(y1);
	}
}

void BUTTON::display(uc n)
{
	if(total)
	{
		char str[1];
		if(*selection == n)
			_dpa(*(x+n),*(y+n),**(list+n),*(SC+n),siz);
		else
			_dpa(*(x+n),*(y+n),**(list+n),*(WC+n),siz);
		*str = 220;
		_dpa(*(x+n)+siz,*(y+n),str,*(BG+n),1);
		*str = 223;
		uc tmp = siz;
		for(;tmp;--tmp)
			_dpa(*(x+n)+tmp,*(y+n)+1,str,*(BG+n),1);
	}
}

void BUTTON::add(uc nx, uc ny, uc nWC, uc nSC, uc nBG, char *str)
{
	if(total)
	{
		x = (uc *)_realloc(x,sizeof(uc)*(total+1));
		y = (uc *)_realloc(y,sizeof(uc)*(total+1));
		WC = (uc *)_realloc(WC,sizeof(uc)*(total+1));
		SC = (uc *)_realloc(SC,sizeof(uc)*(total+1));
		BG = (uc *)_realloc(BG,sizeof(uc)*(total+1));
		list = (string **)_realloc(list,sizeof(string *)*(total+1));
	}
	else
	{
		x = (uc *)_alloc(sizeof(uc));
		y = (uc *)_alloc(sizeof(uc));
		WC = (uc *)_alloc(sizeof(uc));
		SC = (uc *)_alloc(sizeof(uc));
		BG = (uc *)_alloc(sizeof(uc));
		list = (string **)_alloc(sizeof(string *));
	}
	_check(x,x+total);
	_check(y,y+total);
	_check(WC,WC+total);
	_check(SC,SC+total);
	_check(BG,BG+total);
	_check(list,list+total);
	*(x+total) = nx;
	*(y+total) = ny;
	*(WC+total) = nWC;
	*(SC+total) = nSC;
	*(BG+total) = nBG;
	*(list+total) = (string *)_add(new string,sizeof(string));
	_pcheck(*(list+total));
	(**(list+total)) = str;
	++total;
	display_blank(total-1);
}

uc BUTTON::invoke(uc edit_mode, keypress &key)
{
	if(!total)
		_doexit(1);
	switch(edit_mode)
	{
		case UP_EDIT:
							pos = total - 1;
							break;
		case DOWN_EDIT:
							pos = 0;
							break;
		default:
				pos = 0;
			break;
	}
	*selection = pos;
	cursor(CUR_OFF);
	uc found;
	uc reason = 0;
	uc quitting = 0;
	time_t lasttime;
	lasttime = time(NULL);
	display();
	uc tmp;
	do
	{
		key.quick_getscan();
		if(key.scan)
		{
			lasttime = time(NULL);
			found = 0;
			for(tmp = 0; (!found) && (tmp < totbreak); ++tmp)
			{
				if((*(fbreak+tmp))==(key.scan))
				{
					found = 1;
					quitting = 1;
					reason = KEY_BREAK;
				}
			}
			if(!found)
			{
				switch(key.scan)
				{
					case S_HOME:
								*selection = 0;
								display(pos);
								display(0);
								pos = 0;
								break;
					case S_END:
								*selection = total - 1;
								display(pos);
								display(*selection);
								pos = *selection;
								break;
					case S_SPACE:
					case S_ENTER:
								quitting = 1;
								reason = ENTER_EDIT;
								break;
					case S_CUR_LEFT:
					case S_CUR_UP:
								if(pos)
								{
									*selection = pos - 1;
									display(pos);
									--pos;
									display(pos);
								}
								else
								{
									reason = UP_EDIT;
									quitting = 1;
								}
								break;
					case S_TAB:
								uc oldpos = pos;
								if(pos + 1 < total)
									++pos;
								else
									pos = 0;
								*selection = pos;
								display(oldpos);
								display(pos);
								break;
					case S_CUR_RIGHT:
					case S_CUR_DOWN:
								if(pos + 1 < total)
								{
									*selection = pos + 1;
									display(pos);
									++pos;
									display(pos);
								}
								else
								{
									quitting = 1;
									reason = DOWN_EDIT;
								}
								break;
				}
			}
		}
		else
		{
			time_t curtime = time(NULL);
			if(curtime > (lasttime + (((ul)quittime) * 60)))
			{
				quitting = 1;
				reason = TIME_OUT;
			}
		}
	} while(!quitting);
	display_blank();
	return reason;
}



void BOUNCE::addbreak(uc b)
{
	if(totbreak)
		fbreak = (uc *)_realloc(fbreak,sizeof(uc)*(totbreak+1));
	else
		fbreak = (uc *)_alloc(sizeof(uc));
	_check(fbreak,fbreak + totbreak);
	*(fbreak + totbreak) = b;
	++totbreak;
}

void BOUNCE::clear()
{
	quittime = 1;
	if(totbreak)
	{
		_free(fbreak);
		totbreak = 0;
	}
}

void BOUNCE::display(us num, uc *&tag, us &total, string **&list)
{
	uc col;
	if(num < total)
	{
		if(allowtag)
		{
			if(*(tag+num))
			{
				if(cur == num)
					col = THC;
				else
					col = TWC;
			}
			else
			{
				if(cur == num)
					col = HC;
				else
					col = WC;
			}
		}
		else
		{
			if(cur == num)
				col = HC;
			else
				col = WC;
		}
		_dpa(x1,y1+num-top,**(list+num),col,(x2-x1)+1);
	}
	else
		_dpa(x1,y1+num-top,"",WC,(x2-x1)+1);
}

void BOUNCE::display(us num)
{
	display(num,*_tag,*_total,*_list);
}

void BOUNCE::displaylist(uc *&tag, us &total, string **&list)
{
	us t = (y2-y1)+1;
	us n = 0;
	for(; n < t; ++n)
		display(top+n,tag,total,list);
}

void BOUNCE::displaylist()
{
	displaylist(*_tag,*_total,*_list);
}

uc BOUNCE::usecurrent()
{
	if(allowtag)
	{
		uc found = 0;
		us t = 0;
		for(;t < (*_total);++t)
			found |= (*((*_tag)+t));
		return !found;
	}
	else
		return 1;
}

uc BOUNCE::invoke(uc/* edit_mode*/, keypress &key, uc *&tag, us &total, string **& /*list*/)
{
	if(!total)
		_doexit(1);
	cursor(CUR_OFF);
	uc found;
	uc reason = 0;
	uc quitting = 0;
	time_t lasttime;
	lasttime = time(NULL);
	displaylist();
	uc tmp;
	do
	{
		key.quick_getscan();
		if(key.scan)
		{
			lasttime = time(NULL);
			found = 0;
			for(tmp = 0; (!found) && (tmp < totbreak); ++tmp)
			{
				if((*(fbreak+tmp))==(key.scan))
				{
					found = 1;
					quitting = 1;
					reason = KEY_BREAK;
				}
			}
			if(!found)
			{
				switch(key.scan)
				{
					case S_HOME:
								if(cur)
								{
									us last = cur;
									cur = 0;
									display(last);
									if(top)
									{
										top = 0;
										displaylist();
									}
									display(cur);
								}
								break;
					case S_END:
								if(cur + 1 != total)
								{
									us last = cur;
									cur = total - 1;
									display(last);
									if(top + (y2 - y1) < cur)
									{
										top = cur - (y2 - y1);
										displaylist();
									}
									display(cur);
								}
								break;
					case 0x1e:
								if(allowtag)
								{
									us tot;
									for(tot = 0; tot < total; ++tot)
										*(tag + tot) = 1;
									displaylist();
								}
								break;
					case 0x14:
								if(allowtag)
								{
									us tot;
									for(tot = 0; tot < total; ++tot)
										*(tag + tot) = !(*(tag+tot));
                           displaylist();
								}
								break;
					case S_SPACE:
					case S_INSERT:
								if(allowtag)
								{
									*(tag + cur) = !(*(tag + cur));
									display(cur);
								}
								break;
					case S_ENTER:
								if(allowtag)
								{
									if(usecurrent())
										*(tag + cur) = 1;
								}
								quitting = 1;
								reason = ENTER_EDIT;
								break;
					case S_CUR_UP:
								if(cur)
								{
									us last = cur;
									--cur;
									display(last);
									if(cur < top)
									{
										top = cur;
										displaylist();
									}
									display(cur);
								}
								break;
					case S_CUR_DOWN:
								if(cur + 1 < total)
								{
									us last = cur;
									++cur;
									display(last);
									if(top + (y2 - y1) < cur)
									{
										top = cur - (y2 - y1);
										displaylist();
									}
									display(cur);
								}
								break;
					case S_PG_UP:
								us last = cur;
								if(cur > (y2-y1))
									cur -= y2-y1;
								else
									cur = 0;
								display(last);
								if(top > cur)
								{
									top = cur;
									displaylist();
								}
								display(cur);
								break;
					case S_PG_DOWN:
								last = cur;
								if(cur + (y2-y1) < total)
									cur += y2 - y1;
								else
									cur = total - 1;
								display(last);
								if(cur > (y2-y1)+top)
								{
									top = cur - (y2-y1);
									displaylist();
								}
								display(cur);
								break;
				}
			}
		}
		else
		{
			time_t curtime = time(NULL);
			if(curtime > (lasttime + (((ul)quittime) * 60)))
			{
				quitting = 1;
				reason = TIME_OUT;
			}
		}
	} while(!quitting);
	return reason;
}

