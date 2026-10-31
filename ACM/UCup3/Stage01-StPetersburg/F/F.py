import math


def eprint(*args, **kwargs):
    import sys
    print(*args, **kwargs, file=sys.stderr)

def to_finite_differences(seq):
    res = []
    while seq:
        res.append(seq[0])
        seq = [b - a for a, b in zip(seq, seq[1:])]
    return res

def product(vals):
    r = 1
    for v in vals:
        r *= v
    return r

def solve(dat, flg):
    #eprint()
    L = len(dat)
    fact = [1]
    while len(fact) <= L:
        fact.append(fact[-1] * len(fact))

    num_known = sum(v is not None for v in dat)
    eprint(num_known)

    naive_lagrange = []
    for i in range(L):
        v = 0
        for j in range(L):
            if dat[j] is None:
                continue
            numer = product((i-k) for k in range(L) if dat[k] is not None and j != k)
            denom = product((j-k) for k in range(L) if dat[k] is not None and j != k)
            assert fact[L-1] % denom == 0
            v += numer * (fact[L-1] // denom) * dat[j]
        if dat[i] is not None:
            assert v == dat[i] * fact[L-1]
        naive_lagrange.append(v)
    if all(v % fact[L-1] == 0 for v in naive_lagrange):
        for v in naive_lagrange:
            v /= fact[L-1]
        naive_lagrange = to_finite_differences(naive_lagrange)
        while naive_lagrange and naive_lagrange[-1] == 0:
            naive_lagrange.pop()
        assert len(naive_lagrange) <= num_known
        return max(len(naive_lagrange) - 1, 0)

    # Otherwise, we don't care about the lowest num_known terms

    orig_value = to_finite_differences([
        0 if v is None else v for v in dat
    ])[num_known:]
    extra_basis = [
        to_finite_differences([1 if i == j else 0 for j in range(L)])[num_known:]
        for i, v in enumerate(dat)
        if v is None
    ]
    # We can mod values by this
    mods = [fact[L-1 if flg else L] for _ in range(L)]

    def clean_vec(a):
        for i in range(len(a)):
            a[i] %= mods[i]
        return a

    clean_vec(orig_value)
    for v in extra_basis:
        clean_vec(v)

    while orig_value:
        #eprint(f"{orig_value = }")
        #eprint(f"{extra_basis = }")
        assert all(len(x) == len(orig_value) for x in extra_basis)
        n_extra_basis = []
        v = [0 for _ in range(len(orig_value))]
        for u in extra_basis:
            lv = v[-1]
            lu = u[-1]

            g = math.gcd(abs(lv), abs(lu))
            if lu == 0:
                xv = 1 if lv >= 0 else -1
                xu = 0

                yv = 0
                yu = 1
            else:
                xv = pow(lv // g, -1, lu // g)
                #assert (g - xv * lv) % lu == 0
                xu = (g - xv * lv) // lu

                yv = lu // g
                yu = -lv // g

            x = [(xv * curv + xu * curu) % m for curv, curu, m in zip(v, u, mods)]
            y = [(yv * curv + yu * curu) % m for curv, curu, m in zip(v, u, mods)]
            #x = [xv * curv + xu * curu for curv, curu in zip(v, u)]
            #y = [yv * curv + yu * curu for curv, curu in zip(v, u)]
            assert x[-1] == g
            assert y[-1] == 0
            y.pop()
            n_extra_basis.append(y)
            v = x

        #eprint(v, n_extra_basis)
        #eprint("new", v)
        #eprint("coeff", len(v), v[-1], g)
        assert 0 <= orig_value[-1] < mods[len(orig_value)-1]
        if orig_value[-1] == 0:
            orig_value.pop()
        elif v[-1] == 0 or orig_value[-1] % v[-1] != 0:
            #eprint("Failed")
            break
        else:
            f = orig_value[-1] // v[-1]
            for i in range(len(orig_value)):
                orig_value[i] -= v[i] * f
            assert orig_value[-1] == 0
            orig_value.pop()
            orig_value = clean_vec(orig_value)
        extra_basis = n_extra_basis

    #eprint("final", orig_value)
    #eprint("basis", extra_basis)
    assert orig_value
    return max(len(orig_value) + num_known - 1, 0)

for _ in range(int(input())):
    N = int(input())
    dat = [None for _ in range(30)]
    for _ in range(N):
        x, y = map(int, input().split())
        dat[x-1] = y
    while False and dat and dat[-1] is None:
        dat.pop()
    a = solve(dat[:], 0)
    b = solve(dat[:], 1)
    if a != b:
        eprint(N, dat, a, b)
    print(solve(dat, 0))
