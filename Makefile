targz: binaries
	@echo "Creating file tree in temporary folder tmp/"
	misc/make-tree.sh
	@echo "Creating .tar.gz file"
	tar cvzf mdm.tar.gz tmp/*

binaries: bin/read-devices.c
	gcc -Wall -O2 bin/read-devices.c -o bin/read-devices
	gcc -Wall -O2 bin/write-message.c -o bin/write-message

clean:
	rm -f bin/read-devices
	rm -f bin/write-message
	rm -rf tmp
	rm -rf mdm.tar.gz
