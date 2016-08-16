CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align
DEBUGFLAGS = -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -fsanitize=address -fsanitize=undefined -fstack-protector -lmcheck -D_FORTIFY_SOURCE=2

TARGET := $(notdir $(CURDIR))

TESTS := $(sort $(patsubst %.out,%,$(wildcard *.out)))
CASES := $(sort $(patsubst %.in,%,$(wildcard *.in)))

all: $(TARGET)

clean:
	-rm -rf $(TARGET) *.res

%: %.cpp
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

run: $(TARGET)
	time ./$(TARGET)

%.res: $(TARGET) %.in
	time ./$(TARGET) < $*.in | tee $*.res

__test_%: %.res
	diff $*.res $*.out

test: $(patsubst %,__test_%,$(TESTS))

solve: $(patsubst %,%.res,$(CASES))

.PHONY: all clean run test solve

.PRECIOUS: $(patsubst %,%.res,$(CASES))
