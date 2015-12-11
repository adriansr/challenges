#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([badoolization], [1.0], [adrisr83@gmail.com])
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
AC_CONFIG_SRCDIR([src/main.cc])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR(m4)

# Checks for programs.
# prioritize clang, the one I used
AC_LANG(C++)
AC_PROG_CXX(clang++ g++)
AC_PROG_CC(clang gcc)

AX_CXX_COMPILE_STDCXX_11([noext],[optional])
AM_CONDITIONAL([CXX11],[test x"$HAVE_CXX11" = "x1"])
AC_PROG_CXX_C_O

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([memmove])

AC_CONFIG_FILES([Makefile
				 src/Makefile])

AC_LANG_ASSERT([C++])
AC_OUTPUT
