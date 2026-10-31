#!/usr/bin/env python3

N = int(input())
for line in input():
    lvalue, rhs = line.split(' ')
    if any(c in rhs for c in '$^#&') in rhs:
