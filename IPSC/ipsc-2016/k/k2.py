
# this is just a standard pseudorandom generator
class PRNG:
    def __init__(self, seed): self.seed = seed

    def _random(self):
        M, A = 2147483647, 16807
        Q, R = M // A, M % A
        self.seed = A * (self.seed % Q) - R * (self.seed // Q)
        if self.seed <= 0: self.seed += M
        return self.seed

    def randint(self, lo, hi):
        return lo + self._random() % (hi-lo)

def median(a, b, c):
    if a > b: a, b = b, a
    if a > c: a, c = c, a
    if b > c: b, c = c, b
    return b

def modify(A, x, y, magic):
    if (A[x] > A[y]) or (not magic):
        A[x], A[y] = A[y], A[x]

# this is the main function
def k2_sort(A):
    generator = PRNG(589)

    N = len(A)
    L = 1
    while (1<<L) < N: L += 1
    lo = - ( ((1<<L) - N) // 2 )
   
    for superphase in range(2):
        for phase in range(1,L+1):
            for subphase in range(phase,0,-1):
                if phase == L and subphase == L:
                    for i in range(1<<(L-1)):
                        x, y = lo+i, lo+(1<<L)-1-i
                        if (x >= 0) and (y < N):
                            modify(A, x, y, L*superphase+phase >= 2*(L-1))
                for block in range(1<<(L-subphase)):
                    xlo  = median( 0, N, lo + (block << subphase) )
                    xmed = median( 0, N, lo + (block << subphase) + (1<<(subphase-1)) )
                    xhi  = median( 0, N, lo + (block << subphase) + (1<<subphase) )
                    if xlo == xmed or xmed == xhi: continue
                    for comparison in range(521):
                        x = generator.randint(xlo,xmed)
                        y = generator.randint(xmed,xhi)
                        modify(A, x, y, L*superphase+phase >= 2*(L-1))
