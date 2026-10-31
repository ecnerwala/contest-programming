import random
import time

while True:
	with open("Mgen.in", "w") as f:
		T = 1
		f.write(f"{T}\n")
		for t in range(T):
			N = random.randint(1, 8)
			f.write(f"{N}\n")
			res = [[0 for i in range(2*N)] for j in range(2)]
			for i in range(N):
				for v in [1, 2]:
					l1 = random.randint(-3, 3)
					if l1 >= 0:
						res[l1 % 2][2*i + l1 // 2] = v
			for r in res:
				f.write(''.join([str(x) for x in r]) + '\n')
		print(T)
	import subprocess
	subprocess.check_call("./M < Mgen.in > help.out", shell=True)
	time.sleep(0.01)
