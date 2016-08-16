#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll s[int(2e8)];

ll S(ll n) {
	memset(s, 0, sizeof(s));
	ll res = 0;
	for(ll p = 2; p <= n; p++) {
		if(!s[p]) {
			// p is prime
			ll v = 0;
			for(ll e = 1, q = p; q <= n; e++, q *= p) {
				while(e > 0) {
					v += p;
					for(ll t = v; t % p == 0; t /= p) e--;
				}
				for(ll j = q; j <= n; j += q) {
					s[j] = max(s[j], v);
				}
			}
		}
		res += s[p];
	}
	return res;
}

int main() {
	assert(S(100) == 2012);
	cout << S(ll(1e8)) << '\n';
	return 0;
}
