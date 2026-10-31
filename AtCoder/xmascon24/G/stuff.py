from functools import cache

def mex(s):
    s = set(s)
    res = 0
    while res in s:
        res += 1
    return res

@cache
def f(n):
    if n <= 0:
        return -1
    return mex(f(n - i) for i in range(1, (1 if n % 17 == 0 else 2) + 1))


def my_f(x):
    if x < 34:
        return (x + 2) % 3
    x %= 34
    side = x // 17
    x %= 17
    if not side:
        return x % 3
    else:
        v = x % 3
        if v:
            return 3 - v
        else:
            return v


for i in range(1, 200):
    print(i, f(i), my_f(i))
