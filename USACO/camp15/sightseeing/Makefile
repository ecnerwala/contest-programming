CXX = g++
CXXFLAGS = -O2 -std=c++0x -Wall -Werror -Wshadow

TARGET = $(notdir $(CURDIR))

all: $(TARGET)

run: $(TARGET)
	time ./$(TARGET)

clean:
	-rm $(TARGET)

.PHONY: all run clean
