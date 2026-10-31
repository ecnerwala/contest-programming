#include<bits/stdc++.h>
using namespace std;int64_t N,a,b,c,i,V[1<<20],f[1<<20],R[1<<20];main(){for(cin>>N;i<N;cin>>a>>b,a+=1e9,R[i++]=(a<<32)+a+b);for(sort(R,R+N);a=i--;c=V[i]=(c+V[f[i]=a]+1)%998244353)for(;a<N&R[a]<R[i]<<32;a=f[a]);cout<<c+1;}
