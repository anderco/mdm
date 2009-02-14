AC_INIT([mdm], 0.0.3, [http://wiki.c3sl.ufpr.br/multiseat], mdm)
AM_INIT_AUTOMAKE([dist-bzip2])

AC_PROG_CC
AC_PROG_INSTALL

AM_GNU_GETTEXT([external])

AC_ARG_ENABLE(xephyr-gdm,
      	      AS_HELP_STRING([--enable-xephyr-gdm],
			     [Enables xephyr-gdm mode (default: enable)]),
	      [XEPHYR_GDM=$enableval],
	      [XEPHYR_GDM=yes])
AM_CONDITIONAL(XEPHYR_GDM, [test "x$XEPHYR_GDM" = xyes])

PKG_CHECK_MODULES(MDM, cairo x11)
AC_SUBST(MDM_CFLAGS)
AC_SUBST(MDM_LIBS)

AC_OUTPUT([
Makefile
src/Makefile
src/modes/Makefile
src/modes/default/Makefile
src/modes/xephyr-gdm/Makefile
src/config/Makefile
po/Makefile.in
])
