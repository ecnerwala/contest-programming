data = ["HDCC0D0EH","DEA01AFD","EDFD1EFHA","CA1GD1H0C","FG0D0CCFG","DCEFA0DAG","CE00CECEG","GDE01DGCE","CEHHA01H0","DAHHDDGHA","AHE1GFGC","EAAH1GFAH","HDAH1H0FD","HDEADG0HF","A0CH0EE1F","11AC10HCC","HFC10EFHH","C0C11CH0H","E0HFGDGC0","1G1DHG10G","FA1HCFGH","GD0H1H0H1","HHEDDG0FH","DEADFCEFA","10F0HFD10F","H0HCH0A01","GGFH0H1DF","HCDEDAG1D","E0AF1GGAH","AA0010DDC"]
inp = []
outp = []
with open("large.in", "r") as f:
	n = int(f.readline())
	for i in range(n):
		a = int(f.readline())
		inp.append(a)
		# print(bin(a))
	with open("large.out", "r") as f2:
		for i in range(n):
			a = int(f2.readline())
			outp.append(a)
			print(bin(a))

print(inp)
print(outp)
print(len(data))
print(len(inp))
print(len(outp))

from itertools import permutations
for x in permutations("ABCDEFGH", 8):
	cdata = []
	for y in data:
		for i in range(8):
			y = y.replace(x[i], str(i + 2))
		cdata.append(int(y))
	tot = 0
	for num in cdata:
		tot ^= num
	if tot == outp[0]:
		print(tot)
	if tot == inp[0]:
		print(tot)