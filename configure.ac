#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.61])
AC_INIT([serialproxy], [git_full_version], [otto.linnemann@valeo.com])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/serialproxy.c])
AC_CONFIG_HEADERS([config.h])

# makro for fetching full git tag number version() function
AX_GIT_REVISION

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O

# Checks for libraries.

# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_MEMCMP
AC_FUNC_SELECT_ARGTYPES
AC_TYPE_SIGNAL
AC_FUNC_STAT
AC_PROG_LIBTOOL
AC_CHECK_FUNCS([inet_ntoa memset socket])

AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([rilc], [client_init])

PKG_CHECK_MODULES(libolcutils, libolcutils >= 1.0.0 )
PKG_CHECK_MODULES(libintercom, libintercom >= 0.1.1 )

AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN";
   then AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
fi

AM_CONDITIONAL([HAVE_DOXYGEN], [test -n "$DOXYGEN"])
AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([doc/doxygen/doxygen.dox doc/doxygen/doxyfile doc/doxygen/Makefile])])


AC_ARG_ENABLE([codec],
[--enable-codec	route ecall via Linux proxy ports (USB, ethernet) by default],
[codec=${enableval}], [codec=yes])

if test "x${codec}" = xyes; then
   AC_MSG_NOTICE([audio codec is used])
else
   AC_DEFINE([MC_NO_CODEC], 1, [no audio codec used])
   AC_MSG_NOTICE([audio codec is NOT used])
fi


AC_CONFIG_FILES([Makefile src/Makefile])

AC_OUTPUT
