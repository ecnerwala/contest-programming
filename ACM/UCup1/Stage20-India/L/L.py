
mem = {}
# True for Sprague win
def solve(turn, vals):
    turn %= 3
    global mem
    vals = tuple(sorted(i for i in vals if i))
    key = (turn, vals)
    if key in mem:
        return mem[key]

    if not vals:
        if turn == 1:
            return True
        else:
            return False

    for i in range(len(vals)):
        for j in range(vals[i]):
            res = solve(turn+1, vals[:i] + (j,) + vals[i+1:])
            if (res if turn == 0 else not res):
                return res

    return False if turn == 0 else True

for a in range(20):
    for b in range(20):
        if solve(0, (a, b)):
            print(a, b)

for a in range(20):
    for b in range(20):
        for c in range(20):
            if solve(0, (a, b, c)):
                print(a, b, c)

for a in range(20):
    for b in range(20):
        for c in range(20):
            for d in range(20):
                if solve(0, (a, b, c, d)):
                    print(a, b, c, d)
