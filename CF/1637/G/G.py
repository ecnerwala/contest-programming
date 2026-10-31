from functools import *
from itertools import *
import sys


@cache
def check(S, V):
    if S == (2,4,6,8,8,8):
        return (S,)
    if False and all(s == V for s in S):
        return (S,)
    for i in range(len(S)):
        for j in range(i+1, len(S)):
            assert S[i] <= S[j]
            if S[i] + S[j] > V:
                continue
            if S[j] == 0:
                continue
            NS = tuple(sorted(S[:i] + S[i+1:j] + S[j+1:] + (S[i] + S[j], S[j] - S[i])))
            if r := check(NS, V):
                return r + (S,)


def go(N):
    for V in count(N):
        if V & (V-1):
            continue
        if r := check(tuple(range(1, N+1)), V):
            return r

print(go(6))
sys.exit()

for a in range(9):
    for b in range(a, 9):
        for c in range(b, 9):
            for d in range(c, 9):
                for e in range(d, 9):
                    if check((a,b,c,d,e), 8):
                        print(a,b,c,d,e)

for i in range(3, 20):
    print(i, go(i))
