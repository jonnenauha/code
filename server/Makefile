#
# Template Makefile
# http://www.gnu.org/software/make/manual
#
#		Ryan McDougall


EXENAME = happyfun	# Name of the output executable
LIBNAME =		# Name of the output library
OBJS = main.o		# Object files in the project executable
LIBOBJS = 		# Object files in the project library
LIBS =			# Libraries in the project
RM = rm -rf		# Command to remove a file
CC = gcc		# Program for compiling C programs
CXX = g++		# Program for compiling C++ programs
AR = ar			# Program for creating library archives
CFLAGS = -g -Wall -DDEBUG	# Extra flags to give to the C compiler
CPPFLAGS = -I. 		# Extra flags to give to the C preprocessor
CXXFLAGS = $(CFLAGS)	# Extra flags to give to the C++ compiler
LDFLAGS =		# Extra flags to give to compilers to invoke the linker
LDLIBS = -lpthread	# Libraries for the linker to load
ARFLAGS = 		# Arguments to library archiver
SUBDIRS =		# Subdirectories that contain recursive makefiles

# Targets that don't produce a file
.PHONY: all clean distclean $(SUBDIRS)	

# Pattern Rules for building files
#%.o : %.c
#	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
#
#%.o: %.cpp
#	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

all: $(EXENAME) $(LIBNAME)

$(EXENAME): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(EXENAME)

$(LIBNAME): $(LIBOBJS)
	$(AR) rcs lib$(LIBNAME).a $(LIBOBJS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	$(RM) $(OBJS) core

distclean: clean
	$(RM) $(EXENAME) $(LIBS) 
