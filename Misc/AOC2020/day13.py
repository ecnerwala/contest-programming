from functools import *
from collections import *
from itertools import *
from math import *
import re
from builtins import pow
from heapq import heappush, heappop, heappushpop, heapify, heapreplace

# false for a while, then true
def binary_search(monotone_f, definitely_false, definitely_true):
    assert monotone_f(definitely_false) == False
    assert monotone_f(definitely_true) == True
    lo = definitely_false
    hi = definitely_true
    while hi - lo > 1:
        md = (lo + hi) // 2
        assert definitely_false < md < definitely_true
        if monotone_f(md):
            lo = md
        else:
            hi = md
    assert hi == lo + 1
    assert monotone_f(lo) == False
    assert monotone_f(hi) == True

    #return lo
    #return hi


def tree_dfs(cur, prv=-1):
    # do stuff about cur here
    for nxt in adj[cur]:
        if nxt == prv: continue
        # do stuff
        tree_dfs(nxt, cur)
    # or do stuff about cur here 

def modinv(a, m):
    a %= m
    if a == 1 % m: return a
    assert a != 0
    return m - modinv(m, a) * m // a

def crt(x,y):
    a,m1 = x
    b,m2 = y

    a %= m1
    b %= m2
    g = gcd(m1, m2)
    assert a % g == b % g
    return (a + (b-a)*modinv(m1//g,m2//g)%(m2//g)*(m1//g), m1//g*m2)


def main(inp, is_real):
    inp = inp.strip()
    t0, vals = inp.split('\n')
    t0 = int(t0)
    vals = list(vals.split(','))
    t = (0,1)
    for i, v in enumerate(vals):
        if v == 'x': continue
        v = int(v)
        t = crt(t, (-i, v))
    print(t[0])


def main_orig(inp, is_real):
    inp = inp.strip()
    t0, vals = inp.split('\n')
    t0 = int(t0)
    vals = list(vals.split(','))
    t = 0
    m = 1
    for i, v in enumerate(vals):
        if v == 'x': continue
        v = int(v)
        while (t + i) % v != 0: t += m
        m = lcm(m,v)
    print(t)


samp_inp = r"""
939
7,13,x,x,59,x,31,19
"""

real_inp = r"""
1005162
19,x,x,x,x,x,x,x,x,41,x,x,x,x,x,x,x,x,x,823,x,x,x,x,x,x,x,23,x,x,x,x,x,x,x,x,17,x,x,x,x,x,x,x,x,x,x,x,29,x,443,x,x,x,x,x,37,x,x,x,x,x,x,13
"""

print("Sample:")
main(samp_inp, False)

print("Actual:")
main(real_inp, True)
