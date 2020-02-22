/* non-recursive factorial */
#include <u.h>
#include <libc.h>

void
printdec(int n)
{
	char s[16], *p;
	int r;

	p = s+16;
	*--p = '\n';
	for(;;){
		r = n%10;
		*--p = '0'+r;
		n /= 10;
		if(n == 0 || p == s)
			break;
	}
	write(1, p, s+sizeof(s)-p);
}

int
fact(int n)
{
	int a;

	a = n;
repeat:
	if(n <= 0)
		return 0;
	else if(n == 1)
		return a;
	a *= --n;
	goto repeat;
}

void
usage(void)
{
	fprint(2, "usage: %s n\n", argv0);
	exits("usage");
}

void
main(int argc, char *argv[])
{
	int n;

	ARGBEGIN{
	default: usage();
	}ARGEND;
	if(argc != 1)
		usage();
	n = strtol(argv[0], nil, 0);
	printdec(fact(n));
	exits(0);
}
