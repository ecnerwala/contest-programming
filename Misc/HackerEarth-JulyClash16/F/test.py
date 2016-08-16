from random import *

cnt = 0
for i in range(10000):
    v = randint(0, 1000000000)
    w = randint(0, 200)
    for m in range(w):
        v = randint(0, v)
    if v:
        cnt += 1

print(cnt)
