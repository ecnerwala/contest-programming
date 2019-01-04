N = int(input())
R = ['0', '1\n0.0']
for i in range(N):
    a = int(input())
    for _ in range(a+1): input()
    print(f"Case #{i+1}: {R[a&1]}")
