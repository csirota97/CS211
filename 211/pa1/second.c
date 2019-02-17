#include "second.h"

int main (int argc, char **argv) {

	if (argc != 2) {
		printf("error\n");
		return 0;
	}

	int isPrime = atoi(argv[1]);
	
	int i = 2;
	for (; i < isPrime; i++) {
		if (isPrime%i == 0) {
			printf("no\n");
			return 0;
		}
	}

	printf("yes\n");
	return 0;
}
