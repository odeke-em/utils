CC := gcc
FLAGS := -Wall -Wextra
LIBS := -pthread

%.o:	%.c
	$(CC) $(FLAGS) $< -c -o $@ $(LIBS)

%:	%.c %.o
	$(CC) $(FLAGS) $< -o $@

useCase:	DMap.o HashMap.o DNode.o RTrie.o MBuf.o
Test:	DMap.o HashMap.o DNode.o RTrie.o
HashMap:	HashMap.c RTrie.o
clean:	
	rm -f *.o
receipe:    useCase
	[ -f genRandInts.py ] && ./genRandInts.py -c 2000000 -o rIntegers.txt
	[ -f rIntegers.txt ] && cat rIntegers.txt | ./useCase
