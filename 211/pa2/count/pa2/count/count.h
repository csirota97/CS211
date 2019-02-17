#include <stdio.h>
#include <stdlib.h>

typedef struct Head {
	struct Node *next;
} Head;

typedef struct Node {
	unsigned long int value;
	struct Node *next;
} Node;

int hash(unsigned long int);

Node* newNode(Node*, unsigned long int);

void print(Head);
