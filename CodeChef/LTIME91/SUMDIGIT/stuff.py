def digit_sum(a):
    r = 0
    while a:
        r += a % 10
        a //= 10
    return r

def check_case(inp, out):
    try:
        x,y,z,p = map(int, inp.split())
        ans,a,b,c = map(int, out.split())
        assert a > 0 and b > 0 and c > 0
        assert digit_sum(a+b) == x, a+b
        assert digit_sum(b+c) == y, b+c
        assert digit_sum(c+a) == z, c+a
        assert digit_sum(a+b+c) == ans
        if p == 1: assert ans <= 10
    except Exception:
        print("inp:", inp)
        print("out:", out)
        print(x,y,z,p)
        print(ans,a,b,c)
        print(a+b, b+c, c+a)
        raise

with open('test-100.in') as inp:
    inp_lines = inp.read().strip().split('\n')[1:]

with open('test-100.res') as out:
    out_lines = out.read().strip().split('\n')

for inp, out in zip(inp_lines, out_lines):
    check_case(inp, out)
