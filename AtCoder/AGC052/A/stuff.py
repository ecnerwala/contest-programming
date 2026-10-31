from itertools import *

def is_subsequence(a, b):
    try:
        b = iter(b)
        for c in a:
            while next(b) != c:
                pass
        return True
    except StopIteration:
        return False

N = 5

strings = list(permutations([0]*N + [1]*N))
strings = [a+a for a in strings]
print(len(strings))
#print(strings)

for s in [[0] * N + [1] * N + [0]]:
    cnt = 0
    for w in strings:
        cnt += is_subsequence(s, w)
    if cnt >= len(strings) * 0.7:
        print(s, cnt)
