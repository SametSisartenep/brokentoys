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
rev(char *s, ulong len)
{
	char *e;

	e = s+len;
	while(s < e)
		swap(s++, --e);
}

void
usage(void)
{
	fprint(2, "usage: %s text ...\n", argv0);
	exits("usage");
}

void
main(int argc, char *argv[])
{
	int first;

	first = 1;
	ARGBEGIN{
	default: usage();
	}ARGEND;
	if(argc < 1)
		usage();
	while(argc--){
		rev(argv[argc], strlen(argv[argc]));
		print(first ? first--, "%s" : " %s", argv[argc]);
	}
	print("\n");
	exits(nil);
}
