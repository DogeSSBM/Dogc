#include "Includes.h"
//#define DEBUG_PARSER

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
		char *strinc;
		LiscFunc func;
		struct Lisc* lisc;
	};
	struct Lisc* nexc;
}Lisc;
void freeTrec(Lisc *thingec);

Lisc *add(Lisc *thingec)
{
	int accumulator = 0;
	Lisc *retc = calloc(1, sizeof(Lisc));

	while(thingec){
		if(thingec->typc == lisc_t){
			if(thingec->lisc->typc != func_t){
				fprintf(stderr, "Non function in nested list\n");
				return retc;
			}
			Lisc *ret = thingec->lisc->func(thingec->lisc->nexc);
			accumulator += ret->inc;
			freeTrec(ret);
			thingec = thingec->nexc;
		}else if(thingec->typc == inc_t){
			accumulator += thingec->inc;
			thingec = thingec->nexc;
		}else if(thingec->typc == err_t){
			return retc;
		}else{
			fprintf(stderr, "Non numeric atoms in input\n");
			return retc;
		}
	}
	retc->typc = inc_t;
	retc->inc = accumulator;
	return retc;
}

Lisc *mul(Lisc *thingec)
{
	int accumulator = 1;
	Lisc *retc = calloc(1, sizeof(Lisc));

	while(thingec){
		if(thingec->typc == lisc_t){
			if(thingec->lisc->typc != func_t){
				fprintf(stderr, "Invalid function in nested list\n");
				return retc;
			}
			Lisc *ret = thingec->lisc->func(thingec->lisc->nexc);
			accumulator *= ret->inc;
			freeTrec(ret);
			thingec = thingec->nexc;
		}else if(thingec->typc == inc_t){
			accumulator *= thingec->inc;
			thingec = thingec->nexc;
		}else if(thingec->typc == err_t){
			return retc;
		}else{
			fprintf(stderr, "Non numeric atoms in input\n");
			return retc;
		}
	}
	retc->typc = inc_t;
	retc->inc = accumulator;
	return retc;
}

void tabOut(int tabbing)
{
	for(int i = 0; i < tabbing; i++) putchar('\t');
}

void printTrec(Lisc *thingec, int tabbing)
{
	tabOut(tabbing);
	switch(thingec->typc){
		case err_t:
			printf("Data = ERR\n");
			break;
		case inc_t:
			printf("Data = INT: %d\n", thingec->inc);
			break;
		case strinc_t:
			printf("Data = STR: %s\n", thingec->strinc);
			break;
		case func_t:
			if(thingec->func == add) printf("Data = FUNC: ADD\n");
			else if(thingec->func == mul)  printf("Data = FUNC: MUL\n");
			else printf("Data = FUNC: %p\n", thingec->func);
			break;
		case lisc_t:
			printf("Data = LIST: %p\n", thingec->lisc);
			if(thingec->lisc) printTrec(thingec->lisc, tabbing + 1);
			break;
	}
	tabOut(tabbing);
	printf("Next = %p\n", thingec->nexc);
	if(thingec->nexc) printTrec(thingec->nexc, tabbing);
}

void freeTrec(Lisc *thingec)
{
	if((thingec->typc == lisc_t) && (thingec->lisc)) freeTrec(thingec->lisc);
	if(thingec->nexc) freeTrec(thingec->nexc);
	free(thingec);
}

int checkParenthesis(const char *input)
{
	int i = 0;
	int level = 0;
	while(input[i]){
		if(input[i] == '(') level++;
		else if(input[i] == ')') level--;
		if(level < 0) return -1;
		i++;
	}
	return level;
}

int matchParenthesis(const char *input)
{
	int level = 1;
	int i = 0;
	while(!((input[i] == ')') && (!level))){
		i++;
		if(input[i] == '(') level++;
		else if(input[i] == ')') level--;
		if(level < 0) return -1;
	}
	return i;
}

Lisc* parsc(const char* inpuc)
{
	if(!inpuc) return NULL;
	Lisc *root = calloc(1, sizeof(Lisc));
	int i = 0;
	while((inpuc[i] != '\0') && (inpuc[i] == ' ')) i++;

	if(inpuc[i] == '+'){
		root->typc = func_t;
		root->func = add;
	}else if(inpuc[i] == '*'){
		root->typc = func_t;
		root->func = mul;
	}else{
		fprintf(stderr, "Invalid operator: %c\n", inpuc[i]);
		free(root);
		return NULL;
	}

	Lisc *cur = root;
	bool loop = true;
	for(i++; loop; i++){
		while((inpuc[i] != '\0') && (inpuc[i] == ' ')) i++;
#ifdef DEBUG_PARSER
		printf("inpuc[%d] = %c\n", i, inpuc[i]);
#endif /*DEBUG_PARSER*/
		switch(inpuc[i]){
			case '\0':	/*Fallthrough*/
			case ')':
				loop = false;
				break;
			case '0':	/*Fallthrough*/
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				int val = atoi(inpuc + i);
				while(isdigit(inpuc[i])) i++;
				i--;
				cur->nexc = calloc(1, sizeof(Lisc));
				cur->nexc->typc = inc_t;
				cur->nexc->inc = val;
				cur = cur->nexc;
				break;
			case '(':
				cur->nexc = calloc(1, sizeof(Lisc));
				cur->nexc->typc = lisc_t;
				cur->nexc->lisc = parsc(inpuc + i + 1);
				if(!cur->nexc->lisc){
					freeTrec(root);
					return NULL;
				}
				i += matchParenthesis(inpuc + i);
				cur = cur->nexc;
				break;
			default:
				freeTrec(root);
				fprintf(stderr, "Invalid char %c at %d\n", inpuc[i], i);
				return NULL;
		}
	}

	return root;
}

/* Input tests:
 * "+" => 0
 * "*" => 1
 * "+ 1 2 3 4" => 10
 * "* 1 2 3 4" => 24
 * "+ (+ 1 (+ 1 (+ 1) 1))" => 4
 * "+ 1 (* 24 12 2) 21 3 (+ 21 (+ 23 3 (* 2 3) 2))" => 656
 */
int main(int argc, char const *argv[])
{
	if(argc != 2){
		fprintf(stderr, "Usage: Dogc <expr>\n");
		exit(1);
	}
	/*
	Length window = {800, 600};
	init();
	setWindowLen(window);
	*/

	if(checkParenthesis(argv[1]) != 0){
		fprintf(stderr, "Check parenthesis\n");
		return 1;
	}

	Lisc *ast = parsc(argv[1]);

	if(ast){ 
		printTrec(ast, 0);
		printf("\nRun the tree!\n");
		Lisc *ret = ast->func(ast->nexc);
		printf("%s => %d\n", argv[1], ret->inc);
		printf("\nPost run:\n");
		printTrec(ast, 0);
		freeTrec(ast);
	}

	/*
	while(1){
		Frame frame = frameStart();



		frameEnd(frame);
	}
	*/
	return 0;
}
