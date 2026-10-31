#include<bits/stdc++.h>
using namespace std;int f,Q,T;char S[1<<20],*x=S+(1<<18),*y;main(){for(cin>>x>>Q,y=strlen(x)+x;Q--&&cin>>T;)if(T&1)f^=1;else cin>>T,cin>>*(T-1^f?y++:--x);reverse(x,f?y:x);cout<<x;}
