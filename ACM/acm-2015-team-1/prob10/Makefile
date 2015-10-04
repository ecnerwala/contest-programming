CXX = g++
CXXFLAGS = -O2 -Wall --std=c++11 -Wextra -Wshadow

TARGET = $(notdir $(CURDIR))

$(TARGET): $(TARGET).cc

all: $(TARGET)

run: $(TARGET)
	time ./$(TARGET)

%.res: $(TARGET) %.in
	time ./$(TARGET) < $*.in | tee $*.res

clean:
	rm -rf *.res $(TARGET)
