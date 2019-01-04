#!/usr/bin/pypy

from hashlib import sha256
import os
import random
import json
import zlib
import sys

GOLF_LIMIT = 250000

def nots(val, width):
    return (1 << width)-1 if val else 0


def run(x, chalbox, width=1):
    length, gates, check = chalbox
    assert len(x) == length
    b = [i for i in x]
    for name, args in gates:
        if name == 'false':
            b.append(0)
        else:
            u1 = b[args[0][0]] ^ nots(args[0][1], width)
            u2 = b[args[1][0]] ^ nots(args[1][1], width)
            if u1 < 0 or u2 < 0:
                break
            if name == 'or':
                b.append(u1 | u2)
            elif name == 'xor':
                b.append(u1 ^ u2)
            else:
                raise ValueError
    return [b[i] ^ nots(j, width)for i, j in check]


def import_object(s):
    dec = zlib.decompressobj()
    # compression bomb is not interesting
    s1 = dec.decompress(s, max_length=2**26)
    assert len(s1) < 2**26
    return json.loads(s1)


def our_result(input_list):
    number = [sum(input_list[i*64+j] << j for j in range(64))for i in range(8)]
    A11, A12, A21, A22, B11, B12, B21, B22 = number
    C11 = A11*B11+A12*B21
    C12 = A11*B12+A12*B22
    C21 = A21*B11+A22*B21
    C22 = A21*B12+A22*B22
    return [(i >> j) & 1 for i in [C11, C12, C21, C22] for j in range(128)]


def judge(circ):
    M = 4
    input_numbers = [random.randint(0, 2**M-1) for i in range(64*8)]
    output = [our_result([(j >> i) & 1 for j in input_numbers])
              for i in range(M)]
    output_numbers = [sum(output[j][i] << j for j in range(M))
                      for i in range(len(output[0]))]
    if run(input_numbers, circ, M) != output_numbers:
        return 0
    return len(circ[1])


def proof_of_work(stream, pow_len):
    (r, w) = stream
    prefix = os.urandom(24)
    w.write(str(pow_len)+'\n')
    w.write(prefix.encode('base64')+'\n')
    s = r.readline(100).decode('base64')
    h = bytearray(sha256(prefix + s).digest())
    for i in range(pow_len//8):
        if h[i] != 0:
            return False
    return (h[pow_len//8] >> (8 - pow_len % 8)) == 0


def sync(stream):
    r, w = stream
    word = 'SSSTART'  # send me "SSSSSSSSSSTART"
    for i in word:
        while True:
            s = r.read(1)
            assert len(s) == 1
            if s == i:
                break


def main_logic(stream, pow_len, flag):
    (r, w) = stream
    # read length
    try:
        length = int(r.readline(100), 0)
    except:
        w.write('Bad_int\n')
        return
    if length < 0 or length > 2**26:
        w.write('Int_range_invalid\n')
        return
    # read proof of work
    try:
        pow_is_good = proof_of_work(stream, max(
            pow_len, long(length).bit_length()))
    except:
        w.write('Exception_in_reading_pow\n')
        return
    if not pow_is_good:
        w.write('Bad_pow\n')
        return
    # sync (the stream reading is buggy when used mixedly?)
    try:
        sync(stream)
    except:
        w.write('Sync_shortread\n')
        return
    # read payload
    try:
        payload = r.read(length)
    except:
        w.write('Exception_in_reading_pow\n')
        return
    w.write('Received_all_payload\n')
    w.write('Head:'+repr(payload[:16].encode('hex'))+'\n')
    w.write('Hash:'+repr(sha256(payload).hexdigest())+'\n')
    if len(payload) != length:
        w.write('Payload_shortread by {}/{}\n'.format(len(payload), length))
        return
    # judge
    try:
        circ = import_object(payload)
        is_good = judge(circ)
    except:
        is_good = 0
        w.write('Exception_in_check\n')
        raise
    if is_good:
        w.write('Good_input\n')
        w.write(flag[0]+'\n')
        if is_good < GOLF_LIMIT:
            w.write('Perfect_input\n')
            w.write(flag[1]+'\n')
        return payload
    else:
        w.write('Bad_input\n')


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print 'Usage: ' + sys.argv[0] + ' pow_len flag_path'
        exit(1)

    pow_len = int(sys.argv[1])
    flag_path = (sys.argv[2])

    with open(flag_path, 'r') as f:
        flag = f.read().split()

    main_logic((sys.stdin, sys.stdout), pow_len, flag)
