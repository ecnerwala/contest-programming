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
    inp = inp.split('\n\n')
    decks = tuple(tuple(map(int, p.split('\n')[1:])) for p in inp)

    final_ans = 0

    @lru_cache(maxsize=None)
    def solve(decks):
        seen = set()
        while all(decks):
            if decks in seen:
                return 0
            seen.add(decks)

            a, *x = decks[0]
            b, *y = decks[1]
            x = tuple(x)
            y = tuple(y)
            if len(x) >= a and len(y) >= b:
                round_winner = solve((x[:a], y[:b]))
            else:
                round_winner = 0 if a > b else 1

            if round_winner == 0:
                decks = (x + (a, b), y)
            else:
                decks = (x, y + (b, a))

        nonlocal final_ans
        final_ans = sum((i+1) * v for d in decks for i,v in enumerate(d[::-1]))
        return 1 if decks[1] else 0

    solve(decks)
    cprint(final_ans)

    #exit(0)


samp_inp = r"""
Player 1:
9
2
6
3
1

Player 2:
5
8
4
7
10
"""

real_inp = r"""
Player 1:
29
30
44
35
27
2
4
38
45
33
50
21
17
11
25
40
5
43
41
24
12
19
23
8
42

Player 2:
32
13
22
7
31
16
37
6
10
20
47
46
34
39
1
26
49
9
48
36
14
15
3
18
28
"""

print("Sample:")
main(samp_inp, False)

print("Actual:")
main(real_inp, True)
