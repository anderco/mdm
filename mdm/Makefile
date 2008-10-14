# FIXME: this Makefile should not make the tree even if there is no need to do
# it

CC = gcc
CFLAGS = -O2 -Wall
VERSION = 0.0.3

all: tree

install: tree
	@echo "Installing..."
	@misc/install.sh

tree: binaries prefix
	@echo "Translating package"
	@misc/translate.sh
	@echo "Creating file tree in temporary folder tree/"
	@misc/make-tree.sh

prefix:
	@if test ! -z "$(DESTDIR)"; then			\
	    	echo "Changing prefix to $(DESTDIR)";		\
		misc/change-prefix.sh $(DESTDIR);		\
	fi

binaries: src/read-devices.c src/write-message.c src/seat-parent-window.c
	$(CC) $(CFLAGS) src/read-devices.c  -o src/read-devices
	$(CC) $(CFLAGS) src/write-message.c -o src/write-message `pkg-config --libs --cflags cairo x11`
	$(CC) $(CFLAGS) src/seat-parent-window.c -o src/seat-parent-window `pkg-config --libs --cflags x11`

clean:
	rm -f src/read-devices
	rm -f src/write-message
	rm -f src/seat-parent-window
	rm -f po/*.mo
	rm -rf *tree
	rm -rf packages
