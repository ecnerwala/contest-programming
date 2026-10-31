import sys

N = list(map(int, input()))
valid = set(range(10)) - set(N)
if not valid:
    print("Impossible")
    sys.exit(0)

if valid == {0}:
    print(0)
    sys.exit(0)


# next alien
if any(i > N[0] for i in valid):
    r = [min(i for i in valid if i > N[0])]
    r += [min(valid)] * (len(N)-1)
else:
    r = [min(i for i in valid if i > 0)]
    r += [min(valid)] * len(N)

nxt_alien = int(''.join(map(str, r)))

# prv alien
if any(0 < i < N[0] for i in valid):
    r = [max(i for i in valid if i < N[0])]
    r += [max(valid)] * (len(N)-1)
else:
    r = [max(valid)] * (len(N)-1)

if not r:
    r = [0]
N = int(''.join(map(str, N)))
prv_alien = int(''.join(map(str, r)))
if N - prv_alien > nxt_alien - N:
    print(nxt_alien)
elif N - prv_alien < nxt_alien - N:
    print(prv_alien)
else:
    print(prv_alien, nxt_alien)
