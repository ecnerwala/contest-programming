#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = 1e9 + 7;

const int MAXN = 600;
int N;
char grid[MAXN][MAXN];

const int MAXD = 600;
int D;
ll cnt[MAXD];

ll dp[MAXD];

int T;

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) {
		cin >> grid[i];
	}
	
	assert(grid[N / 2][N / 2] == 'O');

	D = N; // note that the max distance is the side length
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			if(grid[i][j] == 'X') {
				cnt[abs(i - N / 2) + abs(j - N / 2)] ++;
			}
		}
	}

	int T = 0;
	for(int d = D, c = 0; d >= 0; d--) {
		c += cnt[d];
		if(c) T = max(T, d + c - 1);
	}

	// number of things at dist at most D
	for(int i = 0; i < D; i++) {
		cnt[i + 1] += cnt[i];
	}

	if(T < D) {
		// max dist is less than T
		assert(cnt[D] == cnt[T]);
		D = T;
	}

	memset(dp, 0, sizeof(dp));
	dp[0] = 1;
	for(int t = 1; t <= D; t++) {
		for(int v = t; v > 0; v--) {
			dp[v] += dp[v - 1] * (cnt[t] - (v - 1));
			dp[v] %= MOD;
		}
	}

	assert(D <= T);
	assert(cnt[D] >= (T - D));
	assert(D >= (cnt[D] - (T - D)));

	ll res = dp[cnt[D] - (T - D)];
	for(ll i = T - D; i > 0; i--) {
		res *= i;
		res %= MOD;
	}
	cout << T << ' ' << res << '\n';

	return 0;
}
