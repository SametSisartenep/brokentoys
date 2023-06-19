#include <u.h>
#include <libc.h>
#include <geometry.h>

void
printm(Matrix m)
{
	int i, j;

	for(i = 0; i < 3; i++){
		for(j = 0; j < 3; j++)
			print("\t%g", m[i][j]);
		print("\n");
	}
}

Point2
rfxform(Point2 p, RFrame rf)
{
	Matrix m = {
		rf.bx.x, rf.by.x, 0,
		rf.bx.y, rf.by.y, 0,
		0, 0, 1
	};
	invm(m);
//	printm(m);
	return xform(subpt2(p, rf.p), m);
}

Point2
invrfxform(Point2 p, RFrame rf)
{
	Matrix m = {
		rf.bx.x, rf.by.x, 0,
		rf.bx.y, rf.by.y, 0,
		0, 0, 1
	};
//	printm(m);
	return addpt2(xform(p, m), rf.p);
}

void
usage(void)
{
	fprint(2, "usage: %s srcp srcbx srcby dstp dstbx dstby p\n", argv0);
	exits("usage");
}

void
main(int argc, char *argv[])
{
	char *s;
	RFrame src, dst;
	Point2 p;

	GEOMfmtinstall();
	ARGBEGIN{
	default: usage();
	}ARGEND;
	if(argc != 7)
		usage();

	s = argv[0]; src.p.x = strtod(s, &s); src.p.y = strtod(s, &s); src.p.w = strtod(s, &s);
	s = argv[1]; src.bx.x = strtod(s, &s); src.bx.y = strtod(s, &s); src.bx.w = 0;
	s = argv[2]; src.by.x = strtod(s, &s); src.by.y = strtod(s, &s); src.by.w = 0;

	s = argv[3]; dst.p.x = strtod(s, &s); dst.p.y = strtod(s, &s); dst.p.w = strtod(s, &s);
	s = argv[4]; dst.bx.x = strtod(s, &s); dst.bx.y = strtod(s, &s); dst.bx.w = 0;
	s = argv[5]; dst.by.x = strtod(s, &s); dst.by.y = strtod(s, &s); dst.by.w = 0;

	s = argv[6]; p.x = strtod(s, &s); p.y = strtod(s, &s); p.w = strtod(s, &s);

	print("src\n\tp %v\n\tbx %v\n\tby %v\n", src.p, src.bx, src.by);
	print("dst\n\tp %v\n\tbx %v\n\tby %v\n", dst.p, dst.bx, dst.by);
	print("p  %v\n", p);
	print("p' %v\n", rframexform(invrframexform(p, src), dst));
//	print("S→D %v\n", rfxform(invrfxform(p, src), dst));
//	print("D→S %v\n", rfxform(invrfxform(p, dst), src));
//	print("S→ %v\n", invrfxform(p, src));
//	print("→S %v\n", rfxform(p, src));
//	print("D→ %v\n", invrfxform(p, dst));
//	print("→D %v\n", rfxform(p, dst));

	exits(nil);
}
