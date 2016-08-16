from scipy.optimize import linprog

N, M, K, P = list(map(int, input().split()))
P /= 1e9
V = list(map(int, input().split()))
W = []
for k in range(K):
    W.append(list(map(int, input().split())))

C = [1] + [0] * (N - 1)
A = []
B = []

for i in range(N):
    D = list(map(int, input().split()))
    for w in W:
        lhs = [0] * N
        lhs[i] = -1
        for m in range(M):
            lhs[D[m]-1] += P * w[m] / 1e3;
        A.append(lhs)
        B.append(-V[i])

def try_run(factor):
    for i in range(N * K):
        B[i] /= factor
    res = linprog(C, A_ub=A, b_ub=B)
    if res.success:
        print('%.10f' % (res.fun * factor - V[0]))
        exit(0)
    for i in range(N * K):
        B[i] *= factor

try_run(1e-3)
try_run(1)
try_run(1e3)
try_run(1e6)
try_run(1e-6)
try_run(1e9)
try_run(1e-9)
exit(1)
