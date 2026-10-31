N = 600000

lastAns = 0
with open("sbig.in", 'w') as fin:
    with open("sbig.out", 'w') as fout:
        print(N, file=fin)
        for i in range(N):
            print(chr(ord('a') + (-lastAns) % 26), (2**30-1) ^ (lastAns % (2**30)), file=fin)
            lastAns = (i+1) * (i+2) // 2 * (2**30-1)
            print(lastAns, file=fout)
