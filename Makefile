# FIXME: this Makefile should not make the tree even if there is no need to do
# it

CC = gcc
CFLAGS = -O2 -Wall

all: tree

install: tree
	@echo "Installing..."
	misc/install.sh

targz: tree
	@echo "Creating .tar.gz file"
	tar cvzf mdm.tar.gz tmp/*

tree: binaries
	@echo "Creating file tree in temporary folder tmp/"
	misc/make-tree.sh

binaries: bin/read-devices.c bin/write-message.c
	$(CC) $(CFLAGS) bin/read-devices.c -o bin/read-devices
	$(CC) $(CFLAGS) bin/write-message.c -o bin/write-message `pkg-config --libs --cflags cairo freetype2 x11 xft`

clean:
	rm -f bin/read-devices
	rm -f bin/write-message
	rm -rf tmp
	rm -rf mdm.tar.gz
