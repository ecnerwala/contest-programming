def go():
    L, V1, V2, T, S = map(int, input().split())

    L //= S

    curDepth = 0
    curRem = 1
    while curRem < (V2 - V1 + T - 1) // T:
        nDepth = L // (V2 - T)
        if curDepth < nDepth:
            curRem <<= 1
            curDepth += 1
        else:
            numTake = (V2 - max(V1, L // (nDepth + 1)) - 1) // T
            assert L // (V2 - numTake * T) == nDepth and V2 - numTake * T > V1

            if curRem <= numTake:
                return "impossible"

            V2 -= numTake * T
            curRem -= numTake

    return curDepth

for _ in range(int(input())):
    print(go())
