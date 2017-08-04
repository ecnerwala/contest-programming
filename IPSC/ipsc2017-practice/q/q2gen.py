import sys

class PRNG:
    def __init__(self, seed): self.seed = seed

    def _random(self):
        M, A = 2147483647, 16807
        Q, R = M // A, M % A
        self.seed = A * (self.seed % Q) - R * (self.seed // Q)
        if self.seed <= 0: self.seed += M
        return self.seed

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

    def sample(self, data, k):
        data = list(data)
        while len(data) > k:
            i = self.randint(0, len(data) - 1)
            del data[i]
        return self.shuffle(data)

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

def gen(n, e):
    print
    print n, e
    for i in range(e):
        who = random.randint(1,n)
        chk.add(who)
        print who

CHKSUM = 76298422865
FILENAME = 'q2.in'

random = PRNG(47*47)
chk = Checksum()

def hard():
    print 100
    for a in range(1,11):
        for b in range(1,11):
            gen(10**4 * a, 10**4 * b)

if __name__ == '__main__':
    sys.stdout = f = open(FILENAME, 'w')
    hard()
    f.close()
    chk.check(CHKSUM, FILENAME)
