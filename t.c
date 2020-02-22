#include <u.h>
#include <libc.h>
#include <draw.h>
#include <thread.h>

Channel *spamchan;

void
freethread(void *)
{
	char *v = nil;

	threadsetname("freethread");
Loop:
	recv(spamchan, &v);
	if(v == nil){
		print("nothing to free\n");
		threadexitsall(0);
	}
	print("freeing %s\n", v);
	free(v);
	goto Loop;
}

void
spammer(void*)
{
	int i;
	char *s;

	threadsetname("spammer");
	for(i = 0; i < 10; ++i){
		s = smprint("%d", i);
		send(spamchan, &s);
	}
	send(spamchan, nil);
}

void
threadmain(int argc, char *argv[])
{
	print("acid -l thread %d\n", getpid());
	threadsetname("main");
	spamchan = chancreate(sizeof(char*), 0);
	threadcreate(spammer, nil, 8192);
	threadcreate(freethread, nil, 8192);
	yield();
}
