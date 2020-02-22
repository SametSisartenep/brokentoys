#include <u.h>
#include <libc.h>

void
main()
{
	char buf[];

	buf = malloc(128);
	print("%d\n", nelem(buf));
	exits(nil);
}
