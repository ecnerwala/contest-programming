#include<iostream>
int T,N,K;main(){for(std::cin>>T;T--;std::cout<<(K>N?K-N:K+N&1)<<' ')std::cin>>N>>K;}
