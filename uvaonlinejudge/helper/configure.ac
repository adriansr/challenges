#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([helper], [1.0], [adrisr83@gmail.com])
AC_CONFIG_SRCDIR([test/helper_test.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_FILES([Makefile test/Makefile])
AM_INIT_AUTOMAKE([foreign subdir-objects tar-pax])
LT_INIT

# Checks for programs.
AC_PROG_CXX
AC_LANG_PUSH(C++)

# Checks for libraries.
BOOST_REQUIRE(1.53)
BOOST_TEST

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T

# Checks for library functions.

AC_OUTPUT
