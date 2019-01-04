CXXFLAGS = --std=c++11 -O2 -Wall -Wextra -Wshadow

all: main

run: main
	./main

prog.txt: main
	./main > prog.txt

clean:
	rm main

.PHONY: all clean run
