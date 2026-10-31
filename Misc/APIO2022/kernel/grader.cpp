#include "mars.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;


int main() {
	int t;
	scanf("%d",&t);
	while(t--)
	{
		int n;
		assert(scanf("%d",&n)==1);
		vector<vector<long long> > cur,nxt;
		cur=vector<vector<long long> >(2*n+1,vector<long long>(2*n+1));
		for(int i=0;i<2*n+1;i++)
		{
			for(int j=0;j<2*n+1;j++)
			{
				char x;
				assert(scanf(" %c",&x)==1);
				cur[i][j]=x-'0';
			}
		}
		for(int k=0;k<n;k++)
		{
			for(int i=0;i<=2*(n-k-1);i++)
			{
				for(int j=0;j<=2*(n-k-1);j++)
				{
					vector<vector<long long> > v(3,vector<long long>(3));
					for(int x=0;x<3;x++)
						for(int y=0;y<3;y++)
							v[x][y]=cur[i+x][j+y];
					cur[i][j]=process(v,i,j,k,n);
				}
			}
		}
		printf("%lld\n",cur[0][0]);
	}
}
