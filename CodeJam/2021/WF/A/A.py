import sys
from fractions import Fraction
import math

inp = lambda: map(int, sys.stdin.readline().split())
def main():
    N, W, H = inp()
    x1, y1, x2, y2 = inp()
    if x1 > x2:
        x1, y1, x2, y2 = x2, y2, x1, y1
    x0, y0 = 0, 0

    n0, n1, n2 = Fraction(0, 1), Fraction(0, 1), Fraction(0, 1)
    if x0 < x1:
        dx = x1 - x0
        dy = y1 - y0
        s = Fraction(dy, dx)
        n0 += s
        n1 -= s
    if x1 < x2:
        dx = x2 - x1
        dy = y2 - y1
        s = Fraction(dy, dx)
        n1 += s
        n2 -= s
    if x0 < x2:
        dx = x2 - x0
        dy = y2 - y0
        s = Fraction(dy, dx)
        n2 += s
        n0 -= s

    #assert x0 < x2

    #assert n0 + n1 + n2 == 0
    area = Fraction(y1 * x2 - y2 * x1, 2)

    totB = 0

    evts = []
    for _ in range(N):
        x, y, a, b = inp()
        totB += b
        a += b
        evts.append((x + x0, 0, a))
        evts.append((x + x1, 1, a))
        evts.append((x + x2, 2, a))

    target = totB * area

    evts.sort()

    cur_x = 0
    cur_slope = Fraction(0, 1)
    cur_y = Fraction(0, 1)
    cur_tot = -target

    ans = abs(cur_tot)

    for nxt_x, t, a in evts:
        #assert nxt_x >= cur_x
        if nxt_x > cur_x:
            dx = nxt_x - cur_x
            nxt_y = cur_y + dx * cur_slope
            nxt_tot = cur_tot + (cur_y + nxt_y) / 2 * dx
            ans = min(ans, abs(nxt_tot))
            mid_tot = nxt_tot
            if cur_slope != 0:
                mx = -cur_y / cur_slope
                if 0 <= mx <= dx:
                    mid_tot = cur_tot + (cur_y + 0) / 2 * mx
            ans = min(ans, abs(mid_tot))
            if min(mid_tot, cur_tot, nxt_tot) <= 0 and 0 <= max(mid_tot, cur_tot, nxt_tot):
                ans = Fraction(0, 1)
                break

            cur_x = nxt_x
            cur_y = nxt_y
            cur_tot = nxt_tot
        if t == 0:
            cur_slope += a * n0
        elif t == 1:
            cur_slope += a * n1
            if x0 == x1:
                cur_y += a * (y1 - y0)
        elif t == 2:
            cur_slope += a * n2
            if x1 == x2:
                cur_y += a * (y2 - y1)
        else:
            assert False

    #assert cur_slope == 0
    #assert cur_y == 0

    ans /= abs(area)

    return ans

T, = inp()
for case_num in range(1, T+1):
    ans = main()
    print(f"Case #{case_num}: {ans.numerator}/{ans.denominator}")
