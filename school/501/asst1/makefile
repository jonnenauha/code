#
#		CPSC 501 -- Asstignment 1
#
#	Makefile
#

CC	= gcc
CFLAGS	= -pg -O2 -c
LFLAGS	= -pg
OBJS	= filecopy.o
NAME	= filecopy

all:	$(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o $(NAME)

filecopy.o:	filecopy.c filecopy.h
	$(CC) $(CFLAGS) filecopy.c

clean:
	rm ./*.o $(NAME)
