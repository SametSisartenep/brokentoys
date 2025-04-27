#include <u.h>
#include <libc.h>
#include <draw.h>
#include <memdraw.h>

#define min(a,b)	((a)<(b)?(a):(b))
#define max(a,b)	((a)>(b)?(a):(b))
#define clamp(a,b,c)	min(max(a,b),c)

static int saturate;

static void
usage(void)
{
	fprint(2, "usage: %s img1 img2\n", argv0);
	exits(nil);
}

void
main(int argc, char *argv[])
{
	Memimage *img1, *img2;
	uchar *p1, *p2;
	uchar *p1e;
	ulong c;
	int fd;

	ARGBEGIN{
	case 's': saturate++; break;
	default: usage();
	}ARGEND;
	if(argc != 2)
		usage();

	fd = open(argv[0], OREAD);
	if(fd < 0)
		sysfatal("open: %r");
	img1 = readmemimage(fd);
	if(img1 == nil)
		sysfatal("readmemimage: %r");
	close(fd);

	fd = open(argv[1], OREAD);
	if(fd < 0)
		sysfatal("open: %r");
	img2 = readmemimage(fd);
	if(img2 == nil)
		sysfatal("readmemimage: %r");
	close(fd);

	if(img1->chan != img2->chan || !eqrect(img1->r, img2->r))
		sysfatal("images shapes differ");

	p1 = img1->data->bdata;
	p2 = img2->data->bdata;
	p1e = p1 + Dx(img1->r)*Dy(img1->r)*img1->nchan;
	while(p1 < p1e){
		c = sqrt(*p1**p1 + *p2**p2++);
		if(saturate)
			c = clamp(c, 0, 0xFF);
		*p1++ = c;
	}

	if(writememimage(1, img1) < 0)
		sysfatal("writememimage: %r");

	freememimage(img2);
	freememimage(img1);
	exits(nil);
}
