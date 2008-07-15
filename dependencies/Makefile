all: help

help:
	@echo "Select one of the following options:"
	@echo "    - debian-lenny"
	@echo "    - debian-lenny-install"
	@echo "    - ubuntu-8.04"
	@echo "    - ubuntu-8.04-install"
	@echo "    - clean"

debian-lenny-install: debian-lenny
	@cd debian-lenny && make install

debian-lenny: debian-lenny/*
	@cd debian-lenny && make

ubuntu-8.04-install: ubuntu-8.04
	@cd ubuntu-8.04 && make install

ubuntu-8.04: ubuntu-8.04/*
	@cd ubuntu-8.04 && make

clean:
	@cd ubuntu-8.04 && make clean
	@cd debian-lenny && make clean
