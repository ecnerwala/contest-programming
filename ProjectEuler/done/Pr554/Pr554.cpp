#include<bits/stdc++.h>
using namespace std;

// case on number of block types:
// has 4: 2 * ((2n choose n) - (n^2 + 1)) - (n-1)^2
// has 3: 4 * ((2n choose n) - 2n)
// has 2 same: 4 * (n - 1)
// has 2 opposite: 2 * ((2n choose n) - 2)
// has 1: 4
// sum: 8 (2n choose n) - (3n^2 + 2n + 7)

typedef long long ll;
const ll MOD = 1e8 + 7;

ll inv(ll a, ll m = MOD) {
	return (a == 1) ? 1 : (m - inv(m % a, a) * m / a);
}

ll choose(ll n, ll r) {
	if(r > n) return 0;
	if(n >= MOD) {
		return choose(n % MOD, r % MOD) * choose(n / MOD, r / MOD) % MOD;
	}
	assert(n < MOD);
	assert(r < MOD);
	r = min(r, n - r);
	ll a = 1, b = 1;
	for(ll i = 0; i < r; i++) {
		a *= n - i, a %= MOD;
		b *= r - i, b %= MOD;
	}
	return a * inv(b) % MOD;
}

ll C(ll n) {
	ll c = choose(2 * n, n);
	c *= 8;
	c %= MOD;
	n %= MOD;
	c -= (3 * n * n + 2 * n + 7) % MOD;
	c %= MOD;
	if(c < 0) c += MOD;
	return c;
}

int main() {
	assert(C(1) == 4);
	assert(C(2) == 25);
	assert(C(10) == 1477721);
	ll res = 0;
	for(ll i = 2, a = 1, b = 1; i <= 90; i ++, b += a, a = b - a) {
		res += C(b);
		res %= MOD;
	}
	cout << res << '\n';
	return 0;
}
