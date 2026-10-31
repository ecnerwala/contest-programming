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
    inp = [int(x) for x in inp]

    cprint()



samp_inps = [
r"""

""",
]

real_inp = r"""

"""

for samp_inp in samp_inps:
    print("Sample:")
    print(samp_inp.strip())
    print("Result:")
    main(samp_inp, False)

print("Actual:")
main(real_inp, True)
