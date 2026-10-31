from collections import *
from functools import cache

# a tree is a pair of label, child forest
# a forest is a sorted tuple of trees
@cache
def get_strings(forest) -> tuple[str]:
    if len(forest) == 0:
        return tuple([''])
    res = []
    for i in range(len(forest)):
        for j in range(len(forest)):
            if forest[i][0] == 'A' and forest[j][0] == 'B':
                res.extend(get_strings(tuple(sorted(
                    forest[:min(i, j)]
                    + forest[min(i,j) + 1:max(i, j)]
                    + forest[max(i,j) + 1:]
                    + forest[i][1]
                    + forest[j][1]
                ))))
    for i in range(len(forest)):
        for s in get_strings(tuple(sorted(
            forest[:i] + forest[i+1:] + forest[i][1]
        ))):
            res.append(forest[i][0] + s)
    return tuple(sorted(set(res)))

@cache
def get_trees(n, top_char):
    assert n >= 1
    res = []
    for v in get_forests(n-1, top_char='B' if top_char == 'A' else 'A'):
        res.append((top_char, v))
    return sorted(res)

@cache
def get_forests(n, top_char=None):
    res = []
    if n == 0:
        res.append(())
    for c in top_char if top_char else 'AB':
        for j in range(1, n+1):
            for f in get_forests(n-j, top_char=top_char):
                for t in get_trees(j, c):
                    if not f or t <= f[0]:
                        res.append((t,) + f)
    return res

def get_count(a):
    if not a:
        return (0, 0)
    ca = 0
    cb = 0
    for t, s in a:
        if t == 'A':
            ca += 1
        else:
            cb += 1
        na, nb = get_count(s)
        ca += na
        cb += nb
    return ca, cb

by_vals = defaultdict(list)
primitives = {}
all_forests = [f for n in range(12) for f in get_forests(n)]

for f in all_forests:
    if len(get_strings(f)) == 1:
        primitives[get_strings(f)[0]] = f

for s in primitives:
    for i in range(0, len(s)-1):
        for j in range(i+1, len(s)-1):
            assert s[i] != s[i+1] or s[j] == s[j+1]

def get_prim_strings(f):
    return tuple(s for s in get_strings(f) if s in primitives)

for f in all_forests:
    prim_strings = get_prim_strings(f)
    #print(f, prim_strings)
    by_vals[prim_strings].append(f)

my_types = dict()
for i in range(12):
    for lo in range(12):
        for hi in range(lo, 12):
            for extra in 'AB':
                for starts in [['AB'], ['BA'], ['AB', 'BA']]:
                    if i > 0 and lo == 0 and len(starts) == 1 and extra != starts[0][0]:
                        continue
                    if i == 0 and lo == 0 and len(starts) == 1:
                        continue
                    if (i > 0 or hi > 0) and lo == hi and len(starts) == 2:
                        continue
                    cnds = []
                    for s in starts:
                        for j in range(lo, hi+1):
                            cnds.append(s * j + extra * i)
                    if max(map(len, cnds)) >= 12:
                        continue
                    my_types[tuple(sorted(set(cnds)))] = (i, lo, hi, extra, starts)

for v in my_types:
    if v not in by_vals:
        print(v in by_vals, v)

for k, v in sorted(by_vals.items()):
    if k in my_types:
        continue
    print(k)
    for a in v: print(a)
    print()

merge = dict()
for a, x in primitives.items():
    for b, y in primitives.items():
        if len(a) + len(b) >= 12:
            continue
        s = get_prim_strings(tuple(sorted(x + y)))
        merge[(a, b)] = s
        ta = my_types[(a,)]
        tb = my_types[(b,)]
        ts = my_types[s]
        if ta[2] < tb[1] and ts[1] != tb[1] - ta[2]:
            print(a, my_types[(a,)], b, my_types[(b,)], my_types[s])
            #assert ts[1] == tb[1] - ta[2] - 1

for a, x in by_vals.items():
    for b, y in by_vals.items():
        if max(map(len, a), default=0) + max(map(len, b), default=0) >= 12:
            continue
        res_direct = get_prim_strings(tuple(sorted(x[0] + y[0])))
        res_other = []
        for l in get_prim_strings(x[0]):
            for r in get_prim_strings(y[0]):
                res_other.extend(merge[(l, r)])
        res_other = tuple(sorted(set(res_other)))
        if res_direct != res_other:
            print(a, b, res_direct, res_other)
            assert False

