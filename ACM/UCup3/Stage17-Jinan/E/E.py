import sys

lines = sys.stdin.readlines()
for line in lines[1:]:
    A, B, C, X, Y, D = map(int, line.split())

    ans = (D + X - 1) // X * A

    for cntA in [
        1,
        D // X,
        D // X + 1,
        D // (X + Y),
        D // (X + Y) + 1,
    ]:
        if not cntA:
            continue
        dist_rem = max(0, D - cntA * X)
        max_B = min(dist_rem, cntA * Y)
        if dist_rem >= Y:
            ans = min(ans, cntA * A + B * Y + C * (dist_rem - Y))
        ans = min(ans, cntA * A + B * max_B + C * (dist_rem - max_B))
    print(ans)

