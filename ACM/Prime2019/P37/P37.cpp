#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MOD = 998244353;

ll modinv(ll a, ll m) { return a == 1 ? 1 : m - modinv(m % a, a) * m / a; }

ll p4(int n) {
	ll ans = 1;
	for (int i = 0; i < n; i++) ans = ans * 4 % MOD;
	return ans;
}

// returns (2n+1)! / n! / n!
ll c(int n) {
	ll numer = 1;
	ll denom = 1;
	for (int i = 1; i <= n; i++) numer *= 2*(2*i+1), numer %= MOD, denom *= i, denom %= MOD;
	return numer * modinv(denom, MOD) % MOD;
}

ll go(int N, int M) {
	if (N < 1) return 0;
	if (M && N < 3) return 0;

	ll ans;
	if (M == 0) {
		ans = -2 * p4(N-1) - 10 * c(N-1) + 7 * c(N) - c(N+1);
	} else if (M == 1) {
		ans = -6 * p4(N-1) + 8 * c(N-2) - 4 * c(N-1) + 90 * c(N) - 118 * c(N+1) + 44 * c(N+2) - 5 * c(N+3);
	} else if (M == 2) {
		ans = -118 * p4(N-2) + 14 * c(N-2) - 102 * c(N-1) + 140 * c(N) - 467 * c(N+1) + 854 * c(N+2) - 550 * c(N+3) + 143 * c(N+4) - 13 * c(N+5);
	} else if (M == 3) {
		ans = -452 * p4(N-2) - 16 * c(N-3) + 64 * c(N-2) - 286 * c(N-1) + 96 * c(N) + 1368 * c(N+1) - 834 * c(N+2) - 2661 * c(N+3) + 3488 * c(N+4) - 1618 * c(N+5) + 330 * c(N+6) - 25 * c(N+7);
	} else assert(false);

	ans %= MOD;
	if (ans < 0) ans += MOD;
	if (ans & 1) ans += MOD;
	ans /= 2;

	return ans;
}

int main() { int N, M; scanf("%d%d",&N,&M); printf("%d\n", int(go(N,M))); }
