#include <u.h>
#include <libc.h>

#define round(s, sz)	((s)+((sz)-1)&~((sz)-1))

static void
usage(void)
{
	fprint(2, "usage: bitround number\n");
	exits("usage");
}

void
main(int argc, char *argv[])
{
	int i;

	if(argc != 2)
		usage();
	i = strtol(argv[1], nil, 0);
	print("i0 %b (%d)\n", i, i);
	i = round(i, sizeof(int));
	print("i1 %b (%d)\n", i, i);
	exits(0);
}
