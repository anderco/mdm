# FIXME: this Makefile should not make the tree even if there is no need to do
# it

CC = gcc
CFLAGS = -O2 -Wall
VERSION = 0.0.1

all: tree

install: tree
	@echo "Installing..."
	@misc/install.sh

debian-lenny-install: debian-lenny
	@echo "Installing the mdm package"
	@dpkg -i packages/mdm_$(VERSION)_i386.deb

debian-lenny: tree
	@echo "Creating debian-lenny-tree"
	@cp -r tree debian-lenny-tree
	@cp -r distro/debian-lenny/DEBIAN debian-lenny-tree
	@mkdir -p packages
	@echo "Creating .deb in packages/"
	@fakeroot dpkg -b debian-lenny-tree packages

ubuntu-8.04-install: ubuntu-8.04
	@echo "Installing the mdm package"
	@dpkg -i packages/mdm_$(VERSION)_i386.deb

ubuntu-8.04: tree
	@echo "Creating ubuntu-8.04-tree"
	@cp -r tree ubuntu-8.04-tree
	@cp -r distro/ubuntu-8.04/DEBIAN ubuntu-8.04-tree
	@mkdir -p packages
	@echo "Creating .deb in packages/"
	@fakeroot dpkg -b ubuntu-8.04-tree packages

tree: binaries prefix
	@echo "Creating file tree in temporary folder tree/"
	@misc/make-tree.sh

prefix:
	@if test ! -z "$(DESTDIR)"; then			\
	    	echo "Changing prefix to $(DESTDIR)";		\
		misc/change-prefix.sh $(DESTDIR);		\
	fi

binaries: src/read-devices.c src/write-message.c src/xephyr-parent-window.c
	$(CC) $(CFLAGS) src/read-devices.c  -o src/read-devices
	$(CC) $(CFLAGS) src/write-message.c -o src/write-message `pkg-config --libs --cflags cairo x11`
	$(CC) $(CFLAGS) src/xephyr-parent-window.c -o src/xephyr-parent-window `pkg-config --libs --cflags x11`

clean:
	rm -f src/read-devices
	rm -f src/write-message
	rm -f src/xephyr-parent-window
	rm -rf *tree
	rm -rf packages
