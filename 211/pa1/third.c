#include "third.h"


int main (int argc, char **argv) {
	int size = 0;
	Node* first = (Node*)malloc(sizeof(Node));
	if (argc != 2) { //Check if argv[1] is real
		printf("error\n");
		return 0;
	}
	
	FILE* fp;
	fp = fopen(argv[1], "r");

	if (fp == NULL) {
		return 0;
	}
	while (!feof(fp)) {

		char operation = NULL;	//i or d
		int val = NULL;		//number being entered or removed from list
		int ret = 0;
		ret = fscanf(fp,"%c\t%d\n", &operation, &val);	//reads file and sets vars

		if (ret != 2) {
			//incorrect format
			printf("error\n");
			return 0;
		}

		if (operation == 'i') { //if new data is being added into list

			if (size == 0) {
				(*first).value = val;
				size++;
			} else {
				int i = 0;
				Node* ptr = (Node*)malloc(sizeof(Node));
				ptr = first;

				
				for (; i < size; i++) {
					
					if (val < (*first).value) {
						first = newNode(ptr, val);
						size++;
						break;
					}
					if ((*ptr).value == val) {
						break;
					} else if (val < (*ptr).value){	//THE PROBLEM IS HERE
					//---------------------------------
						Node* ptra = (Node*)malloc(sizeof(Node));
						ptra = first;
						while ((*ptra).next != ptr) {
							ptra = (*ptra).next;
						}
						(*ptra) .next = newNode(ptr, val);
						size++;

						ptra = newNode(ptr, val);
						break;
					} else if ((*ptr).next == NULL) {
						size++;
						(*ptr).next = newNode(NULL, val);
						ptr = (*ptr).next;
						(*ptr).value = val;
						break;
					}
					ptr = (*ptr).next;
				}
			}
		} else if (operation == 'd') { //if data is being removed from the list
			if (size != 0) {

				Node* tempNode = (Node*)malloc(sizeof(Node));
				tempNode = delete(first, val, size);

				if (tempNode != NULL) {

					first = newNode((*tempNode).next, (*tempNode).value);
					size--;
				}
			}
		}
	}
	print(first, size);
	return 0;
}

Node* newNode(Node* next, int val) {
	Node* nn = (Node*)malloc(sizeof(Node));
	if(nn == NULL) {
		printf("Couldn't create node.\n");
		exit(0);
	}
	(*nn).value = val;
	(*nn).next = next;
 
	return nn;
}

Node* delete(Node* first ,int val, int size) {
	if ((*first).value == val) {
		Node* ptr = (Node*)malloc(sizeof(Node));
		ptr = first;
		ptr = (*ptr).next;
		first = (*first).next;
		return first;

	} else {
		Node* ptr = first;
		int i = 0;
		for (; i < size; i++){
			if ((*ptr).value == val) {
				Node* ptra = first;
				while((*ptra).next != ptr) {
					ptra = (*ptra).next;
				}
				(*ptra).next = (*ptr).next;
				return first;
			}

			ptr = (*ptr).next;
		}
	}
	return NULL;
}

void print(Node* node, int size) {
	Node* ptr = node;
	int i = 0;
printf("size: %d\n", size);
	for (; i < size; i++){
		printf("%d\n", (*ptr).value);
		ptr = (*ptr).next;
	}	
}






