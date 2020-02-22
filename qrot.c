#include <u.h>
#include <libc.h>

#define DEG 0.01745329251994330

typedef struct Quaternion Quaternion;
struct Quaternion {
	double r, i, j, k;
};
typedef struct Point3 Point3;
struct Point3 {
	double x, y, z, w;
};

Point3
Vec3(double x, double y, double z)
{
	return (Point3){x, y, z, 0};
}

Point3
addpt3(Point3 a, Point3 b)
{
	return (Point3){a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};
}

Point3
mulpt3(Point3 p, double s)
{
	return (Point3){p.x*s, p.y*s, p.z*s, p.w*s};
}

double
dotvec3(Point3 a, Point3 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Point3
crossvec3(Point3 a, Point3 b)
{
	return (Point3){
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x,
		0
	};
}

Quaternion
mulq(Quaternion q, Quaternion r)
{
	Point3 qv, rv, tmp;

	qv = Vec3(q.i, q.j, q.k);
	rv = Vec3(r.i, r.j, r.k);
	tmp = addpt3(addpt3(mulpt3(rv, q.r), mulpt3(qv, r.r)), crossvec3(qv, rv));
	return (Quaternion){q.r*r.r - dotvec3(qv, rv), tmp.x, tmp.y, tmp.z};
}

Quaternion
invq(Quaternion q)
{
	double len²;

	len² = q.r*q.r + q.i*q.i + q.j*q.j + q.k*q.k;
	if(len² == 0)
		return (Quaternion){0, 0, 0, 0};
	return (Quaternion){q.r/len², -q.i/len², -q.j/len², -q.k/len²};
}

#pragma varargck type "q" Quaternion
int
qfmt(Fmt *f)
{
	Quaternion q;

	q = va_arg(f->args, Quaternion);
	return fmtprint(f, "[%g %g %g %g]", q.r, q.i, q.j, q.k);
}

void
main()
{
	Quaternion q;
	double c, s;

	fmtinstall('q', qfmt);
	c = cos(45*DEG);
	s = sin(45*DEG);
	q = (Quaternion){c, s, s, s};
	print("q %q\nq⁻¹ %q\nqq⁻¹ %q\n", q, invq(q), mulq(q, invq(q)));
	exits(nil);
}
