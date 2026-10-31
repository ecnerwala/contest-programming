N=int(input());R,d,f,s,x=sorted(list(map(int,input().split()))for i in range(N))+[(2e9,0)],[0]*N+[1],[0]*N,1,N
for i in range(N-1,-1,-1):
	while R[x][0]<sum(R[i]):x=f[x]
	d[i]=s=(s+d[x])%998244353;f[i],x=x,i
print(s)
