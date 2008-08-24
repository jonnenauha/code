#
# Makefile for gobject-tutorial-example1
#

NAME	= example1
CC	= gcc
CFLAGS	= `pkg-config --cflags gobject-2.0`
LIBS	= `pkg-config --libs gobject-2.0`

all: $(NAME)

$(NAME): gobject-tutorial-example1.c gobject-tutorial-example1.h
	$(CC)	$(CFLAGS) $(LIBS) gobject-tutorial-example1.c -o $(NAME)

clean:
	rm -f $(NAME)
