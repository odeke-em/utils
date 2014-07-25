CC := gcc
LIBS := -pthread

%.o:	%.c
	$(CC) $< -c -o $@ $(LIBS)

%:	%.c %.o
	$(CC) $< -o $@

Test:	DMap.o HashMap.o DNode.o RTrie.o
HashMap:	HashMap.c RTrie.o
clean:	
	rm -f *.o
