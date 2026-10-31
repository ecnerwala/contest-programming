def enc(a, b):
    return ''.join(chr((ord(x) - ord('a') + ord(y) - ord('a')) % 26 + ord('a')) for x, y in zip(a, b))

s = input()
if len(s) == 7:
    print(enc(s, 'vampire'))
elif len(s) == 6:
    print('none')
elif len(s) == 5:
    print(enc(s, 'demon'))
elif len(s) == 4:
    print('none')
elif len(s) == 3:
    print(enc(s, 'one'))
else:
    assert False
