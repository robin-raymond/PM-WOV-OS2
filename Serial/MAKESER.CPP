#include <stdio.h>
#include <iostream.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned long ul;
typedef unsigned short us;
typedef unsigned char uc;


ul toserialno(uc *str)
{
	ul no = 1500;
	if(*str)
	{
		us s = strlen((char *)str);
		uc *s1 = str;
		us pos = 0;
		us old = *s1;
		for(;pos < s; ++pos, ++s1)
		{
			no += (pos+1)*((*s1)^134);
			if(old)
				no *= old;
			old = *s1;
		}
	}
	return no;
}

int main(int n, uc **argv)
{
	if(n > 2)
	{
		ul ser = toserialno(*(argv+1));
		char *trick = "RAB#TTY_EIEIO";
		FILE *f;
		f = fopen((char *)(*(argv+2)),"wb+");
		if(f)
		{
			fwrite(&ser,sizeof(ul),1,f);
			us tmp;
			us j;
			for(tmp = 0; tmp < 157; ++tmp)
			{
				if(rand()%2)
					j = rand();
				else
					j = rand()%26+'A';
				fwrite(&j,sizeof(us),1,f);
				if(tmp == 100)
					fwrite(trick,sizeof(uc),14,f);
			}
			fclose(f);
			cout << "Created: " << *(argv+2) << "\n";
			cout << "Serial #: " << *(argv+1) << "\n";
			cout << "Real   #: " << ser << "\n";
		}
	}
	return 0;
}