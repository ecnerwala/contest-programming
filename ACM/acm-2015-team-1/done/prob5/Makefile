CXX = g++
CXXFLAGS = -O2 -Wall --std=c++11 -Wextra -Wshadow

TARGET = $(notdir $(CURDIR))

$(TARGET): $(TARGET).cc

all: $(TARGET)

run: $(TARGET)
	time ./$(TARGET)

clean:
	rm -rf *.res $(TARGET)
