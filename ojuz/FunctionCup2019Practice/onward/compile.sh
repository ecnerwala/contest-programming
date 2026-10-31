#!/bin/bash

problem=onward

g++-7 -Wall -lm -static -DEVAL -o "${problem}" -O2 "grader.cpp" "make.cpp" "guess.cpp"
