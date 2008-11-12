SUBDIRS = modes config

AM_CFLAGS = $(MDM_CFLAGS)

read_devices_LDADD = $(MDM_LIBS)
seat_parent_window_LDADD = $(MDM_LIBS)
write_message_LDADD = $(MDM_LIBS)

sbin_PROGRAMS = read-devices seat-parent-window write-message

read_devices_SOURCES = read-devices.c
seat_parent_window_SOURCES = seat-parent-window.c
write_message_SOURCES = write-message.c

sbin_SCRIPTS = mdm-bin mdm-common mdm-start-monoseat mdm-start-seat \
	       create-xorg-conf discover-devices xephyr-wrapper \
	       xrandr-functions

initscriptsdir = $(sysconfdir)/init.d
initscripts_DATA = mdm

EXTRA_DIST = $(initscripts_DATA)
