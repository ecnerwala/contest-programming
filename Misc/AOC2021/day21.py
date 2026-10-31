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


def main_old(inp, is_real):
    locs = list(inp)
    cur_turn = 0
    scores = [0, 0]
    num_rolls = 0

    last_roll = 100
    def roll():
        nonlocal last_roll
        if last_roll == 100:
            last_roll = 0
        last_roll += 1
        return last_roll

    while max(scores) < 1000:
        num_rolls += 3
        d = roll() + roll() + roll()
        locs[cur_turn] = (locs[cur_turn] + d - 1) % 10 + 1
        scores[cur_turn] += locs[cur_turn]

        cur_turn = 1 ^ cur_turn

    print(scores, num_rolls)
    cprint(min(scores) * num_rolls)


def main(inp, is_real):
    @cache
    def num_wins(locs, cur_turn, scores):
        if max(scores) >= 21:
            return (cur_turn == 1, cur_turn == 0)
        r = [0, 0]
        for a in range(3):
            for b in range(3):
                for c in range(3):
                    nlocs = list(locs)
                    nscores = list(scores)
                    d = a + b + c + 3
                    nlocs[cur_turn] = (nlocs[cur_turn] + d - 1) % 10 + 1
                    nscores[cur_turn] += nlocs[cur_turn]
                    ch = num_wins(tuple(nlocs), 1 ^ cur_turn, tuple(nscores))
                    r[0] += ch[0]
                    r[1] += ch[1]
        return tuple(r)

    a = num_wins(inp, 0, (0, 0))
    print(a)
    cprint(max(a))


samp_inps = [
    (4, 8),
]

real_inp = (3, 10)

for samp_inp in samp_inps:
    print("Sample:")
    print(samp_inp)
    print("Result:")
    main(samp_inp, False)

print("Actual:")
main(real_inp, True)
