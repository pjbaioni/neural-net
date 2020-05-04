CXXFLAGS += -std=c++14 -Ofast#-O3 #-ffast-math
CPPFLAGS += -DNDEBUG -I$$mkEigenInc -I$$mkBoostInc -I./../include/
LDLIBS += -L$$mkBoostLib -lboost_filesystem -lboost_system -lboost_iostreams
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
