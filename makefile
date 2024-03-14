
CC = gcc
CFLAGS = -Wall -std=c11

findme: findme.c findme.h 
	$(CC) $(CFLAGS) -o findme findme.c

clean:
	rm -f findme *.o 