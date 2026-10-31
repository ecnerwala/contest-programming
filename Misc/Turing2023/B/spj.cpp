#include "testlib.h"

const int Max_n = 1e3 + 5;

typedef long long ll;

ll a[Max_n] , b[Max_n];

int main(int argc, char* argv[])
{
	registerTestlibCmd(argc, argv);
	int n = inf.readInt();
	for(int i = 1 ; i <= n ; ++ i)
		a[i] = i;
	for(int i = 1 ; i <= n ; ++ i)
		b[i] = inf.readInt();
	int cnt = ouf.readInt();
	if(cnt > 2001)
	{
		quitf(_wa, "The number of operations is higher than 2001");
		return 0;
	}
	while(cnt --)
	{
		int opt = ouf.readInt() , val = ouf.readInt();
		if(opt == 1)
		{
			if(val < 0)
			{
				quitf(_wa, "Your first operation's val is lower than 0");				
			}
			for(int i = 1 ; i <= n ; ++ i)
				a[i] += val;
		}
		else
		{
			if(val < 1)
			{
				quitf(_wa, "Your second operation's val is lower than 1");				
			}
			for(int i = 1 ; i <= n ; ++ i)
				a[i] %= val;
		}
	}
	for(int i = 1 ; i <= n ; ++ i)
		if(a[i] != b[i])
		{
			quitf(_wa, "Wrong ans on the %d number , expect %d , your ans is %d" , i , b[i] , a[i]);
			return 0;
		}
	quitf(_ok, "Good job! You have got zzz's love.");
	return 0;
}
