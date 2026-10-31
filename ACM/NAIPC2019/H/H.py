#!/usr/bin/env python3
from math import *
from fractions import Fraction

def main():
    x, y = map(Fraction, input().split())
    v, w = map(Fraction, input().split())

    # Normalize time to w=1
    v /= w

    # Normalize length to v=1
    x /= v
    y /= v

    y = abs(y)

    if x >= 1 or (x >= 0 and x ** 2 + y ** 2 <= 2 and x ** 2 + (y-1) ** 2 >= 1):
        # Don't turn in place, make an arc, then take the tangent
        tangent = (x**2 + (y-1)**2 - 1) ** 0.5
        angle = atan2(y-1, x) + asin(1 / sqrt(x**2 + (y-1)**2))
    elif x **2 + y ** 2 <= 2:
        # Turn in place, then make an arc directly to the destination
        tangent = 0
        angle = atan2(y, x) + asin(sqrt(x ** 2 + y ** 2) / 2)
    else:
        # Turn in place, then make a 90 degree arc, then take the tangent
        tangent = sqrt(x ** 2 + y ** 2 - 1) - 1
        angle = atan2(y, x) + asin(1 / sqrt(x ** 2 + y ** 2))

    res = tangent + angle
    # Unnormalize time
    res /= w

    print('{:.20f}'.format(float(res)))

if __name__ == '__main__':
    main()
