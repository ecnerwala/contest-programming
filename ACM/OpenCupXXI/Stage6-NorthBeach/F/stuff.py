def expand(s):
    return ''.join('ab' if c == 'a' else 'a' for c in s)

"""
# Returns the a's and then the b's
def gen(n, flip=False, is_first=True):
    if n == 0: return ([], ['b'])

    sub_a, sub_b = gen(n-1, not flip, False)

    lo, hi = (sub_a, sub_b) if flip else (sub_b, sub_a)

    if flip == n % 2 and not is_first:
        # Special case: move the a$ or b$ to the front
        assert len(hi[0]) == 1
        a = hi[:1] + lo + hi[1:]
    else:
        a = lo + hi

    a = [expand(x) for x in a]
    b = [expand(x)[1:] for x in sub_a]

    return a, b
"""

# Returns all and then the a's
def gen(n, flip=False, is_first=True):
    if n == 0: return (['b'], [])

    sub_all, sub_a = gen(n-1, not flip, False)

    if flip == n % 2 and not is_first:
        # Special case: move the a$ or b$ to the front
        idx = len(sub_a) if flip else len(sub_all) - len(sub_a)
        assert len(sub_all[idx]) == 1
        sub_all = sub_all[idx:idx+1] + sub_all[:idx] + sub_all[idx+1:]

    a = [expand(x) for x in sub_all]
    b = [expand(x)[1:] for x in sub_a]

    return (b + a if flip else a + b), a

FIB = 'b'
for n in range(10):
    s, _ = gen(n)
    print(n, s)
    assert s == sorted(FIB[i:] for i in range(len(FIB)))
    FIB = expand(FIB)

with open('t-small.in', 'w') as inputs:
    with open('t-small.out', 'w') as outputs:
        for N in range(1, 15+1):
            s, _ = gen(N)
            Q = len(s)
            M = 10**9+7
            print(N,Q,M, file=inputs)
            print(*range(1,Q+1), file=inputs)
            print(*(Q+1 - len(v) for v in s), file=outputs)
