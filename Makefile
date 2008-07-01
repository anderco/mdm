CC = gcc
CFLAGS = -02 -Wall

install: tree
	@echo "Installing..."
	misc/install.sh

tree: binaries
	@echo "Creating file tree in temporary folder tmp/"
	misc/make-tree.sh

targz: tree
	@echo "Creating .tar.gz file"
	tar cvzf mdm.tar.gz tmp/*

binaries: bin/read-devices.c
	$(CC) $(CFLAGS) bin/read-devices.c -o bin/read-devices
	$(CC) $(CFLAGS) bin/write-message.c -o bin/write-message

clean:
	rm -f bin/read-devices
	rm -f bin/write-message
	rm -rf tmp
	rm -rf mdm.tar.gz
