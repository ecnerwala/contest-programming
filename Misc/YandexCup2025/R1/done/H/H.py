def fact(n):
    return n * fact(n-1) if n else 1

def catalan(n):
    assert n % 2 == 0
    return fact(n) // fact(n//2) // fact(n//2 + 1)
print(2 * 10**12)
print(catalan(2))
print(catalan(4))
print(catalan(20)**3, catalan(20))
print(catalan(70))
