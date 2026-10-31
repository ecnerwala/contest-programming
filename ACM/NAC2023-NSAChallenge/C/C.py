p, g, y, C, q = (int(input()) for _ in range(5))

h = pow(g, C, p)
good = (pow(g, q, p) * pow(y, h, p)) % p == C
print("VALID" if good else "INVALID")
