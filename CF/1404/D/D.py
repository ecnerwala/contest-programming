from itertools import *

def brute_force(N):
    print(f"brute_force({N=})")
    def p2_turn(pairs):
        return any(sum(v) % (2*N) == 0 for v in product(*pairs))

    if N % 2 == 0:
        pairs = tuple((i, i+N) for i in range(1, N+1))
        assert not p2_turn(pairs)
        print(pairs)
        return
    else:
        return

    printed = False
    def dfs(rem, pairs = ()):
        nonlocal printed
        if not rem:
            if not p2_turn(pairs):
                if not printed:
                    print(pairs)
                    #printed = True
            return

        a = rem.pop()
        for i, b in enumerate(rem):
            dfs(rem[:i] + rem[i+1:], pairs + ((a, b),))

    dfs(list(range(1, 2*N+1)))
    print("DFS done")

for N in range(1,15):
    brute_force(N)
