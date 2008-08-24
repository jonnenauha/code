#
#	Make baby!
#

CC = CC
OBJS = main.o random.o
NAME = gatest

all:	$(OBJS)
	$(CC) $(OBJS) -o $(NAME)

main.o:	main.cc wheel.h genome.h random.h
	$(CC) -c main.cc

random.o: random.cc random.h
	$(CC) -c random.cc

clean:
	rm -rf ./*.o core SunWS_cache ir.out $(NAME)
