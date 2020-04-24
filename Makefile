SUBDIRS = ./src

.PHONY: all build clean distclean run
.DEFAULT_GOAL = build
all: build
build:
	for dir in $(SUBDIRS) ; do \
  make build -C $$dir ; \
  done
clean:
	for dir in $(SUBDIRS) ; do \
  make clean -C $$dir ; \
  done
distclean:
	for dir in $(SUBDIRS) ; do \
  make distclean -C $$dir ; \
  done
run:
	make run -C ./src
 

