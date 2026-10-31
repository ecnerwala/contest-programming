import itertools
import functools

def is_good(a, p):
    if sum(a) % p == 0: return False
    for b in itertools.permutations(a):
        tot = 0
        for v in b:
            tot += v
            tot %= p
            if not tot: break
        if tot:
            return True
    return False

def go(n, p):
    cnt = 0
    for a in itertools.product(range(1,p), repeat=n):
        if is_good(a, p):
            cnt += 1
        else:
            if sum(a) % p != 0 and sorted(a) == list(a):
                print(a)
    return cnt

go(9, 5)
