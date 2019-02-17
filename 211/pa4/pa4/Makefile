CC= gcc
CFLAGS= -Wall -Werror -O -o

all : first

first : first.c first.h
	$(CC) $(CFLAGS) first.c -o first
	
matrix : matrix.c matrix.h
	$(CC) $(CFLAGS) matrix.c -o matrix

clean : 
	rm -f first
