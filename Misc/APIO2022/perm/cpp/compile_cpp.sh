#!/bin/bash

problem=perm

g++ -std=gnu++17 -O2 -pipe -static -o $problem grader.cpp $problem.cpp
