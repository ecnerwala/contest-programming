#include<bits/stdc++.h>
#include<boost/functional/hash.hpp>
using namespace std;
using namespace boost;

typedef long long ll;
unordered_map< pair<ll, pair<ll, ll>>, ll, boost::hash<pair<ll, pair<ll, ll>>> > mem[60];

inline ll digsum(ll n) {
	ll res = 0;
	for(ll j = n; j; j /= 10) res += j % 10;
	return res;
}

inline ll step(ll n) { return n + digsum(n); }

// jump
ll jump(ll n, ll i) {
	// preprocess and do mem lookup
	ll dsum = digsum(n);
	for(ll p10 = 1; p10 <= n; ) {
		dsum -= n / p10 % 10;
		p10 *= 10;
		pair<ll, pair<ll, ll>> state(n % p10, make_pair(dsum, p10));
		if(mem[i].count(state)) {
			return n / p10 * p10 + mem[i][state];
		}
	}

	ll res = n;
	if(i <= 10) {
		for(ll j = (1 << i); j; j--) {
			res = step(res);
		}
	} else {
		res = jump(res, i - 1);
		res = jump(res, i - 1);
	}

	// postprocess: find the longest equal suffix
	ll p10 = 1;
	while(n % p10 == res % p10) p10 *= 10;
	assert(p10 >= 10);
	p10 /= 10;
	assert(p10 >= 1);
	assert(n % p10 == res % p10);
	pair<ll, pair<ll, ll>> state(n % p10, make_pair(digsum(n / p10), p10));
	if(p10 >= 100000) mem[i][state] = res % p10;

	return res;
}

ll a(ll n) {
	n--;
	// start at 1, jump n times
	ll res = 1;
	for(ll i = 0; (1 << i) <= n; i++) {
		if(n & (1 << i)) res = jump(res, i);
	}
	return res;
}

int main() {
	assert(a(1e6) == 31054319);
	cerr << a(1e6) << '\n';
	cout << a(ll(1e15)) << '\n';
	return 0;
}
