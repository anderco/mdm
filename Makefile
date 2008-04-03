#
# Makefile for multiseat-xephyr
#
# 2008-03-2008  Loirto A Santos
#               first version

# package info
_PACK_NAME_=multiseat-xephyr
_VERSION_=2.0.1_i386
_FULLNAME_:=$(_PACK_NAME_)_$(_VERSION_)

# messages
Msg1="Makefile for multiseat-xephyr v1.0"
Msg2="Copyright (c) 2004-2008 Universidade Federal do Parana (UFPR)"
Msg3="Centro de Computacao Cientifica e Software Livre (C3SL)"

default:
	@echo -e $(Msg1)
	@echo -e $(Msg2)
	@echo -e $(Msg3)
	@echo -e "\nUse:"
	@echo -e "\tmake deb\tto generate a .deb package" 
	@echo -e "\tmake tar\tto generate a .tar package"
	@echo -e "\tmake gzip\tto generate a .tar.gz package"
	@echo -e "\tmake bzip\tto generate a .tar.bz2 package\n"

deb:
	@echo -e $(Msg1)
	@echo -e "Generating $(_FULLNAME_).deb package. Please wait..."
	@dpkg -b package .
	@echo -e "Done."

tar:
	@echo -e $(Msg1)
	@echo -e "Generating $(_FULLNAME_).tar package. Please wait..."
	@tar -cf $(_FULLNAME_).tar -C package \
         etc/ usr/
	@echo -e "Done."

gzip:
	@echo -e $(Msg1)
	@echo -e "Generating $(_FULL_NAME_).tar.gz package. Please wait..."
	@tar -czf $(_FULLNAME_).tar.gz -C package \
         etc/ usr/
	@echo -e "Done."

bzip:
	@echo -e $(Msg1)
	@echo -e "Generating $(_FULL_NAME_).tar.bz2 package. Please wait..."
	@tar -cjf $(_FULLNAME_).tar.bz2 -C package \
         etc/ usr/
	@echo -e "Done."
