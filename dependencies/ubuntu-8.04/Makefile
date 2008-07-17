all: libx11

LIBX11_PACKAGE_NAME = libx11-6
LIBX11_SOURCE_DIR   = libx11-1.1.3
LIBX11_PATCH_FILE   = libx11-eagain.patch
LIBX11_DEB_PKG      = libx11-6_1.1.3-1ubuntu2_i386.deb 

install: libx11
	dpkg -i $(LIBX11_DEB_PKG)
	echo "$(LIBX11_PACKAGE_NAME) hold" | dpkg --set-selections

libx11:
	apt-get source $(LIBX11_PACKAGE_NAME)
	cd $(LIBX11_SOURCE_DIR) 			; \
		patch -p1 < ../$(LIBX11_PATCH_FILE)	; \
		fakeroot dpkg-buildpackage -us -uc	;

clean:
	rm -f *.deb *.diff.gz *.dsc *.changes *.orig.tar.gz
	rm -rf $(LIBX11_SOURCE_DIR)
