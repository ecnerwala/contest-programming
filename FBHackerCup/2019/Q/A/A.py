from collections import *
from itertools import *
from functools import *

def solve(s):
    assert s[0] == 'A'
    assert len(s) >= 2
    s = s[1:]
    cnt = Counter(s)
    cb = cnt['B']
    c0 = cnt['.']
    if cb == 0 or c0 == 0:
        return False

    if cb >= c0:
        return True
    else:
        return False

T = int(input())
for t in range(1, T+1):
    s = input()
    ans = solve(s)
    print(f"Case #{t}: {'Y' if ans else 'N'}")
