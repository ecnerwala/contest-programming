import sys
import os
from collections import *
from itertools import *
from functools import *
from math import *
from builtins import pow
from binascii import *
import string
import subprocess

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

def x(a, b):
    return bytes(i^j for i,j in zip(a,b))

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

def pkcs7_pad(dat):
    l = 8 - (len(dat) % 8)
    return dat + bytes([l] * l)
def pkcs7_unpad(dat):
    l = dat[-1]
    assert 1 <= l <= 8
    return dat[:-l]

def encrypt_ecb(dat, K):
    dat = pkcs7_pad(dat)
    r = b''
    for i in range(0, len(dat), 8):
        r += encrypt_block(dat[i:i+8], K)
    return r
def decrypt_ecb(dat, K):
    assert len(dat) % 8 == 0
    r = b''
    for i in range(0, len(dat), 8):
        r += decrypt_block(dat[i:i+8], K)
    return pkcs7_unpad(r)

def encrypt_ctr(dat, K, N):
    dat = pkcs7_pad(dat)
    r = b''
    for i in range(0, len(dat), 8):
        r += x(encrypt_block((int.from_bytes(N, byteorder='big') ^ (i//8)).to_bytes(8, byteorder='big'), K), dat[i:i+8])
    return r
def decrypt_ctr(dat, K, N):
    assert len(dat) % 8 == 0
    r = b''
    for i in range(0, len(dat), 8):
        r += x(encrypt_block((int.from_bytes(N, byteorder='big') ^ (i//8)).to_bytes(8, byteorder='big'), K), dat[i:i+8])
    return pkcs7_unpad(r)

def encrypt_cbc(dat, K, IV):
    dat = pkcs7_pad(dat)
    r = b''
    for i in range(0, len(dat), 8):
        IV = encrypt_block(x(dat[i:i+8], IV), K)
        r += IV
    return r
def decrypt_cbc(dat, K, IV):
    assert len(dat) % 8 == 0
    r = b''
    for i in range(0, len(dat), 8):
        r += x(decrypt_block(dat[i:i+8], K), dat[i-8:i] if i else IV)
    return pkcs7_unpad(r)

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
#key_map = {xo(plain_text, my_stream(k, 12)): k for k in keys}

def parse_tlv(dat):
    cur_idx = 0
    assert dat[cur_idx:cur_idx+12] == b'tlv_format_6'
    cur_idx += 12

    tags = {
        0x01: ("VICTIM_ID", True),
        0x02: ("VERSION", True),
        0x06: ("FILENAME", True),
        0x08: ("IV", False),
        0x09: ("NONCE", False),
        0x13: ("KEY_HASH", True),
        0x27: ("ENC_DATA", False),
    }

    resp = []
    while cur_idx < len(dat):
        tag = dat[cur_idx]
        cur_idx += 1
        tag_name, is_string = tags.get(tag, ("UNKNOWN", False))
        l = 0
        while True:
            v = dat[cur_idx]
            cur_idx += 1
            l <<= 7
            l |= v & 127
            if v & 128:
                continue
            else:
                break
        v = dat[cur_idx:cur_idx+l]
        cur_idx += l
        v_string = v.decode() if is_string else v.hex()
        resp.append((tag_name, v_string))
    return resp

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

key_dict = {key_hash(bytes.fromhex(k)): bytes.fromhex(k) for k in known_keys}

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

if False:
    for fname in ['f1.jpg', 'f2.jpg', 'f3.jpg', 'f4.jpg']:
        if False:
            with open(fname, 'rb') as f:
                dat = f.read().split(b'LOCKED_DATA:')[1].decode().strip()
            print(dat[:12])
            output = subprocess.check_output('./A', input='DECODE\n' + dat, text=True)
            output = bytes.fromhex(output)
            assert output.startswith(b'STREAM_CIPHER:')
            output = output[len(b'STREAM_CIPHER:'):]
            print(output[:12])
            head = xo(plain_text, output[:12])
            k = next(k for k in gen_keys() if my_stream(k, 12) == head)
            print(k)
            output = xo(output, my_stream(k, len(output)))
            with open(fname+'.txt', 'w') as f:
                f.write(output.hex())
        with open(fname+'.txt', 'r') as f:
            output = bytes.fromhex(f.read())
        tlv = parse_tlv(output)
        for a,b in tlv:
            print(a, b[:20])
        tlv = {k: v for k, v in tlv}
        key_hash = tlv['KEY_HASH']
        version = tlv['VERSION']
        enc_data = bytes.fromhex(tlv['ENC_DATA'])
        key = key_dict[key_hash]
        print(key.hex())
        if version == '1.0':
            dec = decrypt_ecb(enc_data, key)
        elif version == '1.2':
            dec = decrypt_ctr(enc_data, key, bytes.fromhex(tlv['NONCE']))
        elif version == '2.0':
            dec = decrypt_cbc(enc_data, key, bytes.fromhex(tlv['IV']))
        else: assert False
        with open(tlv['FILENAME'], 'wb') as f:
            f.write(dec)

K1 = '13155588'
K2 = 'BDCAF0FF'
K3 = 'C92F6F3E'
K4 = 'AA54F565'

K = K1 + K2 + K3 + K4
a = bytes.fromhex(input())
print(decrypt_cbc(a, bytes.fromhex(K), bytes.fromhex('A64FCAD1B76F93E9')).decode())
