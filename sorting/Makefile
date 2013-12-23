CC := gcc

SLRadixSort:	node.o SLRadixSort.c
	  $(CC) Node.o SLRadixSort.c -o SLRadixSort

node.o:	  Node.c Node.h
	  $(CC) -c Node.c -o Node.o

clean:	  
	  rm -f *.o
