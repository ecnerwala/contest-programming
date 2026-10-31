#include<iostream>
#include<cstring>
#include<string>
#include<algorithm>
#include<cmath>
using namespace std;
#define D(x)
#define PD 4320
#define DIE (PD*2)
int main(){
        int n;
        long double dt;
        cin>>n;
        long double a[n*2];
        string name;
        for(int i=0;i<n;++i){
                cin>>name>>a[i];
                a[i]=fmod(a[i]*12,PD);
        }
        cin>>dt;
        sort(a,a+n);
        for(int i=0;i<n;++i)
                a[i+n]=a[i]+PD;
        long double best=DIE;
        int bound=1000;
        if(n>=bound) {
                cout<<"not possible"<<endl;
                return 0;
        }
        for(long double*b=a;b<a+n;++b){
#define GT(x) ((x)*(x)>=dt*dt)
#define FAR(i,j) (GT(b[j]-b[i])?3:0)
#define OK(i,j) (GT(b[i]+PD-b[j])?wrap[i]:0)
                {
                        bool two=false;
                        for(int i=1;i<n;++i)
                                if(!FAR(i-1,i)){
                                        two=true;
                                        break;
                                }
                        if(!two){
                                best=b[n-1]-b[0]+dt;
                                continue;
                        }
                }
                D(
                        for(int i=0;i<n;++i)
                                cout<<b[i]<<",";
                        cout<<endl;
                )
                char reach[n][n],wrap[n];
                for(int i=0;i<n;++i)
                        for(int j=0;j<n;reach[i][j++]=0);
                for(int i=0;i<n;++i)
                        D(cout<<i<<"=>"<<(int))(wrap[i]=(GT(b[0]+PD-b[i])?2:0)|(GT(b[1]+PD-b[i])?1:0))D(<<endl);
                reach[0][1]=1;
                reach[1][0]=2;
                for(int i=0;i<n-1;++i)
                        for(int j=0;j<i;++j){
                                reach[i+1][i]|=FAR(j,i+1)&OK(i+1,i)&reach[j][i];
                                reach[i][i+1]|=FAR(j,i+1)&OK(i,i+1)&reach[i][j];
                                reach[i+1][j]|=FAR(i,i+1)&OK(i+1,j)&reach[i][j];
                                reach[j][i+1]|=FAR(i,i+1)&OK(j,i+1)&reach[j][i];
                        }
                for(int i=0;i<n;++i)
                        for(int j=0;j<2;++j){
                                if(reach[n-1][i]&(1<<j)){
                                        best=min(best,b[i]+dt+PD-b[j]);
                                        //cout<<"from "<<j<<" to "<<i<<"+n"<<endl;
                                        //cout<<b[i]+dt+PD-b[j]<<"="<<b[i]<<"+"<<dt<<"+"<<PD<<"-"<<b[j]<<endl;
                                }
                                if(reach[i][n-1]&(1<<j)){
                                        best=min(best,b[n-1]+dt+PD-b[j]);
                                        //cout<<"from "<<j<<" to "<<n-1<<"+n"<<endl;
                                        //cout<<b[n-1]+dt+PD-b[j]<<"="<<b[n-1]<<"+"<<dt<<"+"<<PD<<"-"<<b[j]<<endl;
                                }
                        }
        }
        if(best<DIE)
                cout<<ceil(best)<<endl;
        else
                cout<<"not possible"<<endl;
        return 0;
}
