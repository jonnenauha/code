#
#	Makefile
#

PRONAME 	= bozon
CC		= g++
OBJFILES	= main.o util.o
CFLAGS		= -g -c
LFLAGS		=


all:	program clean

program: $(OBJFILES)
	$(CC)	$(LFLAGS) $(OBJFILES) -o $(PRONAME)

main.o: main.cc
	$(CC)	$(CFLAGS) main.cc

util.o: util.cc util.hh
	$(CC)	$(CFLAGS) util.cc

clean:	
	rm -f core ./*.o

clear:
	clear

clobber: clean clear
	rm -f $(PRONAME) ./*.bck
