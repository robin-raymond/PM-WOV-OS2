#if defined(__DEBUG__)
	void _gfree(void *, us, char *);
	void _gcheck(void *,void *, us, char *);
	void _acheck(void far *, us, char *);
	void _gcall(us, char *);
	void *_gadd(void *,ul,us, char *);
	void _gremove(void *,us,char *);
	void *_galloc(ul,us,char *);
	void *_grealloc(void *,ul,us,char *);
	#define _HERE_ __LINE__,__FILE__
	#define _alloc(x) _galloc(x,__LINE__,__FILE__)
	#define _realloc(x,y) _grealloc(x,y,__LINE__,__FILE__)
	#define _free(x) _gfree(x,__LINE__,__FILE__)
	#define _call() _gcall(__LINE__,__FILE__)
	#define _check(x,y) _gcheck(x,y,__LINE__,__FILE__)
	#define _pcheck(x) _acheck(x,__LINE__,__FILE__)
	#define _add(x,y) _gadd(x,y,__LINE__,__FILE__)
	#define _remove(x) _gremove(x,__LINE__,__FILE__)
	#include "keyboard.h"
#else
	#define _alloc farmalloc
	#define _free farfree
	#define _realloc farrealloc
	#define _HERE_
	#define _call()
	#define _check(x,y)
	#define _pcheck(x)
	#define _add(x,y) (x)
	#define _remove(x)
#endif
#include <alloc.h>
void _doexit(uc);