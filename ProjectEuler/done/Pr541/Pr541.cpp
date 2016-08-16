#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll P = 137;

ll small_inv[P];

// m is a power of P
ll inv(ll a, ll m) {
	assert(m >= P);
	ll res = small_inv[a % P];
	ll val = small_inv[a % P] * a % m;
	for(ll q = P; q < m; q *= P) {
		// res * a = val (mod m) = 1 (mod q)
		// now add q to make it work
		ll v = small_inv[a % P] * (P - val / q % P) % P;
		res += v * q;
		val += a % (m / q) * v % (m / q) * q % m;
	}
	return res;
}

// returns H(N) mod P, assuming it's an integer
ll H(ll N) {

	ll Q = 1, E = 0;
	while(Q <= N) Q *= P, E++;

	// val is H(N) * (Q / P) mod Q
	ll val = 0;
	for(ll n = N, m = P; m <= Q; m *= P, val *= P, n /= P) {
		// we need to compute sum of non-mult-P terms mod m
		for(ll i = 1; i <= n % m; i++) {
			if(i % P == 0) continue;
			val += inv(i, m), val %= m;
		}
	}
	// at end, val = H(N) * Q % (Q + P)
	assert(val % Q == 0);
	val /= Q;

	return val;
}

// H(a) = 0 mod p
ll dfs(ll a) {

	a *= P;

	ll res = a + (P - 1);
	// hack to avoid overflow, w/e it's big enough
	if(res >= 4577829131456417) return res;

	ll v = H(a);

	if(a && v == 0) {
		res = max(res, dfs(a));
	}

	for(ll i = 1; i < P; i++) {
		v += small_inv[i], v %= P;
		if(v == 0) {
			res = max(res, dfs(a + i));
		}
	}

	return res;
}

int main() {
	for(ll i = 1; i < P; i++) {
		for(ll j = 1; j < P; j++) {
			if(i * j % P == 1) small_inv[i] = j;
		}
	}
	cout << dfs(0) << '\n';
	return 0;
}
