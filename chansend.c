#include <u.h>
#include <libc.h>
#include <thread.h>

void
recvthread(void *arg)
{
	Channel *c = arg;
	int n;

	while(recv(c, &n) > 0)
		fprint(2, "got %x\n", n);
}

void
threadmain()
{
	Channel *c;
	Alt a[2];
	int n;

	c = chancreate(sizeof(int), 1);
	n = 0xB00B5;

	a[0].op = CHANSND;
	a[0].c = c;
	a[0].v = &n;
	a[1].op = CHANEND;

	threadcreate(recvthread, c, mainstacksize);

	for(;;)
		switch(alt(a)){
		case -1:
			return;
		case 0:
			fprint(2, "sent %x\n", n);
			break;
		}
}
