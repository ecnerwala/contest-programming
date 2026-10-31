import random
import time

while True:
	with open("Kgen.in", "w") as f:
		T = random.randint(1, 1000)
		f.write(f"{T}\n")
		N = random.randint(1, 20)
		for x in range(T):
			A = []
			B = []
			a = 1
			b = 30
			f.write(f"{N}\n")
			for x in range(N):
				A.append(random.randint(a, b))
				B.append(random.randint(a, b))
			for c in A:
				f.write(f"{c} ")
			f.write("\n")
			for c in B:
				f.write(f"{c} ")
			f.write("\n")
			print(N, A, B)
	import subprocess
	subprocess.check_call("./K < Kgen.in > help.out", shell=True)
	time.sleep(0.01)
