from functools import *
from itertools import *
from math import *
from builtins import pow


def is_prime(p):
    if p == 1:
        return False
    for i in range(2, p):
        if p % i == 0:
            return False
    return True


@cache
def solve(state):
    s = set()
    for i in range(len(state)):
        p, c = state[i]
        assert c > 0
        if p == 0:
            continue
        assert p > 0

        def check_state(nstate):
            nstate.sort()
            nstate = tuple(filter((lambda a: a[1] > 0), nstate))
            has_0 = any(p == 0 for p, c in nstate)
            if has_0:
                nstate = ((0, 1),) + tuple(filter((lambda a: a[0] > 0), nstate))
            s.add(solve(nstate))

        for j in range(len(state)):
            if state[j][0] == p-1:
                for v in range(1, c+1):
                    nstate = list(state)
                    nstate[i] = (p, c-v)
                    nstate[j] = (p-1, state[j][1]+v)
                    check_state(nstate)
        for v in range(1, c+1):
            nstate = list(state)
            nstate[i] = (p, c-v)
            nstate.append((p-1, 2*v))
            check_state(nstate)
        if c >= p and p % 2 == 1 and is_prime(p):
            nstate = list(state)
            nstate[i] = (p-2, c-p)
            check_state(nstate)

    v = 0
    while v in s:
        v += 1
    print(state, v)
    return v


for a in range(5):
    for b in range(5):
        for c in range(5):
            for d in range(5):
print(solve(((3, 3),)))
