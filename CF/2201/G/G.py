import z3

dirs = [(a, b) for aa, ab in [(2, 3), (3, 2)] for a in (aa, -aa) for b in (ab, -ab)]

#N, BOUND = 5, 9
N, BOUND = 1001, 368615
vars = [z3.Bool(f'g_{x}_{y}') for x in range(N) for y in range(N)]
bv4s = [z3.If(v, z3.BitVecVal(1, 4), z3.BitVecVal(0, 4)) for v in vars]
s = z3.Solver()
for i in range(N*N):
    x, y = divmod(i, N)
    # get the neighbors
    neighbors = [
        bv4s[(x+dx) * N + (y+dy)] for dx, dy in dirs if 0 <= x + dx < N and 0 <= y + dy < N
    ]
    s.add(z3.Or(z3.Not(vars[i]), sum(neighbors, start=z3.BitVecVal(0, 4)) == z3.BitVecVal(2, 4)))

s.add(sum((z3.If(n, z3.BitVecVal(1, 25), z3.BitVecVal(0, 25)) for n in vars), start=z3.BitVecVal(0, 25)) >= z3.BitVecVal(BOUND, 25))

print(s.check())
m = s.model()
for x in range(N):
    for y in range(N):
        print(1 if m[vars[x*N+y]] else 0, end='')
    print()
