#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const int MAXN = 1.1e6;
int N;

ll K;
ll A[MAXN];

ll E[MAXN];

const int MAXM = 15;
int M;
ll primes[MAXM];

ll dp[1<<MAXM];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	cin >> N >> K;
	for (int i = 0; i < N; i++) cin >> A[i];
	for (int i = 0; i < N; i++) cin >> E[i];

	ll g = A[0];
	for (int i = 1; i < N; i++) g = __gcd(g, A[i]);

	M = 0;
	for (ll v = g, p = 2; v > 1; p ++) {
		if (p * p > v) p = v;
		if (v % p != 0) continue;
		primes[M++] = p;
		while (v % p == 0) v /= p;
	}
	cerr << M << '\n';

	for (int i = 0; i < N; i++) {
		ll v = A[i];
		for (int j = 0; j < M; j++) {
			ll p = primes[j];
			assert(v % p == 0);
			ll q = 1;
			while (v % p == 0) { v /= p, q *= p; }
			assert(q >= p);
		}
	}

	return 0;
}
