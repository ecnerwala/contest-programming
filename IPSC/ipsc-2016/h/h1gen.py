#!/usr/bin/env python2

import sys

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
        data = data[:]
        n = len(data)
        for i in range(n):
            j = self.randint(i, n-1)
            data[i], data[j] = data[j], data[i]
        return data

class Checksum:
    def __init__(self): self.chk = 47

    def add(self, *xs):
        for x in xs:
            assert isinstance(x, (int, long))
            self.chk = (42 * self.chk + x) % 123455678901

    def check(self, expected, filename):
        if self.chk != expected:
            f = open(filename,'w')
            f.close() # truncate the output file to 0 bytes
            raise RuntimeError("Internal error, something went wrong. Expected checksum %d got %d" % (expected, self.chk) )

CHKSUM = 73940861784
FILENAME = 'h1.in'

random = PRNG(4747)
chk = Checksum()


TC = random.shuffle(
    [(random.randint(2, 3), 7) for i in xrange(7)] +
    [(10, random.randint(10, 40)) for i in xrange(42)] +
    [(10000 + random.randrange(10), 10000 + random.randint(-50, 50)) for i in xrange(8)] +
    [(1234567, 2345678)] * 6
) + [(10000000, 20000000)]


sys.stdout = f = open(FILENAME, 'w')

chk.add(len(TC))
print(len(TC))
for N, Q in TC:
    print
    print N, Q
    chk.add(N, Q)
    for i in xrange(N-1):
        print i+1, i+2
        chk.add(i+1, i+2)
    for i in xrange(Q):
        l = random.randint(N*2/3 if random.randint(1, 3) == 1 else 0, N-1)
        if random.randrange(2):
            a = random.randint(1 + l, N)
            b = a - l
        else:
            a = random.randint(1, N - l)
            b = a + l
        c = random.randrange(2)
        print a, b, c
        chk.add(a, b, c)

f.close()
chk.check(CHKSUM, FILENAME)
