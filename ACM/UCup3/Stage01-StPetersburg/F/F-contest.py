import math

import sys
sys.set_int_max_str_digits(100000)

def eprint(*args, **kwargs):
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

def solve(dat):
    #eprint()
    L = len(dat)

    orig_value = to_finite_differences([
        0 if v is None else v for v in dat
    ])
    extra_basis = [
        to_finite_differences([1 if i == j else 0 for j in range(L)])
        for i, v in enumerate(dat)
        if v is None
    ]
    easy_basis_value = [
        product((i-j) for j in range(L) if dat[j] is not None) if v is None else 0
        for i, v in enumerate(dat)
    ]
    known_bases = []
    while True:
        v = to_finite_differences(easy_basis_value[:])
        while v and v[-1] == 0:
            v.pop()
        if v:
            known_bases.append(v)
        if len(v) == L:
            break
        for i in range(L):
            if dat[i] is None:
                easy_basis_value[i] *= i

    def clean_vec(a):
        for b in known_bases[::-1]:
            if len(b) > len(a):
                continue
            i = len(b)-1
            f = a[i] // b[i]
            for j in range(len(b)):
                a[j] -= b[j] * f
        return a

    while orig_value:
        #eprint(f"{orig_value = }")
        #eprint(f"{extra_basis = }")
        #assert all(len(x) == len(orig_value) for x in extra_basis)
        n_extra_basis = []
        v = [0 for _ in range(len(orig_value))]
        for u in extra_basis:
            lv = v[-1]
            lu = u[-1]
            if lv == 0:
                v.pop()
                n_extra_basis.append(v)
                v = u
                continue
            if lu == 0:
                u.pop()
                n_extra_basis.append(u)
                continue

            g = math.gcd(abs(lv), abs(lu))
            xv = pow(lv // g, -1, lu // g)
            #assert (g - xv * lv) % lu == 0
            xu = (g - xv * lv) // lu

            yv = lu // g
            yu = -lv // g

            x = clean_vec([xv * curv + xu * curu for curv, curu in zip(v, u)])
            y = clean_vec([yv * curv + yu * curu for curv, curu in zip(v, u)])
            #x = [xv * curv + xu * curu for curv, curu in zip(v, u)]
            #y = [yv * curv + yu * curu for curv, curu in zip(v, u)]
            #assert x[-1] == g
            #assert y[-1] == 0
            y.pop()
            n_extra_basis.append(y)
            v = x

        #eprint(v, n_extra_basis)
        #eprint("new", v)
        #eprint("coeff", len(v), v[-1], g)
        if orig_value[-1] == 0:
            orig_value.pop()
        elif v[-1] == 0 or orig_value[-1] % v[-1] != 0:
            #eprint("Failed")
            break
        else:
            f = orig_value[-1] // v[-1]
            for i in range(len(orig_value)):
                orig_value[i] -= v[i] * f
            #assert orig_value[-1] == 0
            orig_value.pop()
            orig_value = clean_vec(orig_value)
        extra_basis = n_extra_basis

    #eprint("final", orig_value)
    #eprint("basis", extra_basis)
    while orig_value and orig_value[-1] == 0:
        orig_value.pop()
    return max(len(orig_value) - 1, 0)

for _ in range(int(input())):
    N = int(input())
    dat = [None for _ in range(30)]
    for _ in range(N):
        x, y = map(int, input().split())
        dat[x-1] = y
    print(solve(dat))
