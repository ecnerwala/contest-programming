from functools import reduce

def do_knots(lst, lens):
    lst = list(lst)

    cur_pos = 0
    skip_size = 0

    for l in lens:
        if cur_pos > 0:
            lst = lst[cur_pos:] + lst[:cur_pos]
        if l > 0:
            lst = lst[l-1::-1] + lst[l:]
        if cur_pos > 0:
            lst = lst[-cur_pos:] + lst[:-cur_pos]
        cur_pos += l + skip_size
        cur_pos %= len(lst)
        skip_size += 1

    return lst

def sparse_hash(s):
    lens = [ord(c) for c in s] + [17,31,73,47,23]
    return do_knots(list(range(256)), lens * 64)

def dense_hash(s):
    s = sparse_hash(s)
    return [reduce((lambda x,y: x ^ y), s[i*16:i*16+16]) for i in range(16)]

def knot_hash(s):
    return ''.join('{:02x}'.format(i) for i in dense_hash(s))

def knot_hash_bin(s):
    return list(map(int, ''.join('{:08b}'.format(i) for i in dense_hash(s))))
