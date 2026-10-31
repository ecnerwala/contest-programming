#include<iostream>
int64_t N,P,a,c[1<<20],i,j;std::string S;main(){std::cin>>N>>P>>S;if(10%P)for(*c=j=1;N--;i+=(S[N]-48)*(j=j*10%P),a+=c[i%=P]++);else for(;i<N;a+=(S[i++]-8)%P?0:i);std::cout<<a;}
