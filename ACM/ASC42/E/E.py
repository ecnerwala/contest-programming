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

for i in range(1,19):
    A[i] = B[i-1];

    C[i] = 0;
    for d in range(1, i+1):
        if (i % d == 0): C[i] += A[d] * d;

    B[i] = 0;
    for k in range(i):
        B[i] += B[k] * C[i-k];
    assert(B[i] % i == 0);
    B[i] //= i;

    print(i, A[i])

print(A)
