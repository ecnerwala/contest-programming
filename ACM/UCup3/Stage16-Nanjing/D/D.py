from functools import cache

st = frozenset

@cache
def neg(game):
    return st(neg(v) for v in game[1]), st(neg(v) for v in game[0])


@cache
def eval_game(game, cur_player):
    # Returns whether l wins
    return any(not eval_game(g, 1-cur_player) for g in game[cur_player])

@cache
def add(a, b):
    return (
        st(add(a, x) for x in b[0]) | st(add(b, x) for x in a[0]),
        st(add(a, x) for x in b[1]) | st(add(b, x) for x in a[1]),
    )

@cache
def sub(a, b):
    return add(a, neg(b))

@cache
def comp(a, b):
    v0 = eval_game(sub(a, b), 0)
    v1 = eval_game(sub(a, b), 1)
    return v0, v1


def sgn(a, b):
    v0, v1 = comp(a, b)
    if v0 > v1:
        return '>'
    elif v0 < v1:
        return '<'
    elif v0 and v1:
        return '~'
    else:
        return '='


def make(l, r):
    return st(l), st(r)

g0 = make([], [])
g1 = make([g0], [])
g2 = make([g1], [])
g3 = make([g2], [])
half = make([g0], [g1])
g1_5 = make([g1], [g2])

star = make([g0], [g0])

g1_star = add(g1, star)

quarter = make([g0], [half])

generation = []
names = {
    g0: '0',
    g1: '1',
    g2: '2',
    star: '*',
    half: '1/2',
    quarter: '1/4',
}
def lookup(a):
    for k, v in names.items():
        if sgn(a, k) == '=':
            return "=" + v
    res = ""
    for k, v in names.items():
        if sgn(a, k) == '~':
            res += "~" + v + ", "
    if not res:
        for k, v in names.items():
            print(sgn(a, k), v)
        return "?"
    return res

print(sgn(g0, g0))
print(sgn(g1, g0))
print(sgn(make([g0], [g1_star]), half))
print(sgn(make([g0], [g1_star]), g1))
print(sgn(make([half], [g1_star]), g1))
print(sgn(make([half], [g1_star]), g1))
quarter = make([g0], [half])
print(sgn(make([star], [g1]), g0))
print(lookup(make([star], [g1])))
print(lookup(make([star], [g1])))
print(lookup(make([g1_star], [g1_star])))
print(lookup(make([g0], [g1_star])))
print(lookup(make([g1], [add(g2, star)])))
print(lookup(make([g0], [half, add(g1, star)])))
print(lookup(make([star], [half])))
print(lookup(make([g1_star], [g1_5])))
print(lookup(make([half], [g1_star])))
print(lookup(make([g0], [g1, g1_star])))
print(lookup(make([star], [g0])))
