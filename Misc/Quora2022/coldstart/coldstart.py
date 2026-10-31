import sys
from math import log, exp
from collections import Counter
import numpy as np
import scipy.sparse.linalg

def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

def main():
    k, t, n, m, u = map(int, input().split())

    x_train = []
    for _ in range(n):
        x_train.append([tuple(map(int, v.split(':'))) for v in input().split()])
    assert all(len(v) == k for v in x_train)

    y_train = []
    for _ in range(n):
        y_train.append(int(input()))

    x_test = []
    for _ in range(m):
        x_test.append([tuple(map(int, v.split(':'))) for v in input().split()])
    assert all(len(v) == k for v in x_test)

    person_counts = Counter(x for r in x_train for x, a in r)
    person_yes_counts = Counter(x for r in x_train for x, a in r if a == 1)
    test_person_counts = Counter(x for r in x_test for x, a in r)
    test_person_yes_counts = Counter(x for r in x_test for x, a in r if a == 1)
    fracs = {}
    for i in range(u):
        assert (person_counts[i] > 0) == (test_person_counts[i] > 0)
        if not person_counts[i]:
            continue
        #eprint(person_counts[i], person_yes_counts[i], test_person_counts[i], test_person_yes_counts[i])
        fracs[i] = person_yes_counts[i] / person_counts[i]
        eprint(person_yes_counts[i] / person_counts[i], test_person_yes_counts[i] / test_person_counts[i])
    for i, v in enumerate(sorted(fracs.values())):
        eprint(i / len(fracs), v)

    A = np.zeros([n, u+1])
    for i in range(n):
        A[i, u] = 1
        for x, a in x_train[i]:
            A[i, x] = a - fracs[x]
    B = np.array(y_train)

    weights = scipy.sparse.linalg.lsqr(A, B)[0]
    #eprint(weights)
    
    def eval(x):
        return weights[u] + sum((a - fracs[x]) * weights[x] for x, a in x)

    for i in range(n):
        y_ours = eval(x_train[i])
        #eprint(y_ours, y_train[i])

    for x in x_test:
        print(int(eval(x)))

if __name__ == '__main__':
    main()
