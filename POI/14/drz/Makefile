CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++03 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align -Wno-long-long
DEBUGFLAGS = -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -fsanitize=address -fsanitize=undefined -fstack-protector -lmcheck -D_FORTIFY_SOURCE=2

TARGET := $(notdir $(CURDIR))

CASES := $(sort $(patsubst %.in,%,$(wildcard *.in)))
TESTS := $(sort $(patsubst %.out,%,$(wildcard *.out)))

all: $(TARGET)

clean:
	-rm -rf $(TARGET) *.res

%: %.cpp
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

run: $(TARGET)
	time ./$(TARGET)

%.res: $(TARGET) %.in
	time ./$(TARGET) < $*.in > $*.res

__test_%: %.res %.out
	diff $*.res $*.out

runs: $(patsubst %,%.res,$(CASES))

test: $(patsubst %,__test_%,$(TESTS))

.PHONY: all clean run test runs

.PRECIOUS: $(patsubst %,%.res,$(CASES))
