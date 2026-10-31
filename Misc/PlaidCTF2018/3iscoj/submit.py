#!/usr/bin/env python2

from pwn import *
import zlib

from hashlib import sha256
import os
import json

with open('prog.txt', 'r') as f:
    prog = f.read()
    print(len(prog))
    json.loads(prog)
    dat = zlib.compress(prog)
    print(len(dat))

conn = remote('3isc-oj.chal.pwning.xxx', 58405)
#conn = process(["./oj.py", "8", "flag.txt"])

conn.sendline(str(len(dat)))

pow_len = int(conn.recvline())
prefix = conn.recvline().decode('base64')
while True:
    s = os.urandom(24)
    h = bytearray(sha256(prefix + s).digest())
    good = True
    for i in range(pow_len//8):
        if h[i] != 0:
            good = False
    if (h[pow_len//8] >> (8 - pow_len % 8)) != 0:
        good = False
    if good: break

print pow_len, prefix.encode('base64'), s.encode('base64'), sha256(prefix+s).hexdigest()

conn.send(s.encode('base64'))

conn.send('SSSSSSSSSSSTART')

print len(dat)
print repr(dat[:16].encode('hex'))
conn.send(dat)
conn.shutdown('send')


print conn.recvall()
