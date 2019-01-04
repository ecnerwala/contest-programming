#include<bits/stdc++.h>
using namespace std;

int inv[500];

int dp[500][500];

inline int solve(int N, int K, int L, int P) {
	int Q = N / P, R = N % P;
	K -= Q;
	if(K < 0) return 0;
	if(K > R) return 0;

	for(int i = 1; i < P; i++) {
		inv[i] = 1;
		while(i * inv[i] % P != 1) inv[i] ++;
	}

	// we use K to get R
	// formula is R! / (k_p-1)! / (p-1)^(k_p-1) / ...
	dp[0][0] = 1;
	for(int i = 1; i < P; i++) {
		for(int k = 0; k <= i; k++) {
			dp[i][k] = 0;
		}
	}

	for(int l = L; l < P; l++) {
		for(int i = P-l-1; i >= 0; i--) {
			if(l == L && i) continue;
			int m = 1;
			for(int v = l; i + v < P; v += l) {
				m *= inv[v], m %= P;
				for(int k = 0; k <= i; k++) {
					dp[i + v][k + v / l] += dp[i][k] * m;
					dp[i + v][k + v / l] %= P;
				}
			}
		}
	}

	int res = dp[R][K];
	for(int i = 1; i <= R; i++) {
		res *= i, res %= P;
	}

	if(Q & 1) res *= -1;
	res %= P;
	if(res < 0) res += P;
	//cerr << N << ' ' << Q << ' ' << R << ' ' << K << ' ' << L << ' ' << P << ' ' << res << '\n';
	return res;
}

int solve2005(int N, int K, int L) {
	int v5 = solve(N, K, L, 5);
	int v401 = solve(N, K, L, 401);
	// lazy crt
	for(int i = 0; i < 2005; i++) {
		if(i % 5 == v5 && i % 401 == v401) return i;
	}
	assert(false);
	return 2005;
}

int main() {
	ios_base::sync_with_stdio(0);
	int N, K, L; cin >> N >> K >> L;
	cout << solve2005(N, K, L) << '\n';
	return 0;
}
