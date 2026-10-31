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

keys = [bytes.fromhex(v) for v in """
057efa81168fe970
0cd4f32b1de5e21a
07ebf81418fce703
035ffca01460eb9f
0943f6bc1a54e5ab
07eaf81518fbe704
08a0f75f19b1e64e
0ba9f4561cbae345
0ffff0001000efff
00a5ff5a11b6ee49
0e75f18a1f86e079
0c80f37f1d91e26e
0049ffb6115aeea5
0ad5f52a1be6e419
0e1ef1e11f2fe0d0
0123fedc1234edcb
0f00f0ff1011efee
0875f78a1986e679
019afe6512abed54
0b36f4c91c47e3b8
043efbc1154feab0
0d8df2721e9ee161
0449fbb6155aeaa5
0feff01010f0ef0f
""".split()]

for k in keys:
    k = [int(a, 16) for a in k.hex()]
    for i in range(4):
        assert k[i] + k[4+i] == 15
        assert k[i+8] + k[12+i] == 15
        assert (k[i+8] - k[i]) & 15 == 1

def gen_keys():
    for a in range(16):
        for b in range(16):
            for c in range(16):
                s = [0,a,b,c]
                s = s + [15-x for x in s] + [x+1 for x in s] + [14-x for x in s]
                yield b''.join(hex(x & 15)[2:].encode() for x in s)

def xo(a, b):
    return bytes(i^j for i,j in zip(a,b))
keys = list(gen_keys())
assert b'043efbc1154feab0' in keys
plain_text = b'tlv_format_6'
key_map = {xo(plain_text, my_stream(k, 12)): k for k in keys}

for l in sys.stdin.read().split():
    print(key_map[bytes.fromhex(l)[:12]].decode())
