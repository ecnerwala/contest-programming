#!/usr/bin/env python3

with open("binary.in") as f:
    input_data = []
    for line in f:
        input_data.append(list(map(int, line.split())))
mult_table = input_data[:10]
funcs = [[mult_table[i][j] for i in range(10)] for j in range(10)]

def funcmul(a, b):
    return [b[a[i]] for i in range(10)]

def funcpow(a, e):
    assert e >= 0
    r = [i for i in range(10)]
    while e:
        if e & 1: r = funcmul(r, a)
        a = funcmul(a, a)
        e >>= 1
    return r

a, b = input_data[10]
base = a
# half-open interval
a += 1
b += 1

def get_dig(i):
    p10 = 10 ** i
    hia = a // p10
    hib = b // p10
    assert hia <= hib

    if hia == hib:
        return funcpow(funcs[hia % 10], b - a)

    assert hia < hib
    loa = a % p10
    pref = funcpow(funcs[hia % 10], p10 - loa)
    hia += 1

    lob = b % p10
    suff = funcpow(funcs[hib % 10], lob)

    funcpows = [funcpow(funcs[d], p10) for d in range(10)]
    # now we have hia .. hib
    if hia // 10 == hib // 10:
        res = pref
        for d in range(hia % 10, hib % 10):
            res = funcmul(res, funcpows[d])
        return funcmul(res, suff)

    assert hia // 10 < hib // 10

    for d in range(hia % 10, 10):
        pref = funcmul(pref, funcpows[d]);
    for d in range(hib % 10 - 1, -1, -1):
        suff = funcmul(funcpows[d], suff)

    mid = [i for i in range(10)]
    for d in range(10):
        mid = funcmul(mid, funcpows[d])

    return funcmul(funcmul(pref, funcpow(mid, hib // 10 - hia // 10 - 1)), suff)

res = 0
for i in range(20):
    res += get_dig(i)[base // (10 ** i) % 10] * (10 ** i)
with open("binary.out", "w") as f:
    f.write(str(res) + '\n')
