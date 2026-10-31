from math import gcd

A = [0] * 61 # number of movements of a given size
B = [0] * 61 # number of collections of movemenets 
C = [0] * 61

F = [0] * 61 # number of flips of a given size
FS = [[0] * 61 for _ in range(61)] # number of flip sequences of a given length with a given number of flips

A[0] = 0;
B[0] = 1;
C[0] = 0;
F[0] = 0;
FS[0][0] = 1;

for i in range(1,61):
    F[i] = B[i-1];
    FS[i][0] = 0;
    for k in range(1,i+1):
        FS[i][k] = 0;
        for j in range(1,i+1):
            FS[i][k] += F[j] * FS[i-j][k-1];

    for k in range(1,i+1):
        cur = 0;
        for j in range(k):
            p = k // gcd(j, k);
            if i % p != 0: continue;
            cur += FS[i // p][k // p];
        assert(cur % k == 0);
        cur //= k;
        A[i] += cur;

    C[i] = 0;
    for d in range(1, i+1):
        if (i % d == 0): C[i] += A[d] * d;

    B[i] = 0;
    for k in range(i):
        B[i] += B[k] * C[i-k];
    assert(B[i] % i == 0);
    B[i] //= i;

with open("abstract.in", "r") as f:
    with open("abstract.out", "w") as g:
        for t, line in enumerate(f):
            N = int(line.rstrip())
            if N == 0: break
            g.write("Case {}: {}\n".format(t+1, A[N]))
