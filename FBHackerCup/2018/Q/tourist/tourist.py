#!/usr/bin/env python3
NCASES = int(input())
for case_num in range(1, NCASES+1):
    N, K, V = map(int, input().split())
    V -= 1
    V *= K
    V %= N
    ans = []
    for i in range(N):
        a = input()
        if V <= i < V + K or V <= i+N < V+K:
            ans.append(a)
    res = f"Case #{case_num}: {' '.join(ans)}"
    print(res)
