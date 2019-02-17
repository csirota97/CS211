#include "fourth.h"

int main (int argc, char **argv) {
	int ret = 0; //returns number of inputs after each scan
	int num = 0; //stores number
	char operation = NULL; //checks what operation to perform

	Node* HT = (Node*)malloc(1000 * sizeof(Node));

	int i = 0;
	for (; i < 1000; i++) {
		HT[i].check = 0;
		HT[i].value = 0;
	}
	if (argc != 2) { //Check if argv[1] is real
		printf("error\n");
		return 0;
	}
	FILE* fp;
	fp = fopen(argv[1], "r");

	if (fp == NULL) { //ends program if file not found
		printf("error\n");
		return 0;
	}
	while (!feof(fp)) {
		ret = fscanf(fp,"%c\t%d\n", &operation, &num); //reads file and sets vars
		if (ret != 2) {
		} else if (operation == 'i') {
			int dup = 0;
			int hv = hash(num);
			if (hv < 0)
				hv = hv*(-1);
			int full = hv;

			while (HT[hv].check == 1) {
				if (HT[hv].value == num) {
					printf("duplicate\n");
					dup = 1;
				}
				hv++;
				if (full == hv) {
					break;
				}
			}
			if(HT[hv].check == 0 && dup == 0) {
				HT[hv].check = 1;
				HT[hv].value = num;
				printf("inserted %d-%d\n", num,hv);
			}
		} else if (operation == 's') {
			int hv = hash(num);
			if (hv < 0)
				hv = hv*(-1);
			int full = hv;
			while (HT[hv].check == 1 && HT[hv].value != num) {
				hv++;
				if (hv==1000)
					hv=0;
				if (full == hv) {
					printf("absent  %d\n", num);
					break;
				}
			}
			if (HT[hv].value == num && HT[hv].check == 1)
				printf("present  %d\n", num);
			else
				printf("absent %d\n", num);
		} else {
			printf("error\n");
		}
			
	}
	return 0;
}

int hash(int num) {
	return num%1000;
}
