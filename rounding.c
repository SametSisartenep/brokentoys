#include <u.h>
#include <libc.h>

double
roundf(double n)
{
	return floor(n + 0.5);
}

int
roundi(double n)
{
	return n+0.5;
}

void
usage(void)
{
	fprint(2, "usage: %s number\n", argv0);
	exits("usage");
}

void
main(int argc, char *argv[])
{
	double n;

	ARGBEGIN{
	default: usage();
	}ARGEND;
	if(argc != 1)
		usage();
	n = strtod(argv[0], nil);
	print("%g\n", roundf(n));
	print("%d\n", roundi(n));
	exits(0);
}
