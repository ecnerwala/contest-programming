from functools import cache

@cache
def can_win_min(s: str):
    assert s
    if len(s) == 1:
        return s == '1'
    return any(can_win_max(s[:i] + min(s[i], s[i+1]) + s[i+2:]) for i in range(len(s) - 1))

@cache
def can_win_max(s: str):
    assert s
    if len(s) == 1:
        return s == '1'
    return any(can_win_min(s[:i] + max(s[i], s[i+1]) + s[i+2:]) for i in range(len(s) - 1))


for i in range(2, 65536):
    s = bin(i)
    assert s.startswith("0b1")
    s = s.removeprefix("0b1")
    if not can_win_min(s):
        print(s)

# for all odd strings of length >= 5, it's the max
# for odd strings of length 3, it's the max of the first and last
# for even strings of length >= 8, it's the 2nd max
