h = 10**9
t = 0
for i in range(1, 100000000000):
    v = min(i, (h + i - 1) // i)
    t += v
    h -= i * v
    if h < 0:
        break
print(t)
