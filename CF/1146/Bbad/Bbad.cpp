#include<bits/stdc++.h>
using namespace std;

using ll = long long;
int main() {
	ios_base::sync_with_stdio(0); cin.tie(0);
	string s, t;
	ll n;
	ll a[99999],f=0,x=0;
	memset(a,0,sizeof a);
	cin >> s;
	n=s.size();
	for(int i = 0; i < n; i++)
	{
		if (s[i] != 'a')
		{
			t += s[i];
			a[t.size() - 1] = i;
			f=1;
		}
		else
			x = i+1;
	}
	if (f == 1)
	{
		n = t.size();
		if (a[n/2]>=x)
			if (t.substr(0,n/2) == t.substr(n/2, n-n/2)) {
				cout << s.substr(0, a[n/2]);
				return 0;
			}
		cout << ":(";
	}
	else
		cout << s;

	return 0;
}
