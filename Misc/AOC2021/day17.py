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
    #if is_real: return
    (xlo, xhi), (ylo, yhi) = inp

    ans = 0
    cnt = 0
    s = set()
    for vx0 in range(xhi+10):
        vx = vx0
        x = 0
        t = 0
        while x <= xhi:
            if xlo <= x <= xhi:
                #print(vx0, t)
                assert t > 0
                cons = t * (t-1) // 2
                #for vy in range(-100, 100):
                for vy in range((ylo + cons) // t - 10, (yhi + cons) // t + 10):
                    y = vy * t - t * (t-1) // 2
                    if ylo <= y <= yhi:
                        s.add((vx0, vy))
                        ymax = vy * (vy + 1) // 2
                        ans = max(ans, ymax)
            x += vx
            if vx > 0:
                vx -= 1
            t += 1
            if t >= 1000:
                break
    
    if not is_real:
        a = """
        23,-10  25,-9   27,-5   29,-6   22,-6   21,-7   9,0     27,-7   24,-5
25,-7   26,-6   25,-5   6,8     11,-2   20,-5   29,-10  6,3     28,-7
8,0     30,-6   29,-8   20,-10  6,7     6,4     6,1     14,-4   21,-6
26,-10  7,-1    7,7     8,-1    21,-9   6,2     20,-7   30,-10  14,-3
20,-8   13,-2   7,3     28,-8   29,-9   15,-3   22,-5   26,-8   25,-8
25,-6   15,-4   9,-2    15,-2   12,-2   28,-9   12,-3   24,-6   23,-7
25,-10  7,8     11,-3   26,-7   7,1     23,-9   6,0     22,-10  27,-6
8,1     22,-8   13,-4   7,6     28,-6   11,-4   12,-4   26,-9   7,4
24,-10  23,-8   30,-8   7,0     9,-1    10,-1   26,-5   22,-9   6,5
7,5     23,-6   28,-10  10,-2   11,-1   20,-9   14,-2   29,-7   13,-3
23,-5   24,-8   27,-9   30,-7   28,-5   21,-10  7,9     6,6     21,-5
27,-10  7,2     30,-9   21,-8   22,-7   24,-9   20,-6   6,9     29,-5
8,-2    27,-8   30,-5   24,-7
"""
        slo = {tuple(map(int, s.split(','))) for s in a.split()}
        print(slo - s)
        print(s - slo)

        print(s)

    print(s)
    cprint(ans)
    cprint(len(s))



samp_inps = [
((20, 30), (-10, -5)),
]

real_inp = ((241, 275), (-75, -49))

for samp_inp in samp_inps:
    print("Sample:")
    print(samp_inp)
    main(samp_inp, False)

print("Actual:")
main(real_inp, True)
