#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Block {
    unsigned long int tStamp;
    int valid;
	unsigned long int tag;
} Block;

typedef struct Set {
    Block* array;
} Set;


void fifo();
void lru();
void nru();
int twoToThe(int);
int powerOf2(int);
int assocEquals (char*);
int find (char*, char);
int policyEquals (char*);
