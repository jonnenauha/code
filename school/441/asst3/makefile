#
#	Makefile
#

PRONAME 	= netsim
CC		= g++
OBJFILES	= main.o
CFLAGS		= -g -c 
LFLAGS		= 


all:	program clean

program: $(OBJFILES)
	$(CC)	$(LFLAGS) $(OBJFILES) -o $(PRONAME)

main.o: main.cc
	$(CC)	$(CFLAGS) main.cc

clean:	
	rm -f core ./*.o

clear:
	clear

clobber: clean clear
	rm -f $(PRONAME) ./*.bck
