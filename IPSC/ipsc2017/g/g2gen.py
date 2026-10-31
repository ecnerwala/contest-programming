#!/usr/bin/env python2

import sys

FILENAME = 'g2.in'

class PRNG:
    def __init__(self, seed): self.seed = seed

    def _random(self):
        M, A = 2147483647, 16807
        Q, R = M // A, M % A
        self.seed = A * (self.seed % Q) - R * (self.seed // Q)
        if self.seed <= 0: self.seed += M
        return self.seed

    def random(self):
        return self._random() * 1.0 / 2147483647

    def randrange(self, n):
        return self._random() % n

    def randint(self, start, end):
        res = start + int(self._random() % (end - start + 1))
        assert start <= res <= end, "%d %d %d" % (start, res, end)
        return res

    def choice(self, seq):
        return seq[self.randint(0, len(seq) - 1)]

    def shuffle(self, data):
        n = len(data)
        for i in xrange(n):
            j = self.randint(i, n-1)
            data[i], data[j] = data[j], data[i]

class Checksum:
    def __init__(self): self.chk = 47

    def add(self, *xs):
        for x in xs:
            assert isinstance(x, (int, long))
            self.chk = (42 * self.chk + x) % 123455678901

    def check(self, expected, f):
        if self.chk != expected:
            f.truncate(0)
            raise RuntimeError("Internal error, something went wrong. Expected checksum %d got %d" % (expected, self.chk) )

random = PRNG(4747)
chk = Checksum()


sys.stdout = fh = open(FILENAME, 'w')

N = 5000000
Q = [35251638-1]
chk.add(N, Q[0])
print N, Q[0]
P = range(N)
random.shuffle(P)
def write(t, a, b):
    a = (-1 if a == -1 else P[a])
    b = (-1 if b == -1 else P[b])
    chk.add(t, a+1, b+1)
    print t, a+1, b+1
    Q[0] -= 1
S = []
M = N - 1010
O = range(M)
random.shuffle(O)
for i in O:
    write(1, i, (i+1)%M)
for i in xrange(33000000):
    if random.randrange(10):
        k = random.randrange(9 if i%1000000 < 100000 else 10)
        if k <= 3:
            a = random.randrange(N/2)
            write(2, a, (a + N/2 + random.randrange(10000)) % N)
        elif k <= 4:
            write(2, random.randrange(N), random.randrange(N))
        elif k <= 6:
            a = random.randrange(N)
            b = random.choice([(a+N+random.randint(-2000,2000))%N, -1, random.randrange(N)])
            write(1, a, b)
            S.append(a)
        elif S:
            a = random.choice(S)
            S.remove(a)
            write(1, a, (a+1)%M)
    else:
        p, l = random.choice([[M, 10], [M+10, 1000]])
        t = random.randrange(2)
        u = random.randrange(l+1)
        if t and u==l: continue
        write(t+1, p+random.randrange(l), -1 if u==l else p+u)

if Q[0]: chk.chk = -47
chk.check(84228534246, fh)
