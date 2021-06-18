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

/*
+ 2 4 5 (+ 3 5 1)

`(1 2 3 ,@(a b c) 4 5) => (1 2 3 a b c 4 5)
`(1 2 gensym 4) => (1 2 gensym 4)
(define gensym 3)
`(1 2 ,gensym 4) => (1 2 3 4)
*/


Lisc *add(Lisc *thingec)
{
	int accumulator = 0;
	Lisc *retc = calloc(1, sizeof(Lisc));
	if(!thingec) return retc;

	do{
		if(thingec->typc == lisc_t){
			if(thingec->lisc->typc != func_t){
				fprintf(stderr, "Invalid function in nested list\n");
				return retc;
			}
			Lisc *ret = thingec->lisc->func(thingec->lisc->nexc);
			ret.nexc = thingec->nexc;
			thingec->nexc = ret;
		}else if(thingec->typc == inc_t){
			accumulator += thingec->inc;
			thingec = thingec->nexc;
		}else if(thingec->typc == err_t){
			return retc;
		}else{
			fprintf(stderr, "Non numeric atoms in input\n");
			return retc;
		}
	}while(thingec->next != NULL);

	retc->typc = inc_t;
	retc->inc = accumulator;
	return retc;
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
