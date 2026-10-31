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


def tmap(f, n):
    return tuple(map(f, n))


def main(inp, is_real):
    inp = inp.strip().split('\n')
    inp = tuple(tuple(ord(c)-ord('A') for c in x) for x in inp)

    locs = [-2, -1, 1, 3, 5, 7, 8]
    assert len(locs) == 7
    start = (inp, tuple(-1 for _ in range(len(locs))))

    print(start)
    en = (((0,0,0,0),(1,1,1,1),(2,2,2,2),(3,3,3,3)), tuple(-1 for _ in range(len(locs))))

    def print_state(state):
        g = [['.' for _ in range(11)]] + [list('  .#.#.#.  ') for _ in range(5)]
        for l, i in zip(locs, state[1]):
            g[0][l+2] = 'ABCD.'[i]
        for i, s in enumerate(state[0]):
            for j, v in enumerate((s+(-1,-1,-1,-1))[:4]):
                g[4-j][2*i+2] = 'ABCD.'[v]
        print(''.join(g[0]))
        print(''.join(g[1]))
        print(''.join(g[2]))
        print(''.join(g[3]))
        print(''.join(g[4]))

    def get_next_states(state):
        assert len(state[0]) == 4
        assert len(state[1]) == len(locs)

        for i in range(len(locs)):
            if state[1][i] == -1:
                continue
            j = state[1][i]
            if all(v == j for v in state[0][j]) and \
                all(state[1][k] == -1 for k in range(len(locs)) if locs[i] < locs[k] < 2*j or 2*j < locs[k] < locs[i]):
                # can move into place
                cost = abs(locs[i] - 2 * j) + abs(4 - len(state[0][j]))
                cost *= 10 ** j
                nstate = (state[0][:j] + (state[0][j] + (j,),) + state[0][j+1:], state[1][:i] + (-1,) + state[1][i+1:])
                yield nstate, cost
        for j in range(len(state[0])):
            if len(state[0][j]) == 0:
                continue
            for i in range(len(locs)):
                if state[1][i] != -1:
                    continue
                if not all(state[1][k] == -1 for k in range(len(locs)) if locs[i] < locs[k] < 2*j or 2*j < locs[k] < locs[i]):
                    continue
                cost = abs(locs[i] - 2 * j) + abs(4 - len(state[0][j]) + 1)
                cost *= 10 ** state[0][j][-1]
                nstate = (state[0][:j] + (state[0][j][:-1],) + state[0][j+1:], state[1][:i] + (state[0][j][-1],) + state[1][i+1:])
                yield nstate, cost

    dist = {}
    prv = {}
    pq = []
    def try_state(s, c, p):
        if s not in dist or c < dist[s]:
            dist[s] = c
            prv[s] = p
            heappush(pq, (c, s))
    try_state(start, 0, None)
    while pq:
        c, cur = heappop(pq)
        if c != dist[cur]:
            continue
        if cur == en:
            break
        #print(cur, c)
        for nxt, nc in get_next_states(cur):
            #print(' ', nxt, c+nc)
            try_state(nxt, c + nc, cur)

    cprint(dist[en])
    cur = en
    while cur != start:
        print(cur, dist[cur])
        print_state(cur)
        cur = prv[cur]



samp_inps = [
r"""
ADDB
DBCC
CABB
ACAD
""",
]

real_inp = r"""
CDDB
ABCB
DABD
CCAA
"""

for samp_inp in samp_inps:
    print("Sample:")
    print(samp_inp.strip())
    print("Result:")
    main(samp_inp, False)

print("Actual:")
main(real_inp, True)
