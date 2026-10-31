#!/bin/bash

problem=island

g++-7 -Wall -lm -static -DEVAL -o "${problem}" -O2 "grader.cpp" "${problem}.cpp"
