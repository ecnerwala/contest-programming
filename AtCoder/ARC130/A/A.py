
N = int(input())
S = input()
last = ''
cnt = 0
ans = 0
for c in S:
    if c != last:
        last = c
        cnt = 0
    ans += cnt
    cnt += 1
print(ans)
