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
	@echo "Installing dependencies"
	@cd distro/ubuntu-8.04/patches && make install
	@echo "Installing the mdm package"
	@dpkg -i packages/mdm_$(VERSION)_i386.deb

ubuntu-8.04: tree
	@echo "Creating .deb"
	@cp -r tree ubuntu-8.04-tree
	@cp -r distro/ubuntu-8.04/package/DEBIAN ubuntu-8.04-tree
	@mkdir -p packages
	@fakeroot dpkg -b ubuntu-8.04-tree packages

targz: tree
	@echo "Creating .tar.gz file"
	tar cvzf mdm.tar.gz tmp/*

tree: binaries prefix
	@echo "Creating file tree in temporary folder tmp/"
	@misc/make-tree.sh

prefix:
	@if test ! -z "$(DESTDIR)"; then			\
	    	echo "Changing prefix to $(DESTDIR)";		\
		bin/change-prefix.sh $(DESTDIR);		\
	fi

binaries: bin/read-devices.c bin/write-message.c
	$(CC) $(CFLAGS) bin/read-devices.c -o bin/read-devices
	$(CC) $(CFLAGS) bin/write-message.c -o bin/write-message `pkg-config --libs --cflags cairo x11`

clean:
	rm -f bin/read-devices
	rm -f bin/write-message
	rm -rf *tree
	rm -rf packages
	rm -rf mdm.tar.gz
