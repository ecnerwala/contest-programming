CXX = g++
CXXFLAGS = -O2 -Wall -Wextra -Wshadow --std=c++11

TARGET = $(notdir $(CURDIR))

all: $(TARGET)

$(TARGET): $(TARGET).cpp

%.res: $(TARGET) %.in
	time ./$(TARGET) < $*.in | tee $*.res

diff_%: %.res %.out
	diff $*.res $*.out

run: $(TARGET)
	time ./$(TARGET)

clean:
	rm -rf $(TARGET) *.res

.PHONY: all clean run
