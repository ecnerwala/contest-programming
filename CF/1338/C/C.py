s = {0}
for _ in range(100):
    a = 0
    while a in s: a += 1
    b = 0
    while b in s or a^b in s: b += 1
    c = a^b
    print(bin(a),bin(b),bin(c))
    s.add(a)
    s.add(b)
    s.add(c)
