CC := gcc
LRU:	list.o list.h
	$(CC) list.o LRU.c -o LRU

list.o:	  list.h list.c Makefile
	$(CC) -c list.c -o list.o

clean:	  
	rm *.o  LRU
