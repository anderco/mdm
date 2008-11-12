AC_INIT([mdm], 0.0.3, [http://wiki.c3sl.ufpr.br/multiseat], mdm)
AM_INIT_AUTOMAKE([dist-bzip2])

AC_PROG_CC
AC_PROG_INSTALL

PKG_CHECK_MODULES(MDM, cairo x11)
AC_SUBST(MDM_CFLAGS)
AC_SUBST(MDM_LIBS)

AC_OUTPUT([Makefile src/Makefile src/modes/Makefile src/config/Makefile])
