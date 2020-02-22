#include <u.h>
#include <libc.h>

vlong t0, Δt;

double
fac(double n)
{
	double Π;

	Π = 1;
	assert(n > 0);
	while(n > 1)
		Π *= n--;
	return Π;
}

double
bincoeff(double n, double k)
{
	assert(k <= n);
	return fac(n)/(fac(k)*fac(n-k));
}

double
bincoeffmul(double n, double k)
{
	double Π;
	int i;

	assert(k <= n);
	Π = 1;
	for(i = 1; i <= k; i++)
		Π *= (n + 1 - i)/i;
	return Π;
}

void
usage(void)
{
	fprint(2, "usage: bincoeff n k\n");
	exits("usage");
}

void
main(int argc, char *argv[])
{
	double n, k, bc;

	if(argc != 3)
		usage();
	n = strtod(argv[1], nil);
	k = strtod(argv[2], nil);
	t0 = nsec();
	bc = bincoeff(n, k);
	Δt = nsec()-t0;
	print("method 1: %g (%lldns)\n", bc, Δt);
	t0 = nsec();
	bc = bincoeffmul(n, k);
	Δt = nsec()-t0;
	print("method 2: %g (%lldns)\n", bc, Δt);
	exits(nil);
}
