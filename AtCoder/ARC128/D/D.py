from functools import *
from itertools import *

@cache
def can_kill(arr):
    if len(arr) == 2:
        return True
    for i in range(1, len(arr)-1):
        if arr[i] != arr[0] and arr[i] != arr[-1] and can_kill(arr[:i+1]) and can_kill(arr[i:]):
            return True
    return False

for l in range(2,7):
    for a in product(list(range(5)), repeat=l):
        if any(x == y for x,y in zip(a, a[1:])):
            continue
        if not can_kill(a):
            print(a)
