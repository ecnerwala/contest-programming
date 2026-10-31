from functools import *

@lru_cache(maxsize=None)
def wins(cnt, ncnt, wants=(True, True)):
    if cnt < 0 or ncnt < 0: return True # previous player made an invalid move

    if cnt == 0 and ncnt == 0:
        return not wants[0]

    return not wins(cnt-1, ncnt, (wants[1], not wants[0])) or not wins(cnt, ncnt-1, (wants[1], wants[0]))

for i in range(1, 10, 2):
    for j in range(10):
        print(i, j, "01"[wins(i, j)])
