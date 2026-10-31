from functools import *
from collections import *
from itertools import *
from math import *
from sys import exit
from dataclasses import dataclass
import re
from builtins import pow
from heapq import heappush, heappop, heappushpop, heapify, heapreplace
import pyperclip


def cprint(a):
    print(a)
    pyperclip.copy(a)


def lmap(f, n):
    return list(map(f, n))


def main(inp, is_real):
    inp = inp.strip().split('\n')
    edges = [l.split('-') for l in inp]

    adj = defaultdict(list)
    for u, v in edges:
        adj[u].append(v)
        adj[v].append(u)

    ans = 0
    vis = defaultdict(int)
    num_double = 0
    def dfs(cur):
        nonlocal ans
        nonlocal num_double
        if cur == 'end':
            ans += 1
            return

        if 'a' <= cur[0] <= 'z':
            if vis[cur] >= 2:
                return
            if num_double + (vis[cur] == 1) >= 2:
                return
            num_double += (vis[cur] == 1)
            vis[cur] += 1
        for nxt in adj[cur]:
            if nxt == 'start':
                continue
            dfs(nxt)
        if 'a' <= cur[0] <= 'z':
            vis[cur] -= 1
            num_double -= (vis[cur] == 1)

    dfs('start')

    cprint(ans)



samp_inp = r"""
start-A
start-b
A-c
A-b
b-d
A-end
b-end
"""

real_inp = r"""
dr-of
start-KT
yj-sk
start-gb
of-start
IJ-end
VT-sk
end-sk
VT-km
KT-end
IJ-of
dr-IJ
yj-IJ
KT-yj
gb-VT
dr-yj
VT-of
PZ-dr
KT-of
KT-gb
of-gb
dr-sk
dr-VT
"""

print("Sample:")
main(samp_inp, False)

print("Actual:")
main(real_inp, True)
