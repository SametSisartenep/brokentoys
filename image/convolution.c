#include <u.h>
#include <libc.h>
#include <bio.h>
#include <draw.h>
#include <memdraw.h>

#define min(a,b)	((a)<(b)?(a):(b))
#define max(a,b)	((a)>(b)?(a):(b))
#define clamp(a,b,c)	min(max(a,b),c)

static int dim;
static int saturate;

static void
fprintm(int fd, double *m, int dim)
{
	int i, j;

	for(j = 0; j < dim; j++)
	for(i = 0; i < dim; i++)
		fprint(fd, "%g%c", m[j*dim+i], i == dim-1? '\n': '\t');
}

static char *
getline(Biobuf *b)
{
	char *line;

	if((line = Brdline(b, '\n')) != nil)
		line[Blinelen(b)-1] = 0;
	return line;
}

static double *
readkernel(int fd)
{
	Biobuf *bin;
	double *kern;
	char *line, *f[10];
	int nf, i, j;

	bin = Bfdopen(fd, OREAD);
	if(bin == nil)
		sysfatal("Bfdopen: %r");
	do{
		line = getline(bin);
		if(line == nil)
			sysfatal("Brdline: %r");
		dim = tokenize(line, f, nelem(f));
	}while(dim < 1);
	kern = malloc(dim*dim*sizeof(double));
	if(kern == nil)
		sysfatal("malloc: %r");
	for(j = i = 0; i < dim; i++)
		kern[j*dim+i] = strtod(f[i], nil);
	j++;

	while((line = getline(bin)) != nil){
		if((nf = tokenize(line, f, nelem(f))) < 1)
			continue;
		if(nf != dim)
			sysfatal("expected a %dx%d matrix", dim, dim);
		for(i = 0; i < dim; i++)
			kern[j*dim+i] = strtod(f[i], nil);
		j++;
	}
	Bterm(bin);

	if(j != dim)
		sysfatal("expected a %dx%d matrix", dim, dim);

	return kern;
}

static double
coeffsum(double *k, int dim)
{
	double s;
	int i, j;

	s = 0;
	for(j = 0; j < dim; j++)
	for(i = 0; i < dim; i++)
		s += k[j*dim+i];
	return s;
}

static void
normalize(double *k, int dim)
{
	double avg;
	int i, j;

	avg = coeffsum(k, dim);
	avg /= dim*dim;
	if(avg == 0)
		avg = 1;
	for(j = 0; j < dim; j++)
	for(i = 0; i < dim; i++)
		k[j*dim+i] /= avg;
}

static double *
reverse(double *k, int dim)
{
	double *ck;
	int i, j;

	ck = malloc(dim*dim*sizeof(double));
	if(ck == nil)
		sysfatal("malloc: %r");

	for(j = 0; j < dim; j++)
	for(i = 0; i < dim; i++)
		ck[j*dim+i] = k[(dim-j-1)*dim+(dim-i-1)];
	return ck;
}

static void
modulate(double *d, double *s, int dim)
{
	int i, j;

	for(j = 0; j < dim; j++)
	for(i = 0; i < dim; i++)
		d[j*dim+i] *= s[j*dim+i];
}

static double
convolve(double *d, double *s, int dim)
{
	int i, j;
	double denom, r;

	denom = coeffsum(s, dim);
	denom = denom == 0? 1: 1/denom;

	modulate(d, s, dim);
	r = 0;
	for(j = 0; j < dim; j++)
	for(i = 0; i < dim; i++)
		r += d[j*dim+i];
	return fabs(r*denom);
}

static uchar
sample(Memimage *i, Point p, int off)
{
	if(p.x < i->r.min.x || p.y < i->r.min.y
	|| p.x >= i->r.max.x || p.y >= i->r.max.y)
		return 0;	/* edge handler: constant */
	return *(byteaddr(i, p) + off);
}

static void
imgconvolution(Memimage *d, Memimage *s, double *k, int dim)
{
	double **im;
	Rectangle imr;
	Point imc, p, cp;
	double c;
	int i;

	im = malloc(d->nchan*sizeof(double*));
	if(im == nil)
		sysfatal("malloc: %r");
	for(i = 0; i < d->nchan; i++){
		im[i] = malloc(dim*dim*sizeof(double));
		if(im[i] == nil)
			sysfatal("malloc: %r");
	}

	imr = Rect(0,0,dim,dim);
	imc = Pt(dim/2, dim/2);

	for(p.y = s->r.min.y; p.y < s->r.max.y; p.y++)
	for(p.x = s->r.min.x; p.x < s->r.max.x; p.x++){
		for(cp.y = imr.min.y; cp.y < imr.max.y; cp.y++)
		for(cp.x = imr.min.x; cp.x < imr.max.x; cp.x++){
			for(i = 0; i < d->nchan; i++)
				im[i][cp.y*dim + cp.x] = sample(s, addpt(p, subpt(cp, imc)), i);
		}
		for(i = 0; i < d->nchan; i++){
			c = convolve(im[i], k, dim);
			if(saturate)
				c = clamp(c, 0, 0xFF);
			*(byteaddr(d, p) + i) = c;
		}
	}

	for(i = 0; i < d->nchan; i++)
		free(im[i]);
	free(im);
}


static void
usage(void)
{
	fprint(2, "usage: %s kernfile\n", argv0);
	exits("usage");
}

void
main(int argc, char *argv[])
{
	Memimage *in, *out;
	double *kern, *ckern;
	int fd;

	ARGBEGIN{
	case 's': saturate++; break;
	default: usage();
	}ARGEND;
	if(argc != 1)
		usage();

	fd = open(argv[0], OREAD);
	if(fd < 0)
		sysfatal("open: %r");
	kern = readkernel(fd);
	close(fd);
	ckern = reverse(kern, dim);
	free(kern);
	kern = ckern;

	in = readmemimage(0);
	if(in == nil)
		sysfatal("readmemimage: %r");

	out = allocmemimage(in->r, in->chan);
	if(out == nil)
		sysfatal("allocmemimage: %r");

	imgconvolution(out, in, kern, dim);
	if(writememimage(1, out) < 0)
		sysfatal("writememimage: %r");

	freememimage(out);
	freememimage(in);
	free(kern);
	exits(nil);
}
