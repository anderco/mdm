VERSION = 0.0.1

all: tree

install: tree
	@echo "Installing..."
	@misc/install.sh $(DESTDIR)


debian-lenny-install: debian-lenny
	@echo "Installing the mdm-xephyr-gdm package"
	@dpkg -i packages/mdm-xephyr-gdm_$(VERSION)_i386.deb

debian-lenny: tree
	@echo "Creating debian-lenny-tree"
	@cp -r tree debian-lenny-tree
	@cp -r distro/debian-lenny/DEBIAN debian-lenny-tree
	@mkdir -p packages
	@echo "Creating .deb in packages/"
	@fakeroot dpkg -b debian-lenny-tree packages

ubuntu-8.04-install: ubuntu-8.04
	@echo "Installing the mdm-xephyr-gdm package"
	@dpkg -i packages/mdm-xephyr-gdm_$(VERSION)_i386.deb

ubuntu-8.04: tree
	@echo "Creating ubuntu-8.04-tree"
	@cp -r tree ubuntu-8.04-tree
	@cp -r distro/ubuntu-8.04/DEBIAN ubuntu-8.04-tree
	@echo "Applying patches"
	@cd ubuntu-8.04-tree && \
		patch -p1 < ../distro/ubuntu-8.04/gdm.conf.patch
	@mkdir -p packages
	@echo "Creating .deb in packages/"
	@fakeroot dpkg -b ubuntu-8.04-tree packages

tree:
	@echo "Creating file tree in temporary folder tree/"
	@misc/make-tree.sh

clean:
	rm -rf *tree
	rm -rf packages
