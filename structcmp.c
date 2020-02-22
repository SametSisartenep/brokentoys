#include <u.h>
#include <libc.h>

enum {
	A = 1,
	B = 2,
	C = 4,
	D = 8
};

typedef struct Fruit Fruit;
struct Fruit {
	char *name;
	int vitamins;
};

void
main()
{
	Fruit apple, lemon, apple2;

	apple = (Fruit){"apple", C};
	lemon = (Fruit){"lemon", B|C};
	apple2 = (Fruit){"apple", C};
	if(apple == apple)
		fprint(2, "apple equals apple\n");
	if(apple == apple2)
		fprint(2, "apple equals apple2\n");
	if(apple == lemon)
		fprint(2, "apple equals lemon, really?\n");
	exits(0);
}
