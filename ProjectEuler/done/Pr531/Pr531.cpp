#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 2e6;

ll inv(ll a, ll m) {
	if(a == 1 || a == m - 1) return a;
	return m - inv(m % a, a) * m / a;
}

ll gcd(ll a, ll b) {
	while(a) {
		ll tmp = b % a;
		b = a;
		a = tmp;
	}
	return b;
}

ll phi[MAXN];
ll fac[MAXN]; // largest prime factor

ll g(ll a, ll n, ll b, ll m) {
	ll g = gcd(n, m);
	if(a % g != b % g) return 0;
	while(g > 1) {
		ll p = fac[g];
		ll q = 1;
		while(g % p == 0) g /= p, q *= p;
		if(n / q % p) n /= q;
		else m /= q;
	}
	a %= n, b %= m;
	return a + (b - a % m + m) % m * inv(n, m) % m * n;
}

ll f(ll n, ll m) {
	return g(phi[n], n, phi[m], m);
}

void precomp() {
	fac[1] = 1;
	for(ll i = 1; i < MAXN; i++) phi[i] = i;
	for(ll i = 2; i < MAXN; i++) {
		if(phi[i] != i) continue;
		for(ll j = i; j < MAXN; j += i) {
			fac[j] = i;
			phi[j] /= i, phi[j] *= i-1;
		}
	}
}

int main() {
	precomp();

	assert(g(2, 4, 4, 6) == 10);
	assert(g(3, 4, 4, 6) == 0);

	ll res = 0;
	for(int n = 1000000; n < 1005000; n++) {
		for(int m = n + 1; m < 1005000; m++) {
			res += f(n, m);
		}
	}
	cout << res << '\n';
	return 0;
}
