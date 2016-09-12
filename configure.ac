AC_PREREQ([2.69])
AC_INIT([http-server], [0.01], [tharindubathigama@gmail.com])
#AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
#AC_CHECK_LIB(args_parser, args_parser_create,, AC_MSG_ERROR([args_parser library required, check README file for more info.]))
#PKG_CHECK_MODULES([OPENSSL], [openssl],, AC_MSG_ERROR([openssl is required.]))

PKG_CHECK_MODULES([CURL], [libcurl],, AC_MSG_ERROR([libcurl is required.]))
PKG_CHECK_MODULES([OPENSSL], [openssl],, AC_MSG_ERROR([openssl is required.]))

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
				
# Checks for library functions.
AC_CONFIG_FILES([Makefile\
		 src/Makefile\
		 test/Makefile])

AM_INIT_AUTOMAKE([1.11 subdir-objects -Wall -Werror])
AM_SILENT_RULES([yes])

AC_OUTPUT
