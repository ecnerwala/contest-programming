
a = list(map(int, input()))
b = list(map(int, input()))

# We want as many carries as possible
# We need 1 pair to sum to 10 and as many summing to 9 as possible
cnt_a = [0 for _ in range(10)]
cnt_b = [0 for _ in range(10)]

for c in a:
    cnt_a[c] += 1

for c in b:
    cnt_b[c] += 1

best = (0, a, b)
# Just brute force over the a which is in the 10
for sa in range(10):
    if cnt_a[sa] == 0:
        continue
    sb = 10 - sa
    while sb < 10 and cnt_b[sb] == 0:
        sb += 1
    if sb == 10:
        continue

    cur_a = cnt_a[:]
    cur_b = cnt_b[:]

    cur_a[sa] -= 1
    cur_b[sb] -= 1

    val = 1
    ra = [sa]
    rb = [sb]

    for va in range(10):
        vb = 9 - va
        for z in range(cur_a[va]):
            while vb < 10 and cur_b[vb] == 0:
                vb += 1
            if vb == 10:
                break
            cur_a[va] -= 1
            cur_b[vb] -= 1
            ra.append(va)
            rb.append(vb)
            val += 1

    if sum(cur_a) == 0:
        val += cur_b[9]

    if sum(cur_b) == 0:
        val += cur_a[9]

    for va in range(9, -1, -1):
        for z in range(cur_a[va]):
            ra.append(va)

    for vb in range(9, -1, -1):
        for z in range(cur_b[vb]):
            rb.append(vb)

    if val > best[0]:
        best = (val, ra, rb)

_, ra, rb = best
ra.reverse()
rb.reverse()
print(*ra, sep='')
print(*rb, sep='')
