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


def main(inp, is_real):
    inp = inp.strip()
    inp = list(map(int, inp))

    cups = list(inp)
    for _ in range(100 if is_real else 10):
        cur, a,b,c, *rest = cups
        dest = cur - 1
        while dest not in rest:
            if dest < min(cups):
                dest = max(cups)
                continue
            dest = dest - 1
        i = rest.index(dest)
        cups = rest[:i+1] + [a,b,c] + rest[i+1:] + [cur]
    i = cups.index(1)
    cprint(''.join(map(str,cups[i+1:] + cups[:i])))


    #exit(0)


samp_inp = r"""
389125467
"""

real_inp = r"""
364297581
"""

print("Sample:")
main(samp_inp, False)

print("Actual:")
main(real_inp, True)
