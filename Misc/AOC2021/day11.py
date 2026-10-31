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
    g = [lmap(int, l) for l in inp]

    ans = 0
    for z in count():
        vis = []
        for i in range(10):
            for j in range(10):
                g[i][j] += 1
                if g[i][j] > 9:
                    vis.append((i, j))
        for x, y in vis:
            for nx in [x-1,x,x+1]:
                for ny in [y-1,y,y+1]:
                    if (nx,ny) == (x,y):
                        continue
                    if not (0 <= nx < 10 and 0 <= ny < 10):
                        continue
                    g[nx][ny] += 1
                    if g[nx][ny] == 10:
                        vis.append((nx, ny))
        for i, j in vis:
            g[i][j] = 0
        ans += len(vis)

        if len(vis) == 100:
            cprint(z+1)
            break

    #cprint(ans)



samp_inp = r"""
5483143223
2745854711
5264556173
6141336146
6357385478
4167524645
2176841721
6882881134
4846848554
5283751526
"""

real_inp = r"""
1224346384
5621128587
6388426546
1556247756
1451811573
1832388122
2748545647
2582877432
3185643871
2224876627
"""

print("Sample:")
main(samp_inp, False)

print("Actual:")
main(real_inp, True)
