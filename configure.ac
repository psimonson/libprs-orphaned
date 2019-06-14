AC_PREREQ([2.61])
AC_INIT([prs], [0.1], [psimonson1988@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR
AM_PROG_AR
AC_PROG_CC
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  prs.pc:prs.pc.in
  Makefile
  src/Makefile
  test/Makefile
  test/sockhelp/Makefile
])
LT_INIT
AC_OUTPUT
