#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	struct Node *next;
	int value;
} Node;

Node* newNode(Node*, int);

Node* delete(Node*, int, int);

void print(Node*, int);
