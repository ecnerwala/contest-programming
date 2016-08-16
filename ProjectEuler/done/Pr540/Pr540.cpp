#include<bits/stdc++.h>
using namespace std;

// we want pairs m even, n odd, m,n relatively prime, w/ m^2 + n^2 <= N
// sum_g of mu(g) * number of gm even, gn odd w/ m, n relatively prime g^2(m^2 + n^2) <= N^2

typedef long long ll;

const int MAXV = 1e8;
bool prime[MAXV];
int mobius[MAXV];

unordered_map<ll, ll> mem;
ll Q(ll N) {
	if(mem.count(N)) return mem[N];
	ll res = 0;
	for(ll i = 1; i * i <= N; i += 2) {
		ll v = (N - i * i) / 4;
		res += ll(sqrt(double(v)));
	}
	return mem[N] = res;
}

ll P(ll N) {
	ll V = 0;
	while(V * V <= N) V++;
	V--;
	assert(V * V <= N);
	assert(V < MAXV);
	for(ll g = 1; g <= V; g++) {
		prime[g] = true;
		mobius[g] = 1;
	}
	prime[1] = false;
	for(ll g = 2; g <= V; g++) {
		if(prime[g]) {
			for(ll h = g; h <= V; h += g) {
				prime[h] = false;
				mobius[h] *= (h / g % g ? -1 : 0);
			}
			prime[g] = true;
		}
	}

	ll res = 0;

	for(ll g = 1; g <= V; g += 2) {
		res += mobius[g] * Q(N / g / g);
	}
	return res;
}

int main() {
	assert(P(20) == 3);
	assert(P(1000000) == 159139);
	cout << P(3141592653589793) << '\n';
	return 0;
}
