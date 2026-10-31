def f(n,s):
 d=[-n,-n];d[s]=0
 for i in range(y//g):d=[max(d[0],d[1]),d[0]+n*g//y+(i*x%y<n*g%y)]
 return d[s]
import math;n,x,y=map(int,input().split());g,h=math.gcd(x,y),lambda n:max(f(n,0),f(n,1));y+=x;print(n%g*h(n//g+1)+(g-n%g)*h(n//g))
