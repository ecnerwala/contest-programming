can = {}
def dfs(a, b, depth=0):
    v = a + a + b
    if v > 500: return
    can[v] = min(can.get(v, depth), depth)
    dfs(a, a + b, depth+1)
    dfs(a + b, 2 * a + b, depth+1)
dfs(1, 0)

print(can)
print(set(range(1, 501)) - can)
