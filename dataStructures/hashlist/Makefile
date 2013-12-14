CC := gcc

wordTransition.o:   hashlist.o wordTransition.*
	    $(CC) -c wordTransition.c -o wordTransition.o

hashlist.o:   hashList.c hashList.h
	    $(CC) -c hashList.c -o hashlist.o

clean:	  
	    rm -f *.o
