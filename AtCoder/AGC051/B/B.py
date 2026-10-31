def go():
    xs = []
    for i in range(30):
        for j in range(i+1, 30):
            xs.append(2**i + 2**j)
    ds = []
    for i in range(30):
        for j in range(i+1, 30):
            ds.append(2**j - 2**i)
    return [(a,b) for a in xs for b in xs if a-b in ds]

def check(s):
    a = len(set(y for x,y in s))
    b = len(set(x-y for x,y in s))
    c = len(set(x for x,y in s))
    d = len(set(x+y for x,y in s))
    assert len(set(s)) == len(s)
    assert d >= 10 * max(a,b,c)

r = go()
check(r)
print(len(r))
for x, y in r:
    print(x,y)
