CXXFLAGS += -std=c++14 -O3
CPPFLAGS += -DNDEBUG -I$$mkEigenInc -I./../../include/
OUTPUT_OPTION = -o $@
EXE = write_set.out
GENERATED_DATA = ./../../data/Train*.dat
GENERATED_DATA += ./../../data/Test*.dat

.PHONY: all build distclean run

.DEFAULT_GOAL = build

all: build

build: $(EXE)

$(EXE): write_set.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ $(OUTPUT_OPTION)

distclean:
	$(RM) $(EXE)
	$(RM) $(GENERATED_DATA)
run:
	./$(EXE)
