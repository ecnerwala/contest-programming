all: wiring

wiring: wiring.cpp grader.cpp wiring.h
	./compile_cpp.sh

%.res: %.in wiring
	./wiring < $*.in | tee $*.res

test_%: %.res %.out
	diff -s $*.out $*.res

CASES := $(sort $(basename $(wildcard *.in)))
TESTS := $(sort $(basename $(wildcard *.out)))

test: $(patsubst %,test_%,$(TESTS))
cases: $(patsubst %,%.res,$(CASES))

clean:
	rm -rf wiring *.res

.PHONY: all clean cases test test_%

.PRECIOUS: %.res
