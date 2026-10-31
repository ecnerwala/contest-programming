import subprocess

def test(n):
    print("testing", n)
    resp = subprocess.check_output(['./K'], input=str(n), text=True).strip()
    path = [tuple(map(int, l.split())) for l in resp.split('\n')]
    assert len(path) == 2*n*n-4, len(path)
    assert sorted(path) == sorted([(x, y, z) for x in range(1, n+1) for y in range(1, n+1) for z in [0,1] if (x,y) != (1,1) and (x,y) != (n,n)])
    for a, b in zip(path, path[1:] + path[:1]):
        assert len(a) == 3 and len(b) == 3
        x = abs(a[0] - b[0])
        y = abs(a[1] - b[1])
        z = abs(a[2] - b[2])
        assert (x, y, z) == (2,1,0) or (x,y,z) == (1,2,0) or (x,y,z) == (0,0,1), f'{a} {b}'

for n in range(4, 102, 2):
    test(n)
