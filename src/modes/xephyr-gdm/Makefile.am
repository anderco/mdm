modesdir = $(datarootdir)/@PACKAGE@/modes
modes_DATA = xephyr-gdm

gdmconfdir = $(sysconfdir)/gdm
gdmconf_DATA = gdm.conf-custom gdm.conf

gdm.conf: gdm.conf-custom
	cp gdm.conf-custom gdm.conf

EXTRA_DIST = $(modes_DATA) gdm.conf-custom
