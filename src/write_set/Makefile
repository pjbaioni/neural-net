CXXFLAGS += -std=c++14 -O3
CPPFLAGS += -I$$mkEigenInc -I./../../include/
OUTPUT_OPTION = -o $@
EXE = write_set.out

.PHONY: all build distclean run

.DEFAULT_GOAL = build

all: build

build: $(EXE)

$(EXE): write_set.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $^ $(OUTPUT_OPTION)

distclean:
	$(RM) $(EXE)

run:
	./$(EXE)
