#include<iostream>
main(){int64_t N,A,B;std::cin>>N>>A>>B;B+=A;std::cout<<N/B*A+(N%B<A?N%B:A);}
