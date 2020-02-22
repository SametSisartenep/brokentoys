#include <u.h>
#include <libc.h>

void
main()
{
	char buf[128];
	int n;

	n = snprint(buf, sizeof buf, "%s:%d\n", "actual.out", 2);
	print("%d\n", n);
	exits(0);
}
