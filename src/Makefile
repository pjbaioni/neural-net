CXXFLAGS += -std=c++14 -Ofast#-O3 -ffast-math
CPPFLAGS += -DNDEBUG -I$$mkEigenInc -I./../include/
OUTPUT_OPTION=-o $@
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
