CC := gcc

graph:	Graph.* hashlist.o list.o heap.o utils.o Makefile
	$(CC) Graph.c hashlist.o list.o heap.o utils.o -o graph

utils.o:      utils.c
	$(CC) -c $< -o $@

hashlist.o:   hashlist/*.c
	$(CC) -c $< -o $@

list.o:	  list/*.c
	$(CC) -c $< -o $@

heap.o: heap/*.c
	$(CC) -c $< -o $@

%.o:	  %.c
	  $(CC) -c %.c %.o

clean:	  graph
	rm -f *.o graph
