
def is_prime(p):
    if p <= 1: return False
    i = 2
    while i * i <= p:
        if p % i == 0: return False
        i += 1
    return True

def brute_force(N):
    primes = [v for v in range(N+1) if is_prime(v)]
    wins = []
    for i in range(N):
        wins.append(any(not wins[i-p] for p in primes if p <= i))
    losing = []
    for i in range(N):
        if not wins[i]:
            losing.append(i)
    for i in range(len(losing)):
        print(losing[i], losing[i] - losing[i-1])

brute_force(10000)
