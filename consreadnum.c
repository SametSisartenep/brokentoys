#include <u.h>
#include <libc.h>

void
main()
{
	char tmp[64];
	int size;

	size = 2*1024*1024*1024;
	if(size > sizeof tmp || size < 0)
		size = sizeof tmp;
	snprint(tmp, sizeof tmp, "%*lud", size-1, 409234UL);
	tmp[size-1] = ' ';
	print("%s\n", tmp);
	exits(nil);
}
