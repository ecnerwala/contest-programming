import sys
import os
from collections import *
from itertools import *
from functools import *
from math import *
from builtins import pow
from binascii import *
import string

def rc4(K, N, offset=0):
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[i] + K[i % len(K)]) & 255
        S[i], S[j] = S[j], S[i]

    i, j = 0, 0
    r = bytearray()
    for z in range(offset+N):
        i += 1
        i &= 255
        j += S[i]
        j &= 255
        S[i], S[j] = S[j], S[i]
        if z >= offset:
            r.append(S[(S[i] + S[j]) & 255])
    return bytes(r)

assert rc4(0x0102030405060708.to_bytes(8, byteorder='big'), 16, 0) == 0x97ab8a1bf0afb96132f2f67258da15a8.to_bytes(16, byteorder='big')
assert rc4(0x0102030405060708.to_bytes(8, byteorder='big'), 16, 256) == 0xd0a990ff2c05fef5b90373c9ff4b870a.to_bytes(16, byteorder='big')
assert rc4(0x0102030405060708.to_bytes(8, byteorder='big'), 16, 512) == 0x163b319923a6bdb4527c626126703c0f.to_bytes(16, byteorder='big')
assert rc4(0x4578616d706c65204b6579.to_bytes(11, byteorder='big'), 16, 0) == 0xc6d01f550a5eca49cee9ca4128969af2.to_bytes(16, byteorder='big')
assert rc4(0x4578616d706c65204b6579.to_bytes(11, byteorder='big'), 16, 256) == 0x463e0a5bff1fa8888a8451abdd75dda5.to_bytes(16, byteorder='big')
assert rc4(0x4578616d706c65204b6579.to_bytes(11, byteorder='big'), 16, 512) == 0xfb1126010ebc8e232c3c5f60aee185ee.to_bytes(16, byteorder='big')

def rc4_v1(K, N, offset=0):
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[i] + K[i % len(K)]) & 255
        S[i], S[j] = S[j], S[i]

    i, j = 0, 0
    r = bytearray()
    for z in range(offset+N):
        i += 1
        i &= 255
        j += S[i]
        j &= 255
        S[i], S[j] = S[j], S[i]
        if z >= offset:
            r.append(S[S[(S[i] + S[j]) & 255]])
    return bytes(r)

def rc4_v2(K, N, offset=0):
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[i] + K[j % len(K)]) & 255
        S[i], S[j] = S[j], S[i]

    i, j = 0, 0
    r = bytearray()
    for z in range(offset+N):
        i += 1
        i &= 255
        j += S[i]
        j &= 255
        S[i], S[j] = S[j], S[i]
        if z >= offset:
            r.append(S[(S[i] + S[j]) & 255])
    return bytes(r)

def rc4_v3(K, N, offset=0):
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[j] + K[i % len(K)]) & 255
        S[i], S[j] = S[j], S[i]

    i, j = 0, 0
    r = bytearray()
    for z in range(offset+N):
        i += 1
        i &= 255
        j += S[i]
        j &= 255
        S[i], S[j] = S[j], S[i]
        if z >= offset:
            r.append(S[(S[i] + S[j]) & 255])
    return bytes(r)

rcs = [rc4, rc4_v1, rc4_v2, rc4_v3]

def check():
    k = bytes.fromhex('e35223d1de0bfb55eb8f52fd2fc5cfda')
    assert len(k) == 16
    k1 = bytes(a ^ 0x55 for a in k)
    k2 = bytes(a ^ 0xaa for a in k)
    k3 = bytes(a ^ 0x01 for a in k)

    tgt = bytes.fromhex('ead8d509bbddc976c0c75e3fce1c48e05d9f8a82b6fb646caf962243f5a01cd4ec743f7d0a861800113c45f7259128c4c24d0e26d6cd9250fa7db1e70c9af861160da46c671642b5444b64cdb104e0b471f6d9de4458ff3795c23403040ae3f38d315543c1bfbce5a3b00bdb76a7409ac2b2f5a174c667c0ba07fc8d85706dc8')
    assert len(tgt) == 128
    for a in range(4):
        for b in range(4):
            for c in range(4):
                for d in range(4):
                    w = rcs[a](k, 128, 256)
                    x = rcs[b](k1, 128, 1437)
                    y = rcs[c](k2, 128, 512)
                    z = rcs[d](k3, 128, 1352)
                    r = bytes(m^n^o^p for m,n,o,p in zip(w,x,y,z))
                    if r == tgt:
                        return (a,b,c,d)

# variants = check()
variants = [2,3,0,1]

def my_stream(k, n, offset=0):
    assert len(k) == 16
    k1 = bytes(a ^ 0x55 for a in k)
    k2 = bytes(a ^ 0xaa for a in k)
    k3 = bytes(a ^ 0x01 for a in k)

    w = rcs[variants[0]](k, n, 256 + offset)
    x = rcs[variants[1]](k1, n, 1437 + offset)
    y = rcs[variants[2]](k2, n, 512 + offset)
    z = rcs[variants[3]](k3, n, 1352 + offset)
    r = bytes(m^n^o^p for m,n,o,p in zip(w,x,y,z))
    return r

print(my_stream(bytes.fromhex(input()), 128).hex())
