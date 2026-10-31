def solve(N):
    if N == 4:
        return 45 ** 2
    elif N == 5:
        return 205 ** 2
    elif N == 6:
        return 795 ** 2
    elif N % 2 == 1:
        return (10 ** (N // 2) + 45) ** 2
    else:
        return (10 ** (N // 2 - 1) * 4 + 205) ** 2

T = int(input())
for _ in range(T):
    print(solve(int(input())))

