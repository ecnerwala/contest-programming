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
ops_by_name = { f.__name__: f for f in op_fns }

with open("day21.in") as f:
    lines = list(f)
lines = [line[:-1] for line in lines]

assert lines[0].startswith("#ip ")
ip_reg = int(lines[0].split()[1])
lines = lines[1:]

regs = [0 for _ in range(6)]
ip_val = 0

regs[0] = 2985446
while 0 <= ip_val < len(lines):
    if ip_val == 28:
        print(ip_val, regs)
        #break
    regs[ip_reg] = ip_val
    instruction, *args = lines[ip_val].split()
    args = tuple(map(int, args))
    a,b,c = args
    ops_by_name[instruction](a,b,c,regs)
    ip_val = regs[ip_reg] + 1

print(regs)
