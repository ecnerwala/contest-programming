from collections import *
from itertools import *
from functools import *
from math import *
from builtins import pow
from binascii import *
import string

dat = bytes.fromhex(input())
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
    print(tag_name+':'+v_string)
