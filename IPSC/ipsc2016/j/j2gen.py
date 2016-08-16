import sys

# this is just a standard pseudorandom generator
class PRNG:
    def __init__(self, seed): self.seed = seed

    def _random(self):
        M, A = 2147483647, 16807
        Q, R = M // A, M % A
        self.seed = A * (self.seed % Q) - R * (self.seed // Q)
        if self.seed <= 0: self.seed += M
        return self.seed

    def randint(self, lo, hi):
        return lo + self._random() % (hi-lo+1)

gen =PRNG(3958572)

def sprintfl(A):
	S =""
	for i in range(len(A)):
		if i != 0: S =S[:]+" "
		S =S[:]+str(A[i]//100)+"."+str((A[i]//10)%10)+str(A[i]%10)
	return S

def gen_rand(N,M,Qu,Qo,Qq,miv,mxv,mig,mxg,mil,mxl,mivq,mxvq,migq,mxgq,milq,mxlq,mxt):
	sys.stdout.write("\n")
	Q = Qu+Qo+Qq
	sys.stdout.write(str(N)+" "+str(M)+" "+str(Q)+"\n")
	v0 = [0 for i in range(N)]
	g0 = [0 for i in range(N)]
	for i in range(M):
		v = gen.randint(miv,mxv)
		g = gen.randint(mig,mxg)
		l = gen.randint(mil,mxl)
		v0[i] = v
		g0[i] = g
		sys.stdout.write(sprintfl([v,g,l])+"\n")
	tp = []
	t = []
	for i in range(Qu): tp.append(0)
	for i in range(Qo): tp.append(1)
	for i in range(Qq): tp.append(2)
	rq = range(Q)
	for i in rq:
		x =gen.randint(0,i)
		tp[i],tp[x] = tp[x],tp[i]
	for i in rq:
		t.append(gen.randint(1,mxt))
	t.sort()
	while True:
		ok = True
		for i in range(Q-1):
			if t[i] == t[i+1]: 
				t[i] = gen.randint(1,mxt)
				ok = False
		if ok == True: break
		t.sort()
	tps = "OUQ"
	fr = [M+i for i in range(N-M)]
	used = [i for i in range(M)]
	frs =N-M
	for i in rq:
		v = gen.randint(mivq,mxvq)
		g = gen.randint(migq,mxgq)
		l = gen.randint(milq,mxlq)
		if (frs == 0) & (tp[i] == 0): tp[i] =1
		S = tps[tp[i]]+" "+sprintfl([t[i]])
		if tp[i] == 2:
			sys.stdout.write(S+"\n")
			continue
		if tp[i] == 0:
			x = gen.randint(0,frs-1)
			fr[x],fr[frs-1] = fr[frs-1],fr[x]
			S = S[:]+" "+str(fr[frs-1]+1)
			v0[fr[frs-1]] = v
			g0[fr[frs-1]] = g
			used.append(fr[frs-1])
			frs -=1
		else:
			x = used[gen.randint(0,len(used)-1)]
			S = S[:]+" "+str(x+1)
			v = max(v,v0[x])
			g = min(g,g0[x])
			g0[x] = g
			v0[x] = v
		if tp[i] == 0: S =S[:]+" "+sprintfl([v,g,l])
		else: S =S[:]+" "+sprintfl([v,g])
		sys.stdout.write(S+"\n")

def gen_rand_converge(N,M,Qu,Qo,Qq,miv,mxv,mig,mxg,mil,mxl,mivq,mxvq,migq,mxgq,milq,mxlq,mxt):
	sys.stdout.write("\n")
	Q = Qu+Qo+Qq
	sys.stdout.write(str(N)+" "+str(M)+" "+str(Q)+"\n")
	v0 = [0 for i in range(N)]
	g0 = [0 for i in range(N)]
	for i in range(M):
		v = gen.randint(miv,mxv)
		g = gen.randint(mig,mxg)
		l = gen.randint(mil,mxl)
		v0[i] = v
		g0[i] = g
		sys.stdout.write(sprintfl([v,g,l])+"\n")
	tp = []
	t = []
	for i in range(Qu): tp.append(0)
	for i in range(Qo): tp.append(1)
	for i in range(Qq): tp.append(2)
	rq = range(Q)
	for i in rq:
		x =gen.randint(0,i)
		tp[i],tp[x] = tp[x],tp[i]
	for i in rq:
		t.append(gen.randint(1,mxt))
	t.sort()
	while True:
		ok = True
		for i in range(Q-1):
			if t[i] == t[i+1]: 
				t[i] = gen.randint(1,mxt)
				ok = False
		if ok == True: break
		t.sort()
	tps = "OUQ"
	fr = [M+i for i in range(N-M)]
	used = [i for i in range(M)]
	frs =N-M
	for i in rq:
		v = gen.randint(mivq,mxvq)
		g = gen.randint(migq,mxgq)
		l = gen.randint(milq,mxlq)
		if (frs == 0) & (tp[i] == 0): tp[i] =1
		S = tps[tp[i]]+" "+sprintfl([t[i]])
		if tp[i] == 2:
			sys.stdout.write(S+"\n")
			continue
		if tp[i] == 0:
			x = gen.randint(0,frs-1)
			fr[x],fr[frs-1] = fr[frs-1],fr[x]
			S = S[:]+" "+str(fr[frs-1]+1)
			v0[fr[frs-1]] = v
			g0[fr[frs-1]] = g
			used.append(fr[frs-1])
			frs -=1
		else:
			x = used[gen.randint(0,len(used)-1)]
			S = S[:]+" "+str(x+1)
			v = gen.randint(v0[x],mxvq)
			g = gen.randint(1,g0[x])
			g0[x] = g
			v0[x] = v
		if tp[i] == 0: S =S[:]+" "+sprintfl([v,g,l])
		else: S =S[:]+" "+sprintfl([v,g])
		sys.stdout.write(S+"\n")


sys.stdout = f = open("j2.in", 'w', 100000000)

sys.stdout.write("100\n")

gen_rand(1,1,						1000,0,0,	 				1,10**6,1,10**6,1,10**6,1,10**6,1,10**6,1,10**6,10**6)
gen_rand(2,1,						1000,0,10000, 				1,10**6,1,10**6,1,10**6,1,10**6,1,10**6,1,10**6,10**6)
gen_rand(2,2,						0,0,10000, 					1,10**6,1,10**6,1,10**6,1,10**6,1,10**6,1,10**6,10**6)
gen_rand(2*10**5,10**5,				2*10**5,5*10**4,10**5,		1,10**7,1,10**7,1,10**7,1,10**7,1,10**7,1,10**7,10**7)
gen_rand_converge(2*10**5,10**5,	2*10**5,5*10**4,10**5,		1,10**7,1,10**7,1,10**7,1,10**7,1,10**7,1,10**7,10**7)

gen_rand(2*10**6,10**6,				2*10**6,10**6,2*10**6,		1,10**9,1,10**9,1,10**9,1,10**9,1,10**9,1,10**9,10**9)
gen_rand_converge(2*10**6,10**6, 	2*10**6,10**6,2*10**6,		1,10**9,1,10**9,1,10**9,1,10**9,1,10**9,1,10**9,10**9)
gen_rand(18*10**5,10**5,			2*10**6,10**5,25*10**5,		1,10**9,1,10**9,1,10**9,1,10**9,1,10**9,1,10**9,10**9)
gen_rand(18*10**5,10**6,			2*10**6,10**6,2*10**6,		1,10**9,1,10**7,1,10**9,1,10**9,1,10**7,1,10**9,10**9)
gen_rand(18*10**5,15*10**5,			2*10**6,5*10**5,25*10**5,	1,10**7,1,10**9,1,10**9,1,10**7,1,10**9,1,10**9,10**9)
gen_rand(18*10**5,15*10**5,			25*10**5,5*10**5,2*10**6,	1,10**9,1,10**9,1,10**7,1,10**9,1,10**9,1,10**7,10**9)
N = gen.randint(18*10**5,2*10**6)
gen_rand(N,N//2,						3*10**6,N//2,10**6,			1,10**9,1,10**7,1,10**7,1,10**9,1,10**7,1,10**7,10**9)
N = gen.randint(18*10**5,2*10**6)
gen_rand(N,N//2,						10**6,N//2,3*10**6,			1,10**7,1,10**9,1,10**7,1,10**7,1,10**9,1,10**7,10**9)
N =2*10**6-1023
M =2*10**6-3*10**5
gen_rand(N,N//2,						2*10**6,N-M-1,25*10**5,		1,10**7,1,10**9,1,10**9,1,10**9,1,10**9,1,10**9,10**9)
N =2*10**6-gen.randint(500,1000)
M =2*10**6-gen.randint(10**4,10**5)
gen_rand(N,N//2,						2*10**6,N-M-1,25*10**5,		1,10**9,1,10**9,1,10**9,1,10**9,1,10**7,1,10**9,10**9)

for i in range(40):
	N =gen.randint(1,2*10**4)
	M =gen.randint(1,N)
	gen_rand(N,M,gen.randint(1,10**4),gen.randint(1,10**4),gen.randint(1,10)*5000, gen.randint(1,10**6),10**6,gen.randint(1,10**6),10**6,gen.randint(1,10**6),10**6,gen.randint(1,10**6),10**6,gen.randint(1,10**6),10**6,gen.randint(1,10**6),10**6,10**8)
for i in range(15):
	N =gen.randint(1,2*10**4)
	M =gen.randint(1,N)
	gen_rand_converge(N,M,gen.randint(1,10**4),gen.randint(1,10**4),gen.randint(1,10)*5000, gen.randint(1,10**6),10**6,gen.randint(1,10**6),10**6,gen.randint(1,10**6),10**6,gen.randint(1,10**6),10**6,gen.randint(1,10**6),10**6,gen.randint(1,10**6),10**6,10**8)

for i in range(5):
	N =gen.randint(10**4,10**5)
	M =gen.randint(1,N)
	a =b =c =10**4
	d =e =f =10**6
	gen_rand(N,M,gen.randint(1,10**5),gen.randint(1,10**5),gen.randint(1,10)*5000, gen.randint(a,a*10),a*10,gen.randint(b,b*10),b*10,gen.randint(c,c*10),c*10,gen.randint(d,d*10),d*10,gen.randint(e,e*10),e*10,gen.randint(f,f*10),f*10,gen.randint(10**7,10**8))
for i in range(5):
	N =gen.randint(10**4,10**5)
	M =gen.randint(1,N)
	a =b =c =10**6
	d =e =f =10**4
	gen_rand_converge(N,M,gen.randint(1,10**5),gen.randint(1,10**5),gen.randint(1,10)*5000, gen.randint(a,a*10),a*10,gen.randint(b,b*10),b*10,gen.randint(c,c*10),c*10,gen.randint(d,d*10),d*10,gen.randint(e,e*10),e*10,gen.randint(f,f*10),f*10,gen.randint(10**7,10**8))
for i in range(5):
	N =gen.randint(10**4,10**5)
	M =gen.randint(1,N)
	a =c =d =10**8
	e =f =b =10**7
	gen_rand(N,M,gen.randint(1,10**5),gen.randint(1,10**5),gen.randint(1,10)*5000, gen.randint(a,a*10),a*10,gen.randint(b,b*10),b*10,gen.randint(c,c*10),c*10,gen.randint(d,d*10),d*10,gen.randint(e,e*10),e*10,gen.randint(f,f*10),f*10,gen.randint(10**7,10**8))
for i in range(5):
	N =gen.randint(10**4,10**5)
	M =gen.randint(1,N)
	a =f =d =10**3
	e =c =b =10**7
	gen_rand(N,M,gen.randint(1,10**5),gen.randint(1,10**5),gen.randint(1,10)*5000, gen.randint(a,a*10),a*10,gen.randint(b,b*10),b*10,gen.randint(c,c*10),c*10,gen.randint(d,d*10),d*10,gen.randint(e,e*10),e*10,gen.randint(f,f*10),f*10,gen.randint(10**7,10**8))
for i in range(5):
	N =gen.randint(10**4,10**5)
	M =gen.randint(1,N)
	e =c =d =10**5
	a =f =b =10**2
	gen_rand(N,M,gen.randint(1,10**5),gen.randint(1,10**5),gen.randint(1,10)*5000, gen.randint(a,a*10),a*10,gen.randint(b,b*10),b*10,gen.randint(c,c*10),c*10,gen.randint(d,d*10),d*10,gen.randint(e,e*10),e*10,gen.randint(f,f*10),f*10,gen.randint(10**7,10**8))
for i in range(5):
	N =gen.randint(10**4,10**5)
	M =gen.randint(1,N)
	f =b =c =10**4
	e =a =d =10**8
	gen_rand(N,M,gen.randint(1,10**5),gen.randint(1,10**5),gen.randint(1,10)*5000, gen.randint(a,a*10),a*10,gen.randint(b,b*10),b*10,gen.randint(c,c*10),c*10,gen.randint(d,d*10),d*10,gen.randint(e,e*10),e*10,gen.randint(f,f*10),f*10,gen.randint(10**7,10**8))

#f.close()
