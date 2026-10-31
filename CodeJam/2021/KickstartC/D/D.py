import sys
from itertools import *
from collections import *
from functools import *
from random import *
from dataclasses import dataclass
import re

@dataclass(frozen=True)
class MyNum:
    v: int

    def __add__(self, o):
        return MyNum(self.v + o.v)
    def __mul__(self, o):
        return MyNum(self.v * o.v)
    def __matmul__(self, o):
        global cache
        key = (self.v, o.v)
        return cache[key]

def transform(s):
    s = re.sub(r'\b([0-9]+)\b', r'MyNum(\1)', s)
    s = re.sub('#', '@', s)
    return s

def reset():
    global cache
    cache = defaultdict(lambda: MyNum(randrange(1<<32)))

def go(case_num):
    N = int(input())
    funcs = [transform(input()) for _ in range(N)]
    value = [[] for _ in range(N)]
    for z in range(30):
        reset()
        for i in range(N):
            value[i].append(eval(funcs[i]))
    value = list(map(tuple, value))
    num_classes = 0
    mp = {}
    resp = [None for _ in range(N)]
    for i, v in enumerate(value):
        if v not in mp:
            num_classes += 1
            mp[v] = num_classes
        resp[i] = mp[v]
    print(f"Case #{case_num}:", ' '.join(map(str, resp)))

T = int(input())
for case_num in range(T):
    go(case_num+1)
