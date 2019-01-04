#!/usr/bin/python2

from hashlib import sha512
import json
import sys


def verify(x, chalbox):
    length, gates, check = chalbox
    b = [(x >> i) & 1 for i in range(length)]
    for name, args in gates:
        if name == 'false':
            b.append(0)
        else:
            u1 = b[args[0][0]] ^ args[0][1]
            u2 = b[args[1][0]] ^ args[1][1]
            if name == 'or':
                b.append(u1 | u2)
            elif name == 'xor':
                b.append(u1 ^ u2)
    return b[check[0]] ^ check[1]


def test(chalbox):
    length, gates, check = chalbox

    gates = [['input', []] for _ in range(length)] + gates

    x = (16284297358290970909) | (17635442568080165447 << 64)

    b = []
    for i, (name, args) in enumerate(gates):
        if name == 'input':
            b.append(x & 1)
            x >>= 1
        elif name == 'false':
            b.append(0)
        else:
            u1 = b[args[0][0]] ^ args[0][1]
            u2 = b[args[1][0]] ^ args[1][1]
            if name == 'or':
                b.append(u1 | u2)
            elif name == 'xor':
                b.append(u1 ^ u2)
        if i >= 33767:
            for arg, v in args:
                if arg < 33767:
                    if arg == 128:
                        print arg, 1-b[arg], 1-int(v)
                    else:
                        print arg, b[arg], int(v)
    return b[check[0]] ^ check[1]


def pretty_print(chalbox):
    length, gates, check = chalbox

    gates = [['input', []] for _ in range(length)] + gates

    uses = [[] for _ in range(len(gates))]
    for i, (name, args) in enumerate(gates):
        if name in ['or', 'xor']:
            for arg in [args[0][0], args[1][0]]:
                uses[arg].append(i)
    uses[check[0]].append(-1)

    for i, (name, args) in enumerate(gates):
        line = "{:5d}: ".format(i)
        if name == 'false':
            line += 'F'
        elif name == 'input':
            line += '?'
        else:
            line += "{}{:5d} {} {}{:5d}".format(' !'[args[0][1]], args[0][0], {'or':'|', 'xor': '^'}[name], ' !'[args[1][1]], args[1][0])
        line += '   (' + ', '.join(str(u) for u in uses[i]) + ')'
        print line


def print_graph(chalbox):
    length, gates, check = chalbox

    gates = [['input', []] for _ in range(length)] + gates

    print("digraph map3 {")
    for i, (name, args) in enumerate(gates):
        if name in ['or', 'xor']:
            for arg in [args[0][0], args[1][0]]:
                print '  {} -> {};'.format(arg, i)
    print '  {} -> {};'.format(check[0], -1)
    print("}")


def dec(x, w):
    z = int(sha512(str(int(x))).hexdigest(), 16)
    return '{:x}'.format(w ^ z).decode('hex')


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print 'Usage: ' + sys.argv[0] + ' key chalbox.txt'
        exit(1)

    with open(sys.argv[2], 'r') as f:
        cipher, chalbox = json.loads(f.read())

    #pretty_print(chalbox)
    #print_graph(chalbox)
    print test(chalbox)

    #key = int(sys.argv[1]) % (1 << chalbox[0])

    key = (16284297358290970909) | (17635442568080165447 << 64)

    if verify(key, chalbox):
        print 'The flag is:', dec(key, cipher)
    else:
        print 'Wrong key.'
