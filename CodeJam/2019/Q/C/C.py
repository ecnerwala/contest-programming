from math import gcd

# floor sqrt
# taken from https://stackoverflow.com/questions/15390807/integer-square-root-in-python
def isqrt(n):
    x = n
    y = (x + 1) // 2
    while y < x:
        x = y
        y = (x + n // x) // 2
    if x * x == n: return x
    return None

T = int(input())
for case_num in range(1,T+1):
    N, L = map(int, input().split())
    A = list(map(int, input().split()))

    assert len(A) == L

    vals = [None for _ in range(L+1)]
    for i in range(L):
        r = isqrt(A[i])
        if r is not None:
            vals[i+1] = r
            break
    else:
        for i in range(L-1):
            g = gcd(A[i], A[i+1])
            assert g > 1
            if g < A[i]:
                # it's the prime
                vals[i+1] = g
                break
        else: assert False

    for j in range(i, -1, -1):
        vals[j] = A[j] // vals[j+1]
    for j in range(i+2, L+1):
        vals[j] = A[j-1] // vals[j-1]

    primes = {v:chr(ord('A')+i) for i,v in enumerate(sorted(set(vals)))}
    assert len(primes) == 26

    ans = "".join(primes[v] for v in vals)

    print("Case #{case_num}: {ans}".format(case_num=case_num, ans=ans))
