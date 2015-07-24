CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O2 -Wshadow -Wformat=2 -Wfloat-equal -Wconversion -Wlogical-op -Wcast-qual -Wcast-align
DEBUGFLAGS = -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -fsanitize=address -fsanitize=undefined -fstack-protector -lmcheck -D_FORTIFY_SOURCE=2

TARGET := $(notdir $(CURDIR))
ACTIONS := actions.txt

all: $(ACTIONS)

clean:
	-rm -rf $(TARGET) *.res

%: %.cpp
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(ACTIONS): $(TARGET)
	./$(TARGET)

test: $(ACTIONS)
	java -jar pendulum.jar 1000
	cat score.txt

.PHONY: all clean run test

.PRECIOUS: $(patsubst %,%.res,$(TESTS))
