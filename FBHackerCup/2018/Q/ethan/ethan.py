#!/usr/bin/env python3

def ethan(a, b):
    assert len(a) > 0
    assert len(b) > 0
    i, j = 0, 0
    while True:
        if i >= len(a):
            return True
        if j >= len(b):
            return False
        if a[i] == b[j]:
            i += 1
            j += 1
            continue
        if i == 0:
            j += 1
            continue
        i = 0
        continue

def slow(S):
    for i in range(len(S)):
        if not ethan(S, S[:i] + S):
            return S[:i] + S
    return "Impossible"

def go(S):
    BAD = "Impossible"
    loc = S.find(S[0], 1)
    if loc == -1: return BAD
    res = S[:loc] + S
    if res.startswith(S):
        return BAD
    assert not ethan(S, res)
    return res

def main():
    NCASES = int(input())

    for case_num in range(1, NCASES+1):
        S = input()
        res = go(S)
        assert go(S) == slow(S)
        print(f"Case #{case_num}: {res}")

if __name__ == '__main__':
    main()
