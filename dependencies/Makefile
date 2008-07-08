all: help

help:
	@echo "Select one of the following options:"
	@echo "    - ubuntu-8.04"
	@echo "    - ubuntu-8.04-install"
	@echo "    - clean"

ubuntu-8.04-install: ubuntu-8.04
	@cd ubuntu-8.04 && make install

ubuntu-8.04: ubuntu-8.04/*
	@cd ubuntu-8.04 && make

clean:
	@cd ubuntu-8.04 && make clean
