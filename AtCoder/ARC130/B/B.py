H, W, C, Q = map(int, input().split())
ans = [0 for _ in range(C)]
queries = [tuple(map(int, input().split())) for _ in range(Q)]
queries.reverse()

del_rows = set()
del_cols = set()
for t, n, c in queries:
    c -= 1
    if t == 1:
        if n in del_rows:
            continue
        del_rows.add(n)
        H -= 1
        ans[c] += W
    elif t == 2:
        if n in del_cols:
            continue
        del_cols.add(n)
        W -= 1
        ans[c] += H
    else:
        assert False

print(' '.join(map(str, ans)))
