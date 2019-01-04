#!/usr/bin/env python3
from collections import *
from itertools import *
from functools import *

def addr(a,b,c,regs):
    regs[c] = regs[a]+regs[b]
def addi(a,b,c,regs):
    regs[c] = regs[a] + b
def mulr(a,b,c,regs):
    regs[c] = regs[a]*regs[b]
def muli(a,b,c,regs):
    regs[c] = regs[a]*b
def banr(a,b,c,regs):
    regs[c] = regs[a]&regs[b]
def bani(a,b,c,regs):
    regs[c] = regs[a]&b
def borr(a,b,c,regs):
    regs[c] = regs[a] | regs[b]
def bori(a,b,c,regs):
    regs[c] = regs[a] | b
def setr(a,b,c,regs):
    regs[c] = regs[a]
def seti(a,b,c,regs):
    regs[c] = a
def gtir(a,b,c,regs):
    regs[c] = int(a > regs[b])
def gtri(a,b,c,regs):
    regs[c] = int(regs[a] > b)
def gtrr(a,b,c,regs):
    regs[c] = int(regs[a] > regs[b])
def eqir(a,b,c,regs):
    regs[c] = int(a == regs[b])
def eqri(a,b,c,regs):
    regs[c] = int(regs[a] == b)
def eqrr(a,b,c,regs):
    regs[c] = int(regs[a] == regs[b])

op_fns = [addr, addi, mulr, muli, banr, bani, borr, bori, setr, seti, gtir, gtri, gtrr, eqir, eqri, eqrr]

with open("day16.in") as f:
    lines = list(f)
lines = [line[:-1] for line in lines]
assert len(lines) % 4 == 0
for i in range(3,len(lines),4):
    assert lines[i] == ''

samples = [lines[i:i+3] for i in range(0,len(lines),4)]

validOps = {opCode: set(range(len(op_fns))) for opCode in range(len(op_fns))}

part1 = 0
for before,op,after in samples:
    assert before.startswith("Before: ")
    before = before[8:]
    assert after.startswith("After:  ")
    after = after[8:]
    before = tuple(eval(before))
    op = tuple(map(int, op.split()))
    after = tuple(eval(after))

    numCodes = 0
    curValid = set()
    for ind, op_fn in enumerate(op_fns):
        regs = list(before)
        op_fn(op[1], op[2], op[3], regs)
        if regs == list(after):
            numCodes += 1
            curValid.add(ind)
    validOps[op[0]] = validOps[op[0]] & curValid
    if numCodes >= 3:
        part1 += 1
print(part1)
#print(validOps)
assert validOps == {
    0: {1, 2, 4, 6, 7, 8, 9, 11, 13, 15},
    1: {0, 1, 2, 3},
    2: {9, 14, 15},
    3: {0, 1, 6, 7, 8, 9, 10, 11, 12},
    4: {12, 13},
    5: {6, 7},
    6: {9, 10, 11, 12, 13, 14, 15},
    7: {1, 7, 9, 6},
    8: {6},
    9: {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
    10: {0, 6, 9, 13, 14},
    11: {9, 15},
    12: {11, 15},
    13: {1, 3},
    14: {3},
    15: {2},
}

opCodeTable = {
    0: 4,
    1: 0,
    2: 14,
    3: 8,
    4: 12,
    5: 7,
    6: 10,
    7: 9,
    8: 6,
    9: 5,
    10: 13,
    11: 15,
    12: 11,
    13: 1,
    14: 3,
    15: 2,
}
assert set(opCodeTable.values()) == set(range(len(op_fns)))
assert all(v in validOps[k] for k,v in opCodeTable.items())

regs = [0,0,0,0]
with open("day16.in2") as f:
    for op in f:
        opCode, a, b, c = map(int, op.split())
        op_fns[opCodeTable[opCode]](a,b,c,regs)
print(regs[0])
