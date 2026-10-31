from functools import *
from collections import *
from itertools import *
from math import *
from sys import exit
from dataclasses import dataclass
import re
from builtins import pow
from heapq import heappush, heappop, heappushpop, heapify, heapreplace
import pyperclip


def cprint(a):
    print(a)
    pyperclip.copy(a)


def lmap(f, n):
    return list(map(f, n))


def parse_BITS(inp):
    inp = bin(int(inp, 16) + 16**len(inp))[3:]

    idx = 0

    def get_bits(l):
        nonlocal idx
        assert idx + l <= len(inp)
        r = inp[idx:idx+l]
        idx += l
        return r

    def get_int(l):
        return int(get_bits(l), 2)

    def parse_data():
        ver = get_int(3)

        t = get_int(3)
        if t == 4:
            resp = ''
            while True:
                a = get_int(1)
                b = get_bits(4)
                resp += b
                if a == 0:
                    break
                elif a == 1:
                    continue
                else:
                    assert False
            return (ver, t, int(resp, 2))
        else:
            ch = []
            lt = get_int(1)
            if lt == 0:
                l = get_int(15)
                nonlocal idx
                end_idx = idx + l
                while idx < end_idx:
                    ch.append(parse_data())
            elif lt == 1:
                c = get_int(11)
                for _ in range(c):
                    ch.append(parse_data())
            else:
                assert False
            return (ver, t, ch)

    resp = parse_data()
    assert all(v == '0' for v in inp[idx:])
    return resp


def eval_BITS(parse_tree):
    ver, t, contents = parse_tree
    if t == 4:
        assert isinstance(contents, int)
        return contents

    ch = [eval_BITS(c) for c in contents]

    if t == 0:
        return sum(ch)
    elif t == 1:
        return prod(ch)
    elif t == 2:
        return min(ch)
    elif t == 3:
        return max(ch)
    elif t == 5:
        a, b = ch
        return 1 if a > b else 0
    elif t == 6:
        a, b = ch
        return 1 if a < b else 0
    elif t == 7:
        a, b = ch
        return 1 if a == b else 0
    else:
        assert False


def main(inp, is_real):
    inp = inp.strip()
    parse = parse_BITS(inp)

    print(parse)

    def tot_ver(parse_tree):
        ver, t, contents = parse_tree
        ans = ver
        if t == 4:
            assert isinstance(contents, int)
        else:
            for c in contents:
                ans += tot_ver(c)
        return ans

    cprint(tot_ver(parse))

    cprint(eval_BITS(parse))


samp_inp = r"""
D2FE28
"""

real_inp = r"""
6051639005B56008C1D9BB3CC9DAD5BE97A4A9104700AE76E672DC95AAE91425EF6AD8BA5591C00F92073004AC0171007E0BC248BE0008645982B1CA680A7A0CC60096802723C94C265E5B9699E7E94D6070C016958F99AC015100760B45884600087C6E88B091C014959C83E740440209FC89C2896A50765A59CE299F3640D300827902547661964D2239180393AF92A8B28F4401BCC8ED52C01591D7E9D2591D7E9D273005A5D127C99802C095B044D5A19A73DC0E9C553004F000DE953588129E372008F2C0169FDB44FA6C9219803E00085C378891F00010E8FF1AE398803D1BE25C743005A6477801F59CC4FA1F3989F420C0149ED9CF006A000084C5386D1F4401F87310E313804D33B4095AFBED32ABF2CA28007DC9D3D713300524BCA940097CA8A4AF9F4C00F9B6D00088654867A7BC8BCA4829402F9D6895B2E4DF7E373189D9BE6BF86B200B7E3C68021331CD4AE6639A974232008E663C3FE00A4E0949124ED69087A848002749002151561F45B3007218C7A8FE600FC228D50B8C01097EEDD7001CF9DE5C0E62DEB089805330ED30CD3C0D3A3F367A40147E8023221F221531C9681100C717002100B36002A19809D15003900892601F950073630024805F400150D400A70028C00F5002C00252600698400A700326C0E44590039687B313BF669F35C9EF974396EF0A647533F2011B340151007637C46860200D43085712A7E4FE60086003E5234B5A56129C91FC93F1802F12EC01292BD754BCED27B92BD754BCED27B100264C4C40109D578CA600AC9AB5802B238E67495391D5CFC402E8B325C1E86F266F250B77ECC600BE006EE00085C7E8DF044001088E31420BCB08A003A72BF87D7A36C994CE76545030047801539F649BF4DEA52CBCA00B4EF3DE9B9CFEE379F14608
"""

print("Sample:")
main(samp_inp, False)

print("Actual:")
main(real_inp, True)
