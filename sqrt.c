#include <u.h>
#include <libc.h>

int iters;

/*
 * Heron's method to compute the √
 *
 * iteratively do
 * 	x1 = ½(x0 + n/x0)
 * since
 * 	lim M→∞ (xM) = √n
 */
//double
//√(double n)
//{
//	int i;
//	double x;
//
//	x = 2;
//	for(i = 0; i < iters; i++)
//		x = 0.5*(x + n/x);
//	return x;
//}

double
√(double n)
{
	double x0, x;

	if(n == 0)
		return 0;

	x0 = -1;
	x = n > 1? n/2: 1;	/* initial estimate */
	/*
	 * take advantage of the computer's discreteness
	 * to get the most accurate result.
	 */
	while(x0 != x){
		x0 = x;
		x = 0.5*(x0 + n/x0);
		iters++;
	}
	return x;
}

void
usage(void)
{
	fprint(2, "usage: %s number [prec]\n", argv0);
	exits("usage");
}

void
main(int argc, char *argv[])
{
	int prec;
	double n;

	prec = 10;
	ARGBEGIN{
	default: usage();
	}ARGEND
	if(argc < 1)
		usage();

	n = strtod(argv[0], nil);
	if(n < 0)
		sysfatal("too complex");
	if(argc > 2)
		prec = strtoul(argv[1], nil, 10);
	print("√%g = %.*f (took %d iterations)\n", n, prec, √(n), iters);
	exits(nil);
}
