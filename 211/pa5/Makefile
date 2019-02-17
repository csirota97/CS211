CC= gcc
CFLAGS= -Wall -Werror

all : comp

first : first.c first.h
	$(CC) $(CFLAGS) first.c -o first

comp : first.c
	gcc -O -o first first.c

clean : 
	rm -f first
