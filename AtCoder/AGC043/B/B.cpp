#include<bits/stdc++.h>
int N,i,v,k=1;char c;main(){for(std::cin>>N;N--;std::cin>>c,v^=!(N&i++)*c,k&=c);std::cout<<(v&(3^k&~v));}
