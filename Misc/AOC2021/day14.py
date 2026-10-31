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
    s, trans = inp.strip().split('\n\n')
    trans = [l.split(' -> ') for l in trans.split('\n')]

    print(trans)

    trans = {tuple(k): v for k, v in trans}

    ans = 0

    last = s[-1]

    s = Counter(zip(s, s[1:]))

    for _ in range(40):
        ns = Counter()
        for a, cnt in s.items():
            for b in ([(a[0], trans[a]), (trans[a], a[1])] if a in trans else [a]):
                ns[b] += cnt
        s = ns
    c = Counter()
    for (a, _), cnt in s.items():
        c[a] += cnt
    c[last] += 1

    print(c)

    (_, x), *_, (_, y) = c.most_common()

    cprint(x - y)



samp_inp = r"""
NNCB

CH -> B
HH -> N
CB -> H
NH -> C
HB -> C
HC -> B
HN -> C
NN -> C
BH -> H
NC -> B
NB -> B
BN -> B
BB -> N
BC -> B
CC -> N
CN -> C
"""

real_inp = r"""
OKSBBKHFBPVNOBKHBPCO

CB -> P
VH -> S
CF -> P
OV -> B
CH -> N
PB -> F
KF -> O
BC -> K
FB -> F
SN -> F
FV -> B
PN -> K
SF -> V
FN -> F
SS -> K
VP -> F
VB -> B
OS -> N
HP -> O
NF -> S
SK -> H
OO -> S
PF -> C
CC -> P
BP -> F
OB -> C
CS -> N
BV -> F
VV -> B
HO -> F
KN -> P
VC -> K
KK -> N
BO -> V
NH -> O
HC -> S
SB -> F
NN -> V
OF -> V
FK -> S
OP -> S
NS -> C
HV -> O
PC -> C
FO -> H
OH -> F
BF -> S
SO -> O
HB -> P
NK -> H
NV -> C
NB -> B
FF -> B
BH -> C
SV -> B
BK -> K
NO -> C
VN -> P
FC -> B
PH -> V
HH -> C
VO -> O
SP -> P
VK -> N
CP -> H
SC -> C
KV -> H
CO -> C
OK -> V
ON -> C
KS -> S
NP -> O
CK -> C
BS -> F
VS -> B
KH -> O
KC -> C
KB -> N
OC -> F
PP -> S
HK -> H
BN -> S
KO -> K
NC -> B
PK -> K
CV -> H
PO -> O
BB -> C
HS -> F
SH -> K
CN -> S
HN -> S
KP -> O
FP -> H
HF -> F
PS -> B
FH -> K
PV -> O
FS -> N
VF -> V
"""

print("Sample:")
main(samp_inp, False)

print("Actual:")
main(real_inp, True)
