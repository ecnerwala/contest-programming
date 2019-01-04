#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MAXV = 8e17;

vector<ll> fib;

map<ll, int> mp;

int go(ll v) {
	if(v == 0) return 0;
	//v = abs(v);
	if(mp.count(v)) return mp[v];
	if(binary_search(fib.begin(), fib.end(), v)) return mp[v] = 1;
	vector<ll>::iterator it = upper_bound(fib.begin(), fib.end(), v);
	int res = go(*it - v);
	it--;
	res = min(res, go(v - *it));
	res ++;
	//cerr << v << ' ' << res << '\n';
	return mp[v] = res;
}

void precomp() {
	for(ll a = 1, b = 1; b < MAXV; b += a, a = b - a) {
		fib.push_back(b);
	}
}

int main() {

	precomp();

	int T;
	scanf("%d", &T);

	for(int t = 0; t < T; t++) {
		ll v;
		scanf("%lld", &v);
		printf("%d\n", go(v));
	}

	return 0;
}
