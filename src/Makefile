mkEigenInc ?= /usr/include/eigen3
mkBoostInc ?= /usr/include
mkBoostLib ?= /usr/lib
# could be /usr/lib/x86_64-linux-gnu too
CXXFLAGS += -std=c++20 -Ofast -Wall -Wextra
CPPFLAGS += -DNDEBUG -I$(mkEigenInc) -I$(mkBoostInc) -I./../include
LDFLAGS += -L$(mkBoostLib)
LDLIBS += -lboost_filesystem -lboost_iostreams
# add -lboost_system only if your Boost installation requires it
OUTPUT_OPTION=-o $@
EXE = main.out
HEADERS=./../include/$(wildcard *.hpp)
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:%.cpp=%.o)
OPTIONS=#if_empty_minimal_output
GENERATED_DATA=./../data/Yhat*.dat

main.o: $(HEADERS) $(SRCS)
NeuralNetwork.o: $(HEADERS) $(filter-out main.cpp,$(SRCS))

.PHONY: all build clean distclean run

.DEFAULT_GOAL = build

all: build

build: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDLIBS) $^ $(OUTPUT_OPTION)

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) $(EXE)
	$(RM) $(GENERATED_DATA)

run:
	./$(EXE) $(OPTIONS)
