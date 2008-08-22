#
# Template autoconf file
# http://www.gnu.org/software/autoconf/manual/index.html
# run autoscan to get a skeleton configure.ac
# 
#		Ryan McDougall	
#
# AC_DEFINE([DEFINE], [VALUE], [DESCRIPTION])	# Set #define in config.h
# AC_SUBST([MACRO], [VALUE]) 			# Set $(MACRO) in Makefile

# information on the package
AC_INIT([package],[version],[ryan@ddd.co.jp])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# checks for programs
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB

#AC_CHECK_PROGS([TAR], [tar gtar], [N])
#if test "$TAR" = N; then
#  AC_MSG_WARN([This is a warning.])
#  AC_MSG_ERROR([This package needs tar.])
#fi

# checks for libraries
# AC_CHECK_LIB([LIBRARY], [FUNCINLIB], [ACTIFFOUND], [ACTIFNOT])
# If ACTIFFOUND is not set and the library is found, AC_CHECK_LIB
# will do: LIBS="-lLIBRARY $LIBS" and #define HAVE_LIBLIBRARY

# checks for header ﬁles
# AC_CHECK_HEADERS([unistd.h])
# Check for HEADERS and #define HAVE_HEADER_H for each header
# found (ex: #define HAVE_UNISTD_H).

# checks for types
# checks for structures
# checks for compiler characteristics
# checks for library functions
# checks for system services

# Output

# expand AC_DEFINE headers
AC_CONFIG_HEADERS([config.h])

# expand AC_SUBST macros
# can be any template file with placeholders of the form @MACRO@
# automake converts @MACRO@ -> $(MACRO)
AC_CONFIG_FILES([Makefile src/Makefile]) 

AC_OUTPUT
