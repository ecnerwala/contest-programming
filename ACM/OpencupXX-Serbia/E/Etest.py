import random

while True:
	with open("Ebig.in", "w") as f:
		T = 124
		f.write("{}\n".format(T))
		for t in range(T):
			n = random.randint(1, 10**8)
			f.write("{}\n".format(n))
	import os
	assert os.system("./E < Ebig.in > E.out") == 0
	print("ok")
