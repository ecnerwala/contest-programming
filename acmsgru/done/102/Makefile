CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align
DEBUGFLAGS = -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -fsanitize=address -fsanitize=undefined -fstack-protector -lmcheck -D_FORTIFY_SOURCE=2

TARGET := 102
EXECUTE := ./$(TARGET)

CASES := $(sort $(basename $(wildcard *.in)))
TESTS := $(sort $(basename $(wildcard *.out)))

all: $(TARGET)

clean:
	-rm -rf $(TARGET) *.res

%: %.cpp
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

run: $(TARGET)
	time $(EXECUTE)

%.res: $(TARGET) %.in
	time $(EXECUTE) < $*.in > $*.res

__test_%: %.res %.out
	diff $*.res $*.out

runs: $(patsubst %,%.res,$(CASES))

solve: runs

test: $(patsubst %,__test_%,$(TESTS))

.PHONY: all clean run test __test_% runs solve

.PRECIOUS: %.res
