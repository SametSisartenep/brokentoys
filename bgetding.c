#include <u.h>
#include <libc.h>
#include <bio.h>

typedef struct
{
	double x, y, z;
} Vec;

void
usage(void)
{
	fprint(2, "usage: %s [file]\n", argv0);
	exits("usage");
}

void
main(int argc, char *argv[])
{
	Biobuf *bin;
	Vec v;
	int fd, r;

	fd = 0;
	ARGBEGIN{
	default: usage();
	}ARGEND;
	if(argc > 1)
		usage();
	else if(argc == 1){
		fd = open(argv[0], OREAD);
		if(fd < 0)
			sysfatal("open: %r");
	}
	bin = Bfdopen(fd, OREAD);
	if(bin == nil)
		sysfatal("Bfdopen: %r");
	r = Bgetd(bin, &v.x);
	print("got %g r=%d\n", v.x, r);
	r = Bgetd(bin, &v.y);
	print("got %g r=%d\n", v.y, r);
	r = Bgetd(bin, &v.z);
	print("got %g r=%d\n", v.z, r);
	Bterm(bin);
	exits(0);
}
