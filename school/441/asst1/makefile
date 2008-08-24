#
#	Makefile
#

PRONAME 	= ethersim
CC		= g++
OBJFILES	= main.o physical.o datalink.o packet.o
CFLAGS		= -g -c
LFLAGS		=


all:	program clean

program: $(OBJFILES)
	$(CC)	$(LFLAGS) $(OBJFILES) -o $(PRONAME)

main.o: main.cc
	$(CC)	$(CFLAGS) main.cc

physical.o: physical.cc
	$(CC)	$(CFLAGS) physical.cc

datalink.o: datalink.cc
	$(CC)	$(CFLAGS) datalink.cc

packet.o: packet.cc
	$(CC)	$(CFLAGS) packet.cc

clean:	
	rm -f core ./*.o

clear:
	clear
	
clobber: clean clear
	rm -f $(PRONAME) ./*.bck
