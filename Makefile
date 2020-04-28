SUBDIRS = ./src/write_set ./src

.PHONY: all build clean distclean help run test1 test2 test3 verbose_run
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
	@echo " - make run to run the main program with default parameters and options"
	@echo " - make test1 to execute the first test case"
	@echo " - make test2 to execute the second test case"
	@echo " - make test3 to execute the third test case"
	@echo " - make verbose_run to do as make run but in verbose mode"
run:
	for dir in $(SUBDIRS) ; do \
  $(MAKE) run -C $$dir ; \
  done
test1:
	cd ./src/write_set && ./write_set.out -p "./../../data/test1.pot"
	cd ./src && ./main.out -p "./../data/test1.pot"
test2:
	cd ./src/write_set && ./write_set.out -p "./../../data/test2.pot"
	cd ./src && ./main.out -p "./../data/test2.pot"
test3:
	cd ./src/write_set && ./write_set.out -p "./../../data/test3.pot"
	cd ./src && ./main.out -p "./../data/test3.pot"
verbose_run:
	$(MAKE) run -C ./src/write_set
	cd ./src && ./main.out --verbose
 

