def dsum(a):
    return sum(map(int, str(a)))

a = int(''.join(('0' + str(5**i)) for i in range(80)))
vals = [dsum(a * 2**i) for i in range(51)]
for i in range(51):
    print(i, vals[i])
assert sorted(vals, reverse=True) == vals
print(len(str(a)))
print(a)
