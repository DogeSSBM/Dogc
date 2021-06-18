#include "Includes.h"

typedef enum{
	err_t=0,
	inc_t,
	strinc_t,
	func_t,
	lisc_t
}Typc;

typedef struct Lisc* (*LiscFunc)(struct Lisc *);

typedef struct Lisc{
	Typc typc;
	union{
		int inc;
		char strinc;
		LiscFunc func;
		struct Lisc* lisc;
	};
	struct Lisc* nexc;
}Lisc;

Lisc* parsc(Lisc *trec, const char* inpuc)
{

	return trec;
}


Lisc *add(Lisc *thingec)
{
	return calloc(1, sizeof(Lisc));

}

LiscFunc func = add;

Lisc *mul(Lisc *thingec)
{
	return calloc(1, sizeof(Lisc));
}

/*

+ 2 3

+ 2 (* 2 a)
+ 1 2 3 4 5 (* 1 2)

- 3 5 3

* 2 (+ 2 3)

*/

int main(int argc, char const *argv[])
{
	Length window = {800, 600};
	init();
	setWindowLen(window);

	while(1){
		Frame frame = frameStart();



		frameEnd(frame);
	}
	return 0;
}
