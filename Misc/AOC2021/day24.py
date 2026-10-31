import z3
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


CHUNK = '''
inp w
mul x 0
add x z
mod x 26
div z v
add x a
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y b
mul y x
add z y
'''.strip()

"""
inp w
x = (z % 26 + a) != w
z /= v
z = 26 * z + (w + b) if x else z
"""

def main(inp, is_real):
    if not is_real:
        return

    inp = inp.strip().split('\n')
    inp = [tuple(x.split()) for x in inp]

    solver = z3.Solver()
    regs = {'w': z3.BitVecVal(0, 400), 'x': z3.BitVecVal(0, 400), 'y': z3.BitVecVal(0, 400), 'z': z3.BitVecVal(0, 400)}
    ans = []
    for op, *args in inp:
        if op == 'inp':
            r = args[0]
            v = z3.BitVec(f'i{len(ans)}', 400)
            solver.add(v >= z3.BitVecVal(1, 400))
            solver.add(v <= z3.BitVecVal(9, 400))
            ans.append(v)
            regs[r] = v
            continue
        r, v = args
        if v in regs:
            v = regs[v]
        else:
            v = int(v)
        if op == 'add':
            regs[r] += v
        elif op == 'mul':
            regs[r] *= v
        elif op == 'div':
            regs[r] = regs[r] / v
        elif op == 'mod':
            regs[r] %= v
        elif op == 'eql':
            regs[r] = z3.If(regs[r] == v, z3.BitVecVal(1, 400), z3.BitVecVal(0, 400))
        else:
            assert False
    solver.add(regs['z'] == 0)
    print(solver.check())
    print(solver.model())
    return

    ans = ''
    i = 0
    chunks = []
    while i < len(inp):
        j = i+1
        while j < len(inp) and inp[j] != ('inp', 'w'):
            j += 1
        cur = inp[i:j]
        i = j
        chunks.append(cur)

    def eval(prog, inps):
        regs = {'w': 0, 'x': 0, 'y': 0, 'z': 0}
        it = iter(list(inps))
        for op, *args in prog:
            if op == 'inp':
                r = args[0]
                regs[r] = next(it)
                continue
            r, v = args
            if v in regs:
                v = regs[v]
            else:
                v = int(v)
            if op == 'add':
                regs[r] += v
            elif op == 'mul':
                regs[r] *= v
            elif op == 'div':
                regs[r] = int(trunc(regs[r] / v))
            elif op == 'mod':
                regs[r] %= v
            elif op == 'eql':
                regs[r] = int(regs[r] == v)
            else:
                assert False
        print(regs)
        return regs['z']

    vals = []
    for cur in chunks:
        print("starting")
        for d in range(1, 10):
            regs = {'w': 0, 'x': 0, 'y': 0, 'z': 0}
            it = iter([d])
            for op, *args in cur:
                if op == 'inp':
                    r = args[0]
                    regs[r] = next(it)
                    continue
                r, v = args
                if v in regs:
                    v = regs[v]
                else:
                    v = int(v)
                if op == 'add':
                    regs[r] += v
                elif op == 'mul':
                    regs[r] *= v
                elif op == 'div':
                    regs[r] = int(trunc(regs[r] / v))
                elif op == 'mod':
                    regs[r] %= v
                elif op == 'eql':
                    regs[r] = int(regs[r] == v)
                else:
                    assert False
            print(d, regs)
            if regs['z'] == 0:
                print(d)
                ans += str(d)
        cur = cur[:]
        v = int(cur[4][2])
        cur[4] = cur[4][:2] + ('v',)
        a = int(cur[5][2])
        cur[5] = cur[5][:2] + ('a',)
        b = int(cur[15][2])
        cur[15] = cur[15][:2] + ('b',)
        s = '\n'.join(' '.join(op) for op in cur)
        assert s == CHUNK
        print(v, a, b)
        vals.append((v, a, b))
        assert 1 <= 1+b and 9+b < 26

    def eval_mine(inps):
        z = []
        assert len(inps) == len(vals)
        for w, (v, a, b) in zip(inps, vals):
            assert v == 1 or v == 26
            x = (z[-1] if z else 0) + a != w
            if v == 26:
                z.pop()
            if x:
                z.append(w+b)
        return not z

    print(vals)
    print(eval(inp, lmap(int, '74929995999389')))
    assert eval_mine(lmap(int, '74929995999389'))



samp_inps = [
r"""
inp w
add z w
mod z 2
div w 2
add y w
mod y 2
div w 2
add x w
mod x 2
div w 2
mod w 2
""",
]

real_inp = r"""
inp w
mul x 0
add x z
mod x 26
div z 1
add x 11
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 14
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 13
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 8
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 11
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 4
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 10
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 10
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -3
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 14
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -4
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 10
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 12
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 4
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -8
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 14
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -3
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 1
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -12
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 6
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 14
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 0
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -6
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 9
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 1
add x 11
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 13
mul y x
add z y
inp w
mul x 0
add x z
mod x 26
div z 26
add x -12
eql x w
eql x 0
mul y 0
add y 25
mul y x
add y 1
mul z y
mul y 0
add y w
add y 12
mul y x
add z y
"""

for samp_inp in samp_inps:
    print("Sample:")
    print(samp_inp.strip())
    print("Result:")
    main(samp_inp, False)

print("Actual:")
main(real_inp, True)
