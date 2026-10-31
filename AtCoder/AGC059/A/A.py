from itertools import count, permutations
from collections import Counter

def solve(N):
    dists = {}
    q = []
    for c in 'ABC':
        st = c * N
        dists[st] = 0
        q.append(st)

    for z in count():
        if z == len(q):
            break
        cur = q[z]
        d = dists[cur]
        num_changes = sum(a != b for a, b in zip(cur, cur[1:]))
        if num_changes == N-1:
            #d != (num_changes + 1) // 2
            print(d, cur, sorted(Counter(cur).values()))
        for l in range(N):
            for r in range(l+1,N+1):
                for p in permutations('ABC'):
                    nxt = cur[:l] + ''.join(p[ord(c)-ord('A')] for c in cur[l:r]) + cur[r:]
                    if nxt not in dists:
                        dists[nxt] = d+1
                        q.append(nxt)

for n in range(1, 10):
    solve(n)
