def eprint(*args, **kwargs):
    import sys
    print(*args,**kwargs, file=sys.stderr, flush=True)

n = int(input())

def read_score():
    problems = input().split(',')
    score = 0
    penalty = 0
    solveTimes = []
    eprint(problems)
    for x in problems:
        if x == '-': continue
        eprint(x)
        t,w = map(int, x.split())
        score += 1
        solveTimes.append(t)
        penalty += t + w * 20
    solveTimes.sort()
    solveTimes.reverse()
    return tuple([score, -penalty, *solveTimes])

def read_pang():
    problems = input().split(',')
    score = 0
    penalty = 0
    solveTimes = []
    for x in problems:
        if x == '-': continue
        eprint(x)
        t,w = map(int, x.split())
        score += 1
        solveTimes.append(t)
        penalty += w * 20
    solveTimes.sort()
    if solveTimes:
        for i in range(1, len(solveTimes)):
            solveTimes[i] += solveTimes[i-1]
    for t in solveTimes:
        penalty += t
    solveTimes.reverse()
    return tuple([score, -penalty, *solveTimes])

scores = [read_score() for _ in range(n-1)]
my_score = read_pang()
eprint(scores)
eprint(my_score)
