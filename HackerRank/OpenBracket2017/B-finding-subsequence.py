#!/bin/python3

import sys

def solve(s, k):
    r = ''
    for c in reversed('abcdefghijklmnopqrstuvwxyz'):
        cnt = s.count(c)
        if cnt < k:
            s = ''.join(v for v in s if v != c)
        else:
            r += c * cnt
            for i in range(cnt):
                s = s[s.index(c)+1:]
    return r

if __name__ == "__main__":
    s = input().strip()
    k = int(input().strip())
    result = solve(s, k)
    print(result)
