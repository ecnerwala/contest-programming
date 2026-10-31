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
    if not is_real: return

    inp = inp.strip()
    inp = inp.split('\n')

    a, b = map(int, inp)

    BASE = 7
    MOD = 20201227

    x = 0
    while pow(BASE, x, MOD) != a: x += 1
    cprint(pow(b, x, MOD))


    #exit(0)



samp_inp = r"""

"""

real_inp = r"""
15113849
4206373
"""

print("Sample:")
main(samp_inp, False)

print("Actual:")
main(real_inp, True)
