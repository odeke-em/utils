CC := gcc
FLAGS := -Wextra -Wall

all:	utils.o testSuite.c

%.o:	%.c %.h
		$(CC) $(FLAGS) -c $< -o $@

%:	%.c %.o
	$(CC) $(FLAGS) $< -o $@

testSuite:	testSuite.c utils.o

clean:
		rm -f *.o
