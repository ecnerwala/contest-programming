from functools import *
from collections import *
from itertools import *
from math import *
import re
from builtins import pow
from heapq import heappush, heappop, heappushpop, heapify, heapreplace
import pyperclip


def cprint(a):
    print(a)
    pyperclip.copy(a)


def main(inp, is_real):
    inp = inp.strip()
    inp = inp.split('\n')

    is_set = set()

    for x,row in enumerate(inp):
        for y,c in enumerate(row):
            if c == '#':
                is_set.add((x,y,0,0))

    for z in range(6):
        nei_count = defaultdict(int)
        for (x,y,z,w) in is_set:
            for dx in [-1,0,1]:
                for dy in [-1,0,1]:
                    for dz in [-1,0,1]:
                        for dw in [-1,0,1]:
                            if (dx,dy,dz,dw) == (0,0,0,0): continue
                            nei_count[(x+dx,y+dy,z+dz,w+dw)] += 1
        next_set = {a for a,v in nei_count.items() if v == 3 or (v == 2 and a in is_set)}
        #next_set = {a for a in is_set if nei_count[a] in (2,3)}
        #next_set = next_set | {a for a,v in nei_count.items() if v == 3 and a not in is_set}
        is_set = next_set
    print(len(is_set))


samp_inp = r"""
.#.
..#
###
"""

real_inp = r"""
####...#
......##
####..##
##......
..##.##.
#.##...#
....##.#
.##.#.#.
"""

print("Sample:")
main(samp_inp, False)

print("Actual:")
main(real_inp, True)
