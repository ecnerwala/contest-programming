DCJ = ./dcj/dcj.sh

NODES = 10
export PYTHONPATH = /usr/bin/python2.7

TARGET := modnum

INPUTS := $(sort $(basename $(wildcard *.h)))
EXECUTABLES := $(INPUTS)
TESTS := $(sort $(basename $(wildcard *.out)))

all: $(EXECUTABLES)

clean:
	-rm -rf $(EXECUTABLES) *.res Testrun-*.cpp

Testrun-%.cpp: $(TARGET).cpp %.h
	sed -e "/\#include INPUT_HEADER/r $*.h" $< | sed -e "/\#include INPUT_HEADER/d" - >$@

testruns: $(patsubst %,Testrun-%.cpp,$(EXECUTABLES))

%: $(TARGET).cpp %.h
	$(DCJ) build --source=$< --executable=$@ --extra_flags="-DINPUT_HEADER=\"$*.h\""

__run_%: %
	$(DCJ) run --executable=$< --nodes=$(NODES)

run: $(patsubst %,__run_%,$(EXECUTABLES))

%.res: %
	$(DCJ) run --executable=$< --nodes=$(NODES) --output=all | tee $*.res

__test_%: %.res %.out
	diff -s $*.res $*.out

test: $(patsubst %,__test_%,$(TESTS))


%.out: %

.PHONY: all clean run __run_% test __test_% testruns

.PRECIOUS: %.res

.SUFFIXES:
