import sys
import os
from collections import *
from itertools import *
from functools import *
from math import *
from builtins import pow
from binascii import *
import string

def F(x, ka, kb, delta):
    def n(x):
        return ((1 << 32) - 1) ^ x
    return ((n(x) + delta) ^ ((x >> 3) + ka) ^ ((x << 13) + kb)) & ((1 << 32) - 1)

assert F(0x504c4521, 0x4558414d, 0x504c4520, 0xbb40e64d) == 0xfd650dfa
assert F(0x01020304, 0x01020304, 0x01020304, 0x01020304) == 0xbfbf3f9f
assert F(0xabcdef01, 0xff33eedd, 0xdeadbeef, 0x23456789) == 0xff570ad5

def split_int32(a):
    assert len(a) % 4 == 0
    return [int.from_bytes(a[i*4:(i+1)*4], byteorder='big') for i in range(len(a) // 4)]

def join_int32(a):
    return b''.join(v.to_bytes(4, byteorder='big') for v in a)

def encrypt_block(data, K):
    assert len(data) == 8
    assert len(K) == 16
    k0, k1, k2, k3 = split_int32(K)
    L, R = split_int32(data)
    Delta = 0
    delta = 0xBB40E64D
    for _ in range(64):
        Delta += delta
        Delta &= (1 << 32) - 1
        L ^= F(R, k0, k1, Delta)
        R ^= F(L, k2, k3, Delta)
    return join_int32([R, L])

def decrypt_block(data, K):
    assert len(data) == 8
    assert len(K) == 16
    k0, k1, k2, k3 = split_int32(K)
    L, R = split_int32(data)
    Delta = 0xD0399340
    delta = 0xBB40E64D
    for _ in range(64):
        L ^= F(R, k2, k3, Delta)
        R ^= F(L, k0, k1, Delta)
        Delta -= delta
        Delta &= (1 << 32) - 1
    return join_int32([R, L])

op = input()
key = bytes.fromhex(input())
data = bytes.fromhex(input())
f = {
    'ENCRYPT': encrypt_block,
    'DECRYPT': decrypt_block,
}[op]
print(f(data, key).hex())
