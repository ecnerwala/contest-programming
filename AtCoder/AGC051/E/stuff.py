S = {0, 14, 91, 105}
while True:
    ns = set(S)
    for a in S:
        for b in S:
            if (a+b) & 1: continue
            c = (a+b)>>1
            ns.add(c)
    if ns == S: break
    S = ns

print(sorted(S))
