CXX = g++
CXXFLAGS = -O2 -std=c++0x -Wall -Wshadow -Werror

TARGET = $(notdir $(CURDIR))

all: $(TARGET)

run: $(TARGET)
	time ./$(TARGET)

clean:
	rm -rf $(TARGET)

.PHONY: all run clean
