#
# Template automake file
# http://www.gnu.org/software/automake/manual/index.html
# 
#		Ryan McDougall	
#
# Only one Makefile.am per directory
# must be declared in AC_CONFIG_FILES

# where_PROGRAMS
# where_LIBRARIES
# where_LTLIBRARIES (Libtool libraries)
# where_HEADERS
# where_SCRIPTS
# where_DATA
#
# $(where)            Default value
# preﬁx               /usr/ local
#       exec-preﬁx    preﬁx
#              bindir exec-preﬁx/bin
#              libdir exec-preﬁx/lib
#              ...
#       includedir    preﬁx/include
#       datadir       preﬁx/share
#       mandir        preﬁx/man
#       infodir       preﬁx/info
#
# also: noinst_, check_, dist_, notdist_

# make dist and make distcheck creates a tarball with all 
# files in *_SOURCES and *_INCLUDES
# files in dist_*_SCRIPTS and dist_*_DATA
# anything in EXTRA_DIST

# Order of recursive builds
SUBDIRS = lib src .

# foo_CFLAGS 	# Additional C compiler ﬂags
# foo_CPPFLAGS	# Additional preprocessor ﬂags (-Is and -Ds)
# foo_LDADD 	# Additional link objects, -ls and -Ls (if foo is a program)
# foo_LIBADD 	# Additional link objects, -ls and -Ls (if foo is a library)
# foo_LDFLAGS 	# Additional linker ﬂags
# The default value for foo XXXFLAGS is $(AM_XXXFLAGS).

execname_LDADD = ../lib/libbar.a
execname_CPPFLAGS = -I$(srcdir)/../lib

bin_PROGRAMS = execname
execname_SOURCES = main.c main.h

# Set in configure.ac with 
# AC_CHECK_HEADER([baz.h], [uselibfoo=yes])
# AM_CONDITIONAL([BUILDLIBFOO], [test "$uselibfoo" = yes])
if BUILDLIBFOO
    lib_LIBRARIES = libfoo.a
    libfoo_a_SOURCES = foo.c privatefoo.h
    include_HEADERS = foo.h
endif

