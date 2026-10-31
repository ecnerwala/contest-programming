import sys
from fractions import Fraction

def solve(n_vars, eqns):
    nxt_eqns = []
    for eqn in eqns:
        assert len(eqn) == n_vars + 1
        if all(x == 0 for x in eqn[:n_vars]):
            if eqn[-1] < 0:
                return None
            else:
                continue
        else:
            nxt_eqns.append(eqn)
    eqns = nxt_eqns

    if not eqns:
        return [0 for _ in range(n_vars)]

    assert n_vars > 0
    pos = []
    neg = []
    nxt_eqns = []
    for eqn in eqns:
        if eqn[0] == 0:
            nxt_eqns.append(eqn[1:])
        elif eqn[0] > 0:
            pos.append([v * Fraction(1, eqn[0]) for v in eqn[1:]])
        else:
            neg.append([v * Fraction(1, eqn[0]) for v in eqn[1:]])
    for x in pos:
        for y in neg:
            nxt_eqns.append([a-b for a, b in zip(x, y)])
    nres = solve(n_vars - 1, nxt_eqns)
    if nres is None:
        return None

    vals = nres + [1]
    lower = max((-sum(c * v for c, v in zip(p, vals)) for p in pos), default=-(1<<32))
    upper = min((-sum(c * v for c, v in zip(n, vals)) for n in neg), default=+(1<<32))
    assert lower <= upper
    v0 = (lower + upper) // 2
    return [v0] + nres

def main():
    lines = sys.stdin.read().strip().split('\n')
    var_names = []
    eqns = []
    for l in lines:
        op, args = l.split(maxsplit=1)
        if op == 'V':
            var_names.append(args)
        else:
            *lhs, val, rhs = args.split()
            lhs = list(map(int, lhs))
            rhs = int(rhs)
            if val == '<=':
                lhs = [-x for x in lhs]
                rhs = -rhs
            else:
                assert val == '>='
            lhs.append(-rhs)
            eqns.append(lhs)

    sols = solve(len(var_names), eqns)
    if sols is None:
        print("No solution")
    else:
        for v, val in zip(var_names, sols):
            print(v, ':=', int(val))

main()
