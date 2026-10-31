import random
import time

while True:
	with open("Agen.in", "w") as f:
		T = random.randint(1, 1000)
		f.write(f"{T}\n")
		for i in range(T):
			l = random.randint(1, 10 ** random.randint(1, 9))
			r = random.randint(1, 10 ** random.randint(1, 9))
			f.write(f"{l} {r}\n")
		print(T)
	import subprocess
	subprocess.check_call("./A < Agen.in > help.out", shell=True)
	time.sleep(0.01)
