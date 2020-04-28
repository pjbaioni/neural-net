SUBDIRS = ./src/write_set ./src

.PHONY: all build clean distclean help run verbose
.DEFAULT_GOAL = build
all: build
build:
	for dir in $(SUBDIRS) ; do \
  $(MAKE) build -C $$dir ; \
  done
clean:
	$(MAKE) clean -C ./src
distclean:
	for dir in $(SUBDIRS) ; do \
  $(MAKE) distclean -C $$dir ; \
  done
help:
	@echo "Type:" 
	@echo " - make, make all or make build to build"
	@echo " - make clean to remove objects"
	@echo " - make distclean to clean and to remove executables and generated datafiles"
	@echo " - make run to run the main program"
	@echo " - make verbose_run to run the main program in verbose mode"
run:
	for dir in $(SUBDIRS) ; do \
  $(MAKE) run -C $$dir ; \
  done
verbose_run:
	$(MAKE) run -C ./src/write_set
	cd ./src && ./main.out --verbose
 

