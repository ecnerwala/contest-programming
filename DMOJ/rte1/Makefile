CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wcast-qual -Wcast-align -Wno-unused-result -Wno-sign-conversion
DEBUGFLAGS = -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all -fstack-protector -D_FORTIFY_SOURCE=2

CXXFLAGS += -Igrader

DEBUG = false
ifeq ($(DEBUG),true)
	CXXFLAGS += $(DEBUGFLAGS)
endif

INTERACTIVE_TARGET := main-interactive
EXECUTE_INTERACTIVE := ./$(INTERACTIVE_TARGET)
FILE_TARGET := main-file
EXECUTE_FILE := ./$(FILE_TARGET)
CLEAN_TARGETS := $(INTERACTIVE_TARGET) $(FILE_TARGET)

#TARGET := $PROBLEM_NAME.py
#EXECUTE := python3 ./$(TARGET)
#CLEAN_TARGETS :=

DATADIR := grader/DataInteractive
TESTS := $(shell python -c 'import yaml; print(" ".join(v["in"].rstrip(".in") for v in yaml.safe_load(open("grader/init.yml"))["test_cases"][0]["batched"]))')

all: $(INTERACTIVE_TARGET)

clean:
	-rm -rf $(CLEAN_TARGETS) ${DATADIR}/*.res grader/checker

main-interactive: grader/grader.cpp solution.cpp grader/problem.h
	$(LINK.cpp) grader/grader.cpp solution.cpp $(LOADLIBES) $(LDLIBS) -o $@

main-file: solution.cpp
	$(LINK.cpp) solution.cpp $(LOADLIBES) $(LDLIBS) -o $@

%: %.cpp
	$(LINK.cpp) $< $(LOADLIBES) $(LDLIBS) -o $@

%.res: $(INTERACTIVE_TARGET) %.in
	time $(EXECUTE_INTERACTIVE) < $*.in > $*.res
ifeq ($(DEBUG),true)
	@echo "Built with DEBUG flags enabled, code may be slower than normal"
endif

%.out: % # Cancel the builtin rule

__test_%: ${DATADIR}/%.res ${DATADIR}/%.out grader/checker
	cat ${DATADIR}/$*.in ${DATADIR}/$*.out ${DATADIR}/$*.res | ./grader/checker
	@#echo "check(${DATADIR}/$*.res, ${DATADIR}/$*.out)"
	@#python -c 'from grader.checker import check; read = lambda f: open(f, "rb").read(); res = check(read("${DATADIR}/$*.res"), read("${DATADIR}/$*.out"), read("${DATADIR}/$*.in"), 1, 1); print(f"> CheckerResult(passed={res.passed!r},points={res.points!r},feedback={res.feedback!r})"); import sys; sys.exit(not res.passed)'

runs: $(patsubst %,${DATADIR}/%.res,$(CASES))

solve: runs

test: $(patsubst %,__test_%,$(TESTS))

.PHONY: all clean run test __test_% runs solve

.PRECIOUS: %.res
