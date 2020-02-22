#include <u.h>
#include <libc.h>

typedef struct Point Point;
struct Point
{
	int x, y;
};

typedef struct Pointd Pointd;
struct Pointd
{
	double x, y, w;
};

Point
Pt(int x, int y)
{
	return (Point){x, y};
}

Pointd
Ptd(double x, double y, double w)
{
	return (Pointd){x, y, w};
}

vlong t0;

void
main()
{
	Point p;
	Pointd pd;

	t0 = nsec();
	p = (Point){2, 3};
	fprint(2, "p1 %lldnsec\n", nsec()-t0);
	t0 = nsec();
	p = Pt(2, 3);
	fprint(2, "p2 %lldnsec\n", nsec()-t0);
	t0 = nsec();
	pd = (Pointd){2.0, 3.0, 1.0};
	fprint(2, "pd1 %lldnsec\n", nsec()-t0);
	t0 = nsec();
	pd = Ptd(2.0, 3.0, 1.0);
	fprint(2, "pd2 %lldnsec\n", nsec()-t0);
	exits(0);
}
