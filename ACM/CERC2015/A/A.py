R = 7
C = 5

SYMBOLS = "0123456789+"
SYMBOL_ROWS = (lambda s: tuple(s.strip().split()))("""
xxxxx
x...x
x...x
x...x
x...x
x...x
xxxxx
....x
....x
....x
....x
....x
....x
....x
xxxxx
....x
....x
xxxxx
x....
x....
xxxxx
xxxxx
....x
....x
xxxxx
....x
....x
xxxxx
x...x
x...x
x...x
xxxxx
....x
....x
....x
xxxxx
x....
x....
xxxxx
....x
....x
xxxxx
xxxxx
x....
x....
xxxxx
x...x
x...x
xxxxx
xxxxx
....x
....x
....x
....x
....x
....x
xxxxx
x...x
x...x
xxxxx
x...x
x...x
xxxxx
xxxxx
x...x
x...x
xxxxx
....x
....x
xxxxx
.....
..x..
..x..
xxxxx
..x..
..x..
.....
""")

SYMBOL_STRINGS = {c: tuple(SYMBOL_ROWS[R*i:R*(i+1)]) for i,c in enumerate(SYMBOLS)}
SYMBOL_TABLE = {tuple(zip(*v)): c for c,v in SYMBOL_STRINGS.items()}

inp_cols = tuple(zip(*(input() for _ in range(7))))
inp_string = ''.join(SYMBOL_TABLE[inp_cols[i:i+C]] for i in range(0, len(inp_cols), C+1))
res = str(eval(inp_string))
print('\n'.join('.'.join(r) for r in zip(*(SYMBOL_STRINGS[c] for c in res))))
