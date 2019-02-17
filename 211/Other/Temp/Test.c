#include <stdio.h>

int main (void) {
const int N = 6;
int i, j;

for (i=1; i<=N+1; i++) {

for (j=1; j<=N;j++) {

printf("%c", 'A' +((i+j-2) % N));
}
printf("\n");

}

}
