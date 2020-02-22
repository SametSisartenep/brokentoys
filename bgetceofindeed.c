#include <u.h>
#include <libc.h>
#include <bio.h>

void
main()
{
	Biobuf *bin;
	char c;

	bin = Bfdopen(0, OREAD);
	if(bin == nil)
		sysfatal("Bfdopen: %r");
	while((c = Bgetc(bin)) != Beof)
		;
	USED(c);
	c = Bgetc(bin);
	if(c == Beof)
		print("eof indeed\n");
	else
		print("no eof after eof\n");
	exits(0);
}
