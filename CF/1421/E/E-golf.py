N,x,y,z,v,w=input(),-9e9,-9e9,-9e9,0,1
for A in map(int,input().split()):x,y,z,v,w=max(z+A,y-A),max(x+A,z-A),max(y+A,x-A,v-w*A),v+w*A,-w
print([v,y][N>'1'])
