CXX = g++
CXXFLAGS = -O2 -Wall -Wextra -Wshadow --std=c++11

TARGET = $(notdir $(CURDIR))

all: $(TARGET).class

%.class: %.java
	javac $^

$(TARGET): $(TARGET).cpp

%.res: $(TARGET).class %.in
	time java $(TARGET) < $*.in | tee $*.res

diff_%: %.res %.out
	diff $*.res $*.out

run: $(TARGET).class
	time java $(TARGET)

clean:
	rm -rf $(TARGET) *.res

.PHONY: all clean run
