CC := gcc

pqueue:	pQueue.c pQueue.h heap.o parcel.o Makefile
	$(CC) parcel.o pQueue.c heap.o -o pqueue

parcel.o:	parcel.c parcel.h
	$(CC) -c parcel.c -o parcel.o

heap.o:	heap.c heap.h
	$(CC) -c heap.c -o heap.o

clean:	
	rm -f heap *.o
