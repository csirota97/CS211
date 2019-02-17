#include "first.h"

int main (int argc, char **argv) {
	if (argc != 2) {
		printf("error\n");
		return 0;
	}
	int check = atoi(argv[1]);
	if (check % 2 == 0) {
		printf("even\n");
	} else {
		printf("odd\n");
	}
	
	return 0;
}
