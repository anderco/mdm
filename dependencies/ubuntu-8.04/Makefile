all: xephyr libx11

XEPHYR_PACKAGE_NAME = xserver-xephyr
XEPHYR_SOURCE_DIR   = xorg-server-1.4.1~git20080131
XEPHYR_PATCH_FILE   = xephyr-geometry.patch
XEPHYR_DEB_PKG      = xserver-xephyr_1.4.1~git20080131-1ubuntu9.2_i386.deb

LIBX11_PACKAGE_NAME = libx11-6
LIBX11_SOURCE_DIR   = libx11-1.1.3
LIBX11_PATCH_FILE   = libx11-eagain.patch
LIBX11_DEB_PKG      = libx11-6_1.1.3-1ubuntu2_i386.deb 

install: xephyr libx11
	dpkg -i $(XEPHYR_DEB_PKG)
	echo "$(XEPHYR_PACKAGE_NAME) hold" | dpkg --set-selections
	dpkg -i $(LIBX11_DEB_PKG)
	echo "$(LIBX11_PACKAGE_NAME) hold" | dpkg --set-selections

xephyr:
	apt-get source $(XEPHYR_PACKAGE_NAME)
	cd $(XEPHYR_SOURCE_DIR) 			; \
		patch -p1 < ../$(XEPHYR_PATCH_FILE)	; \
		fakeroot dpkg-buildpackage -us -uc	;

libx11:
	apt-get source $(LIBX11_PACKAGE_NAME)
	cd $(LIBX11_SOURCE_DIR) 			; \
		patch -p1 < ../$(LIBX11_PATCH_FILE)	; \
		fakeroot dpkg-buildpackage -us -uc	;

clean:
	rm -f *.deb *.diff.gz *.dsc *.changes *.orig.tar.gz
	rm -rf $(XEPHYR_SOURCE_DIR)
	rm -rf $(LIBX11_SOURCE_DIR)
