SUBDIRS = ./src/write_set ./src

.PHONY: all build clean distclean help run test1 test2 test3 test4 test5 verbose_run
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
	@echo " - make clean to remove object files"
	@echo " - make distclean to clean and to remove executables and generated datafiles"
	@echo " - make run to run the main program with default parameters and options,"
	@echo "   as specified in data/Parameters.pot"
	@echo " - make test<number> to execute the number-th test case"
	@echo " - make verbose_run to do as make run but in verbose mode"
	@echo "See doc/report.pdf for more informations"
run:
	for dir in $(SUBDIRS) ; do \
  $(MAKE) run -C $$dir ; \
  done
test1:
	cd ./src/write_set && ./write_set.out -p "./../../data/Test1.pot"
	cd ./src && ./main.out -p "./../data/Test1.pot"
test2:
	cd ./src/write_set && ./write_set.out -p "./../../data/Test2.pot"
	cd ./src && ./main.out -p "./../data/Test2.pot"
test3:
	cd ./src/write_set && ./write_set.out -p "./../../data/Test3.pot"
	cd ./src && ./main.out -p "./../data/Test3.pot"
test4:
	cd ./src/write_set && ./write_set.out -p "./../../data/Test4.pot"
	cd ./src && ./main.out -p "./../data/Test4.pot"
test5:
	cd ./src/write_set && ./write_set.out -p "./../../data/Test5.pot"
	cd ./src && ./main.out -p "./../data/Test5.pot"
verbose_run:
	$(MAKE) run -C ./src/write_set
	cd ./src && ./main.out --verbose
 

