#include <u.h>
#include <libc.h>
#include <geometry.h>

int
lineXline(Point2 *r, Point2 p0, Point2 p1, Point2 p2, Point2 p3)
{
	Point2 dp[2], xp;
	double m[2], c[2];

	dp[0] = subpt2(p1, p0); m[0] = dp[0].y/dp[0].x; c[0] = p0.y - m[0]*p0.x;
	fprint(2, "#1 := %gx + %g\n", m[0], c[0]);
	dp[1] = subpt2(p3, p2); m[1] = dp[1].y/dp[1].x; c[1] = p2.y - m[1]*p2.x;
	fprint(2, "#2 := %gx + %g\n", m[1], c[1]);

	if(m[0] == m[1]){
		if(c[0] == c[1]){	/* coincident */
			r[0] = p0;
			r[1] = p1;
			return 2;
		}
		/* parallel */
		return 0;
	}

	/* intersecting */
	xp.x = (c[1]-c[0])/(m[0]-m[1]);
	xp.y = m[0]*xp.x + c[0];
	xp.w = 1;
	r[0] = xp;
	return 1;
}

void
usage(void)
{
	fprint(2, "usage: %s x0 y0 x1 y1 x2 y2 x3 y3\n", argv0);
	exits("usage");
}

void
main(int argc, char *argv[])
{
	Point2 p0, p1, p2, p3, r[2];
	int n;

	GEOMfmtinstall();
	ARGBEGIN{
	default: usage();
	}ARGEND
	if(argc != 8)
		usage();

	p0.x = strtod(argv[0], nil); p0.y = strtod(argv[1], nil);
	p1.x = strtod(argv[2], nil); p1.y = strtod(argv[3], nil);
	p2.x = strtod(argv[4], nil); p2.y = strtod(argv[5], nil);
	p3.x = strtod(argv[6], nil); p3.y = strtod(argv[7], nil);

	n = lineXline(r, p0, p1, p2, p3);
	while(n--)
		print("p%d: %v\n", n, r[n]);

	exits(nil);
}
