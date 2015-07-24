# Set the command for your C++ compiler, and specify any compiler flags you
# want to use (e.g. -g -Werror).
CXX			= g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align

# The driver outputs TAP (Test Anything Protocol), so it can also be used with
# any TAP test harness (e.g. prove).  Set the path to your test harness here,
# then use the `prove' target to run the tests with that harness.
PROVE		= prove
PROVEFLAGS	= -e ""

all: driver

clean:
	rm -f driver

distclean: clean
	rm -f Bitwisdom

run: all
	./driver

test: all
	./driver --abort-on-fail

prove: all
	$(PROVE) $(PROVEFLAGS) ./driver


%: %.cc
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

driver: Bitwisdom.cc

.PHONY: all clean distclean run test prove

# vim:ft=make
