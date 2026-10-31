from functools import *
from collections import *
from itertools import *
from math import *
import re
from builtins import pow
from heapq import heappush, heappop, heappushpop, heapify, heapreplace
#import pyperclip


def cprint(a):
    print(a)
    #pyperclip.copy(a)


def main(inp, is_real):
    inp = inp.strip()
    inp = list(map(int, inp.split(',')))

    a = []
    #last_time = [[] for _ in range(4*10**7)]
    last_time = defaultdict(list)

    for i, v in enumerate(inp):
        a.append(v)
        last_time[v].append(i)

    n = 30000000
    #n = 2020
    for i in range(len(inp), n+10):
        if i % 1000000 == 0:
            print("got ", i)
        last = a[i-1]
        x = last_time[last]
        if len(x) == 1:
            v = 0
        else:
            v = x[-1]-x[-2]
        a.append(v)
        last_time[v].append(i)
        #last_time[v] = last_time[v][-2:]
    cprint(a[n-1])


samp_inp = r"""
0,3,6
"""

real_inp = r"""
16,11,15,0,1,7
"""

print("Sample:")
main(samp_inp, False)

print("Actual:")
main(real_inp, True)
