# coding: utf-8
from itertools import *
def gen():
    def s(a):
        res = 's'
        if a&1: res += 'A'
        if a&2: res += 'B'
        if a&4: res += 'C'
        return res
    for x in range(1,8):
        for y in range(x,8):
            for z in range(y,8):
                assert x <= y <= z
                
                if not any(a&1 and b&2 and c&4 for a,b,c in permutations([x,y,z])): continue

                if x == y == z:
                    print(f"res += g3({s(x)});")
                elif x == y:
                    print(f"res += g2({s(x)}) * {s(z)};")
                elif y == z:
                    print(f"res += {s(x)} * g2({s(y)});")
                else:
                    assert x < y < z
                    print(f"res += {s(x)} * {s(y)} * {s(z)};")
gen()
