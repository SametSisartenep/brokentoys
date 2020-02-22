#include <u.h>
#include <libc.h>

enum {
	NF,
	NM
};

typedef struct Name Name;
struct Name
{
	char *name;
	int sex;
};

void
genrandname(char *d, ulong len)
{
	Name names[] = {
		"mariana", NF,
		"jerca", NF,
		"repa", NF,
		"jaca", NF,
		"pinta", NF,
		"manolo", NM,
		"eustaquio", NM,
		"aberroncho", NM,
		"merovingio", NM,
		"trudi", NM
	};
	char *adjectives[] = {
		"atropelladX",
		"bacaladX",
		"acojonadX",
		"estrictX",
		"diarreas",
		"gordacX"
	}, buf[256], *p;
	int i;

	i = ntruerand(nelem(adjectives));
	snprint(buf, sizeof buf, "%s", adjectives[i]);
	i = ntruerand(nelem(names));
	if((p = strchr(buf, 'X')) != nil)
		*p = names[i].sex == NF ? 'a' : 'o';
	snprint(d, len, "%s%s", names[i].name, buf);
}

void
main()
{
	char buf[256];

	genrandname(buf, sizeof buf);
	print("%s\n", buf);
	exits(0);
}
