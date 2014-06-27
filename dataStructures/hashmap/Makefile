CC := gcc
LIBS := -pthread

%.o:	%.c
	$(CC) $< -c -o $@ $(LIBS)

%:	%.c %.o
	$(CC) $< -o $@

HashMap:	HashMap.c RTrie.o
clean:	
	rm -f *.o
