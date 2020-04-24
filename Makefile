SUBDIRS = ./src ./src/write_set

.PHONY: all build clean distclean help run verbose
.DEFAULT_GOAL = build
all: build
build:
	for dir in $(SUBDIRS) ; do \
  make build -C $$dir ; \
  done
clean:
	$(MAKE) clean -C ./src
distclean:
	for dir in $(SUBDIRS) ; do \
  make distclean -C $$dir ; \
  done
help:
	@echo "Type:" 
	@echo " - make, make all or make build to build"
	@echo " - make clean to remove objects"
	@echo " - make distclean to clean and to remove executables"
	@echo " - make run to run the main program"
	@echo " - make verbose to run the main program in verbose mode"
run:
	$(MAKE) run -C ./src
verbose:
	cd ./src && ./main.out --verbose
 

