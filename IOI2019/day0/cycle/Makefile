CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++14 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -Wno-unused-result -Wno-sign-conversion
DEBUGFLAGS = -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fstack-protector -D_FORTIFY_SOURCE=2

DEBUG = true
ifeq ($(DEBUG),true)
	CXXFLAGS += $(DEBUGFLAGS)
endif

PROBLEM := cycle

TARGET := grader
EXECUTE := ./$(TARGET)
SYSTARGET := sysgrader
SYSEXECUTE := ./$(SYSTARGET)
CLEAN_TARGETS := $(TARGET) $(SYSTARGET)

CASES := $(sort $(basename $(wildcard *.in)))
TESTS := $(sort $(basename $(wildcard *.out)))

SYSTESTS := $(sort $(basename $(notdir $(wildcard tests/*.out))))

all: $(TARGET)

clean:
	-rm -rf $(CLEAN_TARGETS) *.res

$(TARGET): $(PROBLEM).cpp $(PROBLEM).h $(TARGET).cpp
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(SYSTARGET): $(PROBLEM).cpp $(PROBLEM).h $(SYSTARGET).cpp
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

run: $(TARGET)
	time $(EXECUTE)

%.res: $(TARGET) %.in
	time $(EXECUTE) < $*.in > $*.res

tests/%.res: $(SYSTARGET) tests/%.in
	time $(SYSEXECUTE) < tests/$*.in > tests/$*.res

ifeq ($(DEBUG),true)
	@echo "Built with DEBUG flags enabled, code may be slower than normal"
endif

%.out: % # Cancel the builtin rule

__test_%: %.res %.out
	diff $*.res $*.out

__systest_%: tests/%.res tests/%.out
	diff tests/$*.res tests/$*.out

runs: $(patsubst %,%.res,$(CASES))

solve: runs

test: $(patsubst %,__test_%,$(TESTS))
systest: $(patsubst %,__systest_%,$(SYSTESTS))

.PHONY: all clean run test __test_% runs solve __systest_%

.PRECIOUS: %.res tests/%.res
