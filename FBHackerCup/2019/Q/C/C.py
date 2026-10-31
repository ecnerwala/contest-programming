from collections import *
from itertools import *
from functools import *

def solve(s):
    v0 = eval(s, None, {'x': 0, 'X': 1})
    v1 = eval(s, None, {'x': 1, 'X': 0})
    return 0 if v0 == v1 else 1

T = int(input())
for t in range(1, T+1):
    s = input()
    ans = solve(s)
    print(f"Case #{t}: {ans}")
