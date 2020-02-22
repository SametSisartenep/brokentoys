#include <u.h>
#include <libc.h>

void
swap(char *a, char *b)
{
	char tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void
srev(char *s, char *e)
{
	while(s < e)
		swap(s++, e--);
}

void
printdec(int n)
{
	char buf[16];
	char *p, *e;
	int r;

	p = buf;
	e = buf+sizeof(buf)-1;
	while(n > 0 && p < e){
		r = n%10;
		*p++ = '0'+r;
		n /= 10;
	}
	*p = 0;
	print("%s\n", buf);
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
	int n;

	ARGBEGIN{
	default: usage();
	}ARGEND
	if(argc != 1)
		usage();
	srev(argv[0], argv[0]+strlen(argv[0])-1);
	n = strtol(argv[0], nil, 10);
	printdec(n);
	exits(0);
}
