m = 963
a = (10**53 - 1) // 963
used = [False for _ in range(1001)]
s = ""
for i in range(1, 1001):
    if used[i]:
        continue
    cur = i
    while not used[cur]:
        used[cur] = True
        cnd = str(a * cur)
        #print(cur, cnd)
        for l in range(1, len(cnd)+1):
            if s.endswith(cnd[:-l]):
                s += cnd[-l:]
                #print("adding ", l)
                break
            assert l < len(cnd)
        assert s.endswith(cnd)
        cur = cur * 10 % m
#print(len(s))
print(a)
print(s)
