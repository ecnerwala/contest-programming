#include<iostream>
int a[3],C[9],i;main(){using namespace std;typedef string s;s S,T;cin>>S;for(char&c:S)++a[c=c>'M'?2:c>'L'];for(T=s(a[0],0)+s(a[1],1)+s(a[2],2);i<size(S);i++)++C[S[i]*3+T[i]];cout<<max(C[1],C[3])+max(C[5],C[7])+min(C[6],C[2]);}
