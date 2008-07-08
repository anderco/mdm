# FIXME: this Makefile should not make the tree even if there is no need to do
# it

CC = gcc
CFLAGS = -O2 -Wall
VERSION = 0.0.1

all: tree

install: tree
	@echo "Installing..."
	@misc/install.sh

ubuntu-8.04-install: ubuntu-8.04
	@echo "Installing the mdm package"
	@dpkg -i packages/mdm_$(VERSION)_i386.deb

ubuntu-8.04: tree
	@echo "Creating .deb"
	@cp -r tree ubuntu-8.04-tree
	@cp -r distro/ubuntu-8.04/DEBIAN ubuntu-8.04-tree
	@mkdir -p packages
	@fakeroot dpkg -b ubuntu-8.04-tree packages

tree: binaries prefix
	@echo "Creating file tree in temporary folder tree/"
	@misc/make-tree.sh

prefix:
	@if test ! -z "$(DESTDIR)"; then			\
	    	echo "Changing prefix to $(DESTDIR)";		\
		misc/change-prefix.sh $(DESTDIR);		\
	fi

binaries: bin/read-devices.c bin/write-message.c
	$(CC) $(CFLAGS) src/read-devices.c -o src/read-devices
	$(CC) $(CFLAGS) src/write-message.c -o src/write-message `pkg-config --libs --cflags cairo x11`

clean:
	rm -f src/read-devices
	rm -f src/write-message
	rm -rf *tree
	rm -rf packages
