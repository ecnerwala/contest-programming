def to_bytes(s):
    r = bytearray()
    for v in s.split():
        r.append(int(v, 16))
    return bytes(r)

def dump_string(fname, s):
    with open(fname, 'bw') as f:
        f.write(to_bytes(s))

dump_string(
"s1.in", 
"""
0A 00 00 00 01 00 00 00 02 00 00 00 03 00 00 00
04 00 00 00 05 00 00 00 0A 00 00 00 09 00 00 00
08 00 00 00 00 00 00 00 0C 00 00 00
"""
)

dump_string(
"s1.out", 
"""
0F 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 06 00 00 00 07 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 09 00 00 00
"""
)

dump_string(
"s2.in",
"""
05 00 00 00 05 00 00 00 04 00 00 00 03 00 00 00
02 00 00 00 01 00 00 00
"""
)

dump_string(
        "s2.out", "FF FF FF FF"
        )
