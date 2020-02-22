#include <u.h>
#include <libc.h>

void
main()
{
	char buf[256], *f[10];
	int nf, i;

	while(read(0, buf, sizeof(buf)-1) > 0){
		nf = tokenize(buf, f, nelem(f));
		for(i = 0; i < nf; i++)
			fprint(2, "%d: %s\n", i, f[i]);
	}
	exits(0);
}
