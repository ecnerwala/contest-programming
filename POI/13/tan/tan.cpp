#include<bits/stdc++.h>
using namespace std;

int inv[500];
int fact[500];
int ifact[500];

int dp[500][500];

template <int P> int solve(int N, int K, int L) {
	int Q = N / P, R = N % P;
	K -= Q;
	if(K < 0) return 0;
	if(K > R) return 0;

	fact[0] = 1;
	for (int i = 1; i <= P-1; i++) fact[i] = fact[i-1] * i % P;

	ifact[P-1] = 1; while (ifact[P-1] * fact[P-1] % P != 1) ifact[P-1] ++;

	for (int i = P-1; i >= 1; i--) ifact[i-1] = ifact[i] * i % P;

	for(int i = 1; i < P; i++) {
		inv[i] = ifact[i] * fact[i-1] % P;
	}

	// we use K to get R
	// formula is R! / (k_p-1)! / (p-1)^(k_p-1) / ...
	dp[0][0] = 1;
	for(int i = 1; i <= R; i++) {
		for(int k = 0; k <= i && k <= K; k++) {
			dp[i][k] = 0;
		}
		for (int l = L; l <= i; l++) {
			int m = fact[i-1] * ifact[i-l] % P;
			for (int k = 0; k <= i-l && k <= K-1; k++) {
				dp[i][k+1] += dp[i-l][k] * m;
				dp[i][k+1] %= P;
			}
		}
	}

	int res = dp[R][K];

	if(Q & 1) res = (res ? P-res : 0);
	//cerr << N << ' ' << Q << ' ' << R << ' ' << K << ' ' << L << ' ' << P << ' ' << res << '\n';
	return res;
}

int solve2005(int N, int K, int L) {
	int v5 = solve<5>(N, K, L);
	int v401 = solve<401>(N, K, L);
	// lazy crt
	for(int i = 0; i < 2005; i++) {
		if(i % 5 == v5 && i % 401 == v401) return i;
	}
	assert(false);
	return 2005;
}

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, K, L; cin >> N >> K >> L;
	cout << solve2005(N, K, L) << '\n';
	return 0;
}
