all: tree

install: tree
	@echo "Installing..."
	@misc/install.sh $(DESTDIR)

tree:
	@echo "Creating file tree in temporary folder tree/"
	@misc/make-tree.sh

clean:
	rm -rf tree
	rm -rf packages
