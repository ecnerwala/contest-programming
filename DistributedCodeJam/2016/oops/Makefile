DCJ = ./dcj/dcj.sh

NODES = 100

export PYTHONPATH = /usr/bin/python2.7

TARGET := $(notdir $(CURDIR))

TESTS := $(sort $(patsubst %.in,%,$(wildcard *.in)))

all: $(TARGET)

clean:
	-rm -rf $(TARGET) *.res

%: %.cpp %.h
	$(DCJ) build --source=$< --executable=$@

__run_%: %
	$(DCJ) run --executable=$< --nodes=$(NODES)

run: __run_$(TARGET)

%.res: $(TARGET) %.in
	$(DCJ) run --executable=$(TARGET) --nodes=$(NODES) --output=all < $*.in > $*.res

__test_%: %.res
	diff $*.res $*.out

test: $(patsubst %,__test_%,$(TESTS))

.PHONY: all clean run test

.PRECIOUS: $(patsubst %,%.res,$(TESTS))
