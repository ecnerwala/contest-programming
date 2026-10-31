from collections import *

with open('t-bad.in') as f:
    T = int(f.readline())
    assert T == 1
    N = int(f.readline())
    judge_edges = [ list(map(int, f.readline().split())) for _ in range(N) ]
with open('t-bad.out') as f:
    M = int(f.readline())
    my_edges = [ list(map(int, f.readline().split())) for _ in range(M) ]

par = {}
def get_par(a):
    if a not in par or par[a] == a:
        par[a] = a
        return a
    par[a] = get_par(par[a])
    return par[a]
def merge(a, b):
    a = get_par(a)
    b = get_par(b)
    if a == b: return False
    par[a] = b
    return True
parity = defaultdict(lambda:False)
num_edges = 0
for x, y in judge_edges + my_edges:
    parity[x] = not parity[x]
    parity[y] = not parity[y]
    num_edges += merge(x, y)
assert sum(parity.values()) == 2
assert len(par) == len(parity)
assert num_edges == len(par) - 1
