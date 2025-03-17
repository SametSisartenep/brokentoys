#include <u.h>
#include <libc.h>
#include <bio.h>

enum {
	TSTR,
	TNUM,
	TNL,
};

typedef struct Token Token;
typedef struct Tokenizer Tokenizer;

struct Token
{
	int t;
	char *s;
	double v;
};

struct Tokenizer
{
	Biobuf *in;
	Token tok;
	char *line;
	char *f[10];
	int nf;
	int cur;
};

static char *
getline(Biobuf *b)
{
	char *l;

	if((l = Brdstr(b, '\n', 1)) == nil)
		return nil;
	return l;
}

#define isnum(c) ((c)>='0'&&(c)<='9')

static int
lex(Tokenizer *t)
{
	int new;

	new = t->line == nil;

	if(t->cur < t->nf){
		if(isnum(t->f[t->cur][0])){
			t->tok.t = TNUM;
			t->tok.v = strtod(t->f[t->cur++], nil);
		}else{
			t->tok.t = TSTR;
			t->tok.s = t->f[t->cur++];
		}
	}else{
		free(t->line);
		if(!new){
			t->line = nil;
			t->tok.t = TNL;
			return TNL;
		}
		if((t->line = getline(t->in)) == nil){
			werrstr("could not read a line");
			return -1;
		}
		t->nf = tokenize(t->line, t->f, nelem(t->f));
		t->cur = 0;
		t->tok.t = lex(t);
	}
	return t->tok.t;
}

void
main(int, char**)
{
	Biobuf *bin;
	Tokenizer t;

	memset(&t, 0, sizeof t);

	if((bin = Bfdopen(0, OREAD)) == nil)
		sysfatal("Bfdopen: %r");
	t.in = bin;
	while(lex(&t) >= 0){
		switch(t.tok.t){
		case TSTR:
			print("str: %s\n", t.tok.s);
			break;
		case TNUM:
			print("num: %g\n", t.tok.v);
			break;
		case TNL:
			print("nl\n");
			break;
		default: sysfatal("isotope escaped");
		}
	}
	free(t.line);
	exits(nil);
}
