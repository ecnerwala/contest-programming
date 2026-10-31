from functools import cache

State = tuple[tuple[int, int] | None, tuple[tuple[int, int],...]]

@cache
def solve(s: State):
    if (s[0][1] if s[0] is not None else 0) + sum(y for x, y in s[1]) == 0:
        # We lost if the opponent is out of cards
        return False
    if s[0] is None:
        for i in range(len(s[1])):
            if s[1][i][0]:
                if not solve(((s[1][i][1], s[1][i][0] - 1), tuple(sorted((y, x) for x, y in s[1][:i] + s[1][i+1:])))):
                    return True
    else:
        # pass
        if not solve((None, tuple(sorted((y, x) for x, y in (s[0],) + s[1])))):
            return True
        if s[0][0]:
            if not solve(((s[0][1], s[0][0] - 1), tuple(sorted((y, x) for x, y in s[1])))):
                return True

    return False


def my_solve(s: State):
    assert sum(x for x, y in s[1]) != 0
    assert sum(y for x, y in s[1]) != 0
    assert s[1] == tuple(sorted(s[1]))
    assert s[0] is None

    m1 = (0, 0)
    m2 = (0, 0)
    for x, y in s[1]:
        cnd = (min(x, y), (y > x) - (y < x))
        if cnd > m1:
            m2, m1 = m1, cnd
        elif cnd > m2:
            m2 = cnd
    if m1[1] == +1 or m2[1] == +1:
        return True
    elif m1[1] == -1 or m1[0] == m2[0]:
        return False
    else:
        return True


    v = max(min(x, y) for x, y in s[1])
    biggest = tuple((x, y) for x, y in s[1] if min(x, y) == v)
    if all(x >= y for x, y in biggest) and len(biggest) >= 2:
        return False
    if len(biggest) == 1 and all(x > y for x, y in biggest):
        # next biggest must be ==
        v2 = max(min(x, y) for x, y in s[1] if min(x, y) != v)
        if all(x >= y for x, y in s[1] if min(x, y) == v2):
            return False
    return True


def format_pair(a: tuple[int, int] | None) -> str:
    if a is None:
        return "x"
    return str(min(a)) + "-=+"[(a[1] > a[0]) - (a[0] > a[1]) + 1]

def format_state(s: State):
    return format_pair(s[0]) + " : " + " ".join(format_pair(x) for x in s[1])


def test():
    for a in range(6):
        for b in range(6):
            for c in range(6):
                for d in range(6):
                                    state = (None, ((a, b), (c, d)))
                                    if sum(x for x, y in state[1]) == 0 or sum(y for x, y in state[1]) == 0:
                                        continue
                                    if state[1] != tuple(sorted(state[1])):
                                        continue
                                    ans = solve(state)
                                    res = my_solve(state)
                                    if ans != res:
                                        print(format_state(state), ans)
    for a in range(6):
        for b in range(6):
            for c in range(6):
                for d in range(6):
                    for e in range(6):
                        for f in range(6):
                                    state = (None, ((a, b), (c, d), (e, f)))
                                    if sum(x for x, y in state[1]) == 0 or sum(y for x, y in state[1]) == 0:
                                        continue
                                    if state[1] != tuple(sorted(state[1])):
                                        continue
                                    ans = solve(state)
                                    res = my_solve(state)
                                    if ans != res:
                                        print(format_state(state), ans)
    for a in range(6):
        for b in range(6):
            for c in range(6):
                for d in range(6):
                    for e in range(6):
                        for f in range(6):
                            for g in range(6):
                                for h in range(6):
                                    state = (None, ((a, b), (c, d), (e, f), (g, h)))
                                    if sum(x for x, y in state[1]) == 0 or sum(y for x, y in state[1]) == 0:
                                        continue
                                    if state[1] != tuple(sorted(state[1])):
                                        continue
                                    ans = solve(state)
                                    res = my_solve(state)
                                    if ans != res:
                                        print(format_state(state), ans)

    print("Testing len 5")

    for a in range(4):
        for b in range(4):
            for c in range(4):
                for d in range(4):
                    for e in range(4):
                        for f in range(4):
                            for g in range(4):
                                for h in range(4):
                                    for i in range(4):
                                        for j in range(4):
                                            state = (None, ((a, b), (c, d), (e, f), (g, h), (i, j)))
                                            if sum(x for x, y in state[1]) == 0 or sum(y for x, y in state[1]) == 0:
                                                continue
                                            if state[1] != tuple(sorted(state[1])):
                                                continue
                                            ans = solve(state)
                                            res = my_solve(state)
                                            if ans != res:
                                                print(format_state(state), ans)

    for a in range(6):
        for b in range(6):
            for c in range(6):
                for d in range(6):
                    for e in range(6):
                            state = (None, ((a, a), (b, b), (c, c), (d, d), (e, e)))
                            if sum(x for x, y in state[1]) == 0 or sum(y for x, y in state[1]) == 0:
                                continue
                            if state[1] != tuple(sorted(state[1])):
                                continue
                            ans = solve(state)
                            res = my_solve(state)
                            if ans != res:
                                print(format_state(state), ans)

test()
