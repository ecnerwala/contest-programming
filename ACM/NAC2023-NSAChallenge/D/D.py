import sys
from collections import *

lines = sys.stdin.read().strip().split('\n')

cur_node = None
start_node = None
bug_node = None
adj = defaultdict(list)
for l in lines:
    if l[0] == 'N':
        cur_node = l.split()[1]
        if start_node is None:
            start_node = cur_node
    else:
        l = l.strip()
        if l == 'BUG':
            bug_node = cur_node
            continue
        x, y, z = l.lstrip().split()
        assert x == 'UE'
        assert y == '->'
        nxt_node = z
        assert cur_node is not None
        adj[cur_node].append(nxt_node)

par = {}
q = []
par[start_node] = None
q.append(start_node)
idx = 0
while idx < len(q):
    cur = q[idx]
    idx += 1
    for nxt in adj[cur]:
        if nxt in par:
            continue
        par[nxt] = cur
        q.append(nxt)


res = []
cur = bug_node
while cur is not None:
    res.append(cur)
    cur = par[cur]
res = res[::-1]
print('\n'.join(res))
