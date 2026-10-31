from heapq import *
from math import log

MOD = 998244353
def go(N, X, Y):
    N += 2
    lst = []

    cnts = {(1, 0): 1, (0, 1): 1}
    pq = [(log(X), (1, 0)), (log(Y), (0, 1))]

    heapify(pq)
    ans = 1
    while N > 0:
        v = heappop(pq)
        c = min(N, cnts[v[1]])
        N -= c
        ans = ans * pow(X, v[1][0] * c, MOD) % MOD
        ans = ans * pow(Y, v[1][1] * c, MOD) % MOD
        lst.append((v, c))
        for ov, oc in lst:
            nv, nc = (v[0] + ov[0], (v[1][0] + ov[1][0], v[1][1] + ov[1][1])), (c * oc if v[1] != ov[1] else (c * oc - c) // 2)
            if nv[1] not in cnts:
                cnts[nv[1]] = nc
                heappush(pq, nv)
            else:
                cnts[nv[1]] += nc
    print(len(lst))
    return ans

def main():
    T = int(input())
    for _ in range(T):
        N, X, Y = map(int, input().split())
        print(go(N, X, Y))

if __name__ == '__main__':
    main()
