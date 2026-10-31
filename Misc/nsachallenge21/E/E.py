import sys
import os
from collections import *
from itertools import *
from functools import *
from math import *
from builtins import pow
from binascii import *
import string

def ROTR32(a, d):
    return ((a << (32-d)) | a >> d) & ((1 << 32) - 1)
def ROTL32(a, d):
    return ((a >> (32-d)) | a << d) & ((1 << 32) - 1)

def key_hash(K):
    def n(a):
        return ((1 << 32) - 1) ^ a
    assert len(K) == 16
    v1, v2, v3, v4 = [int.from_bytes(K[i*4:(i+1)*4], byteorder='big') for i in range(4)]
    a = ROTR32(v4, 14)
    b = ROTL32(n(v2), 12)
    c = ROTL32(v2, 3)
    d = ROTR32(v3, 6)
    e = ROTL32(v1, 12) & ROTR32(v2, 3)
    f = ROTR32(c ^ d, 20)
    g = ROTL32(d, 6)
    O = v1 & f
    i = (v1 & n(v2)) ^ v3
    j = ROTR32(O, 13)
    u = n(g ^ n(e))
    l = ROTL32(d, 18) ^ ROTR32(j, 12)
    p = n(ROTL32(l, 30)) & b
    T = b & ROTR32(l, 2)
    U = v1 & n(ROTR32(ROTL32(c, 13) ^ ROTR32(d, 19), 1))
    t = a ^ n(i)
    return ':'.join(map(str, [O,U,T,p,u,t]))


known_keys = """
3c3102c78e3a3ed31656d8e3c9ac1f3b

c3540552a86a0db0128a3f378f05e83a

22858a33d4b403687f28254d3785e09d

107aabf691f48debc53e88ca5a4c884d

813c893aa5e3c52dc64e3dc5493abb16

7fdd4c7a8fc01a09e0e7c1bdd43b1328

30d8b441f9af30c0734833af36cb74ae

05b115935984da0430baf5b9c3ae2b6a

3ad8d3e573276aefd1b837946ee694d2

a58126faef3ba174987694c3b6184b20

5d7c57a158e737e587c4c20a8b154e0c

0ae45a528a51d725bc7a11b22378a48a

e028fc7de6accc28adb69301d827960d

82ea7a8ec082d18a489313a5033620b6

222a1c92c80082e22f5ec31c671eaa43

ce0ba6bff6263f28ed0c34dd364fe206

689b4d4700ccee9d5d4b706d4ca561ec

70763051505a3ec3a1b03ba52a5034e0

5f1e1e731bc174dcc5c22580b5915b4c

8d0fdb518e924d1c028430b82c5a0063

7f66ab1948c42b4676d4a1d958b0a5b3

560f62c0d48765f14e43d5fb9d096232

b0f84325c00718c472996358dd0f3b4f

557fdc037cbfda3b2b3d0adb916e1db2

999c184e59fe8d1f66f260da14ff6bf0

d3d89edb212c332fdb1d6ce85ec9263f

34bec5e9bdc3872e96c9cff3c1a5d59a

de999e3b47a3bb6ccd4852bef6a647e0

f9d7e90352956071d4453f6b7d5ca38c

91acdb8b283d12cc86f06994db7f2797

8a1358de28b3c8525de63bc488af5214

d9c00ba45e567fadeae00017506667e2

c89632547d5cb8bf7d3dea561b001ad0

c7dccf991e019a69aa6e6f019b00e9c5

547e72961be9908c2b466d21ceb968b2
""".split()

key_dict = {key_hash(bytes.fromhex(k)): k for k in known_keys}

known_hashes = """
 540541445:2428518688:33628526:2356318865:4070121552:1352772538

1275659304:2181210775:2416013469:202113024:4149014589:2464322186

886310936:1258898530:67593217:4200071938:4029137660:598092508

270864:3365015620:874776608:5242890:505212503:1761639401

441995429:545293120:2303721669:67592:1570535065:1977017073

2050:2196402828:1078399330:2459976341:1217616773:937216646""".split()
for hsh in known_hashes:
    pass
    #print(key_dict[hsh])

for l in sys.stdin.read().split():
    print(key_hash(bytes.fromhex(l)))
