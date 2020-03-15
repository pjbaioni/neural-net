CXXFLAGS += -std=c++14 -O3 -ffast-math
CPPFLAGS += -I/u/sw/pkgs/toolchains/gcc-glibc/7/pkgs/eigen/3.3.3/include/eigen3 -I./../include/
OUTPUT_OPTIONS=-o $@
EXE = main.out
HEADERS=./../include/$(wildcard *.hpp)
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:%.cpp=%.o)

main.o: $(HEADERS) $(SRCS)
NeuralNetwork.o: $(HEADERS) $(filter-out main.cpp,$(SRCS))

.PHONY: all build clean distclean run

.DEFAULT_GOAL = build

all: build

build: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ $(OUTPUT_OPTION)

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(EXE)

run:
	./$(EXE)
