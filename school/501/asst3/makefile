#
#	Make baby!
#

CC 	= g++
CFLAGS	= -g -c
OBJS 	= main.o random.o
NAME 	= neuralnet

all:	$(OBJS)
	$(CC) $(OBJS) -o $(NAME)

main.o:	main.cc random.h
	$(CC) $(CFLAGS) main.cc

random.o: random.cc random.h
	$(CC) $(CFLAGS) random.cc

clean:
	rm -rf ./*.o core SunWS_cache ir.out $(NAME)
