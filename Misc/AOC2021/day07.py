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
    inp = inp.strip().split('\n')
    inp = [int(x) for x in inp]

    cprint()



samp_inp = r"""

"""

real_inp = r"""

"""

print("Sample:")
main(samp_inp, False)

print("Actual:")
main(real_inp, True)
