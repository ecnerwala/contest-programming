N,P=map(int,input().split());S,a,i,j,c=input(),0,0,1,[1]+[0]*P
if 10%P:
	for v in S[::-1]:i,j=(i+int(v)*j)%P,j*10%P;a+=c[i];c[i]+=1
else:
	for v in S:
		i+=1
		if int(v)%P<1:a+=i
print(a)
