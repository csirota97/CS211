#include "count.h"

int main (int argc, char **argv) {
	unsigned long int addr;
	int ret;
	int uni = 0;

	Head* HT = (Head*)malloc(1000 * sizeof(Head));			//creates array of Heads which 
									//	points to linked lists

	int i;
	for (i = 0; i < 1000; i++) {					//sets all Heads to null
		HT[i].next = NULL;
	}
	
	FILE* fp;
	fp = fopen(argv[1], "r");

	if (fp == NULL) { 						//prints "error" if the file does not exist and then exits
		printf("error\n");
		return 0;
	}
	while (!feof(fp)) {
		ret = fscanf(fp,"%lx\n", &addr);			 //reads file and sets address
		if (ret != 1) { } else {
			if (HT[hash(addr)].next == NULL) {		//if the Head is null
				HT[hash(addr)].next = newNode(NULL, addr);//creates new linked list from head
				uni++;					//increments unique


			} else { 					//if the Head has a linked list already

				Node* ptr = HT[hash(addr)].next;		//creates a pointer
				int check = 0;
				while (ptr != NULL) {			//checks if address is in linked list already
					if ((*ptr).value == addr) {
						check = 1;		//sets "check" to 1 if address already exists and exits loop
						break;
					}
					ptr = (*ptr).next;
				}
				if (check == 0) {			
					HT[hash(addr)].next = newNode(HT[hash(addr)].next,addr);//if "check" equals 0, adds address to linked list at head
					uni++;				//increments unique
				}
			}
		}
	}
	printf("%d\n", uni);						//prints total number of unique addresses
	return 0;
}

int hash(unsigned long int addr) {
	return addr%1000;						//returns the key for the address
}

Node* newNode(Node* next, unsigned long int val) {			//creates new node with address
	Node* nn = (Node*)malloc(sizeof(Node));
	(*nn).value = val;						//sets value
	(*nn).next = next;						//sets pointer to next node
 
	return nn;							//returns new node
}

