#include<bits/stdc++.h>
using namespace std;

typedef long long ll;


ll exp(ll a, ll b, ll m) {
	a %= m;
	ll r = 1;
	while(b) {
		if(b & 1) r *= a, r %= m;
		a *= a, a %= m;
		b /= 2;
	}
	return r;
}

ll inv(ll a, ll m) {
	return a == 1 ? 1 : (m - inv(m % a, a) * m / a);
}

const int MAXN = 2e8;

ll pows[MAXN];
ll fac[MAXN];

ll F(ll N, ll M) {
	for(int i = 1; i <= N; i++) {
		pows[i] = exp(i - 1, i, M);
	}
	fac[0] = 1;
	for(int i = 1; i <= N; i++) {
		fac[i] = fac[i - 1] * i % M;
	}

	ll res = N * pows[N - 1] % M * (N - 1) % M;
	for(int i = 2; i <= N - 1; i++) {
		res += fac[N] * inv(fac[N - i], M) % M * inv(i, M) % M * pows[N - i] % M;
		res %= M;
	}
	return res;
}

int main() {
	ll MOD = 135707531;
	assert(F(3, MOD) == 6);
	assert(F(8, MOD) == 16276736);
	assert(F(100, MOD) == 84326147);
	cout << F(12344321, MOD) << '\n';
	return 0;
}

// F(N) = N * (N-2)^(N-1) + sum (i = 2..N-1) (n choose i * (i-1)! * (n-i-1)^(n-i))
