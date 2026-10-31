
dist = {}
dist[(0, 0, 0, 0)] = 0
can_vis = {(0, 0, 0, 0)}

for i in range(1, 10):
    n_can_vis = set()
    for p in can_vis:
        for d in range(4):
            n_can_vis.add(p[:d] + (p[d] + i,) + p[d+1:])
            n_can_vis.add(p[:d] + (p[d] - i,) + p[d+1:])
    for n in n_can_vis:
        if n not in dist:
            dist[n] = i
    can_vis = n_can_vis

    all_poss = 0
    tot_sum = i * (i+1) // 2
    for a in range(0,tot_sum+1):
        for b in range(a, tot_sum-a+1):
            for c in range(b, tot_sum-a-b+1):
                for d in range(c, tot_sum-a-b-c+1):
                    if a+b+c+d <= tot_sum and (a+b+c+d)%2 == tot_sum % 2:
                        if (a,b,c,d) not in can_vis:
                            print("bad", i, (a, b, c, d), tot_sum - a - b - c - d)
