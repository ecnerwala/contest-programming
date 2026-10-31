def f(t,d=''):global x,y;t=int(t);x-=c[t]>3;y-=c[t]//2;c[t]+=int(d+'1');x+=c[t]>3;y+=c[t]//2
_,a,_,*o=open(0);x,y,*c=[0]*9**6;[*map(f,a.split())]
for q in o:f(q[2:],q[0]);print("YNEOS"[x<1 or y<4::2])
