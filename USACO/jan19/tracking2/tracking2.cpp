#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const ll MOD = int(1e9)+7;

const int MAXN = 1.1e5;
const int V_TOP = int(1e9);
int N, K;
int C[MAXN];
int V[MAXN];

ll dp[MAXN];
ll pref[MAXN];

ll powmod(ll a, ll b) {
	assert(b >= 0);
	ll r = 1;
	while (b) {
		if (b & 1) r *= a, r %= MOD;
		a *= a, a %= MOD;
		b /= 2;
	}
	return r;
}

// non-zero options
ll solve(ll l, ll numHigher, ll opts) {
	l -= numHigher;
	if (l <= 0) {
		return 1;
	}
	assert(l > 0);

	if (l < K) {
		if (numHigher) {
			// we have something on a side to take care of it
			return powmod(opts+1, l);
		}
		ll res = powmod(opts+1, l) - powmod(opts, l);
		res %= MOD;
		if (res < 0) res += MOD;
		return res;
	}

	ll optsK = powmod(opts, K);

	dp[0] = 1;
	pref[0] = 1;
	for (int i = 1; i <= l+1; i++) {
		//dp[i] = dp[i-1] + dp[i-2] * opts + dp[i-3] * opts * opts + ... + dp[i-K] * opts^(K-1)
		
		dp[i] = pref[i-1];
		if (i >= K+1) {
			dp[i] -= pref[i-K-1] * optsK;
			dp[i] %= MOD;
			if (dp[i] < 0) dp[i] += MOD;
		}

		pref[i] = (pref[i-1] * opts + dp[i]) % MOD;
	}

	assert(l >= K);

	return dp[l+1];
}

int main() {
	if(fopen("tracking2.in", "r")) {
		auto res = freopen("tracking2.in", "r", stdin);
		assert(res);
		res = freopen("tracking2.out", "w", stdout);
		assert(res);
	}
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K;
	for (int i = 0; i <= N-K; i++) {
		cin >> C[i];
	}

	assert(K >= 1);

	// sliding window maximum
	deque<pair<int, int>> q;
	for (int i = 0; i < N; i++) {
		if (i <= N-K) {
			// append the new constraint
			while (!q.empty() && q.back().first <= C[i]) {
				q.pop_back();
			}
			q.emplace_back(C[i], i);
		}
		while (!q.empty() && q.front().second <= i-K) {
			q.pop_front();
		}
		assert(!q.empty());
		V[i] = q.front().first;
	}

	//for (int i = 0; i < N; i++) { cerr << i << ' ' << V[i] << '\n'; }

	// We may assume that it's always possible, so no need to validate things

	// V[-1] and V[N] are negaive infty

	ll res = 1;
	for (int i = 0; i < N; ) {
		int j = i;
		while (j < N && V[j] == V[i]) j++;
		assert(j > i);
		int numHigher = (i > 0 && V[i-1] > V[i]) + (j < N && V[j] > V[j-1]);
		// solve it
		res *= solve(j-i, numHigher, V_TOP-V[i]), res %= MOD;
		i = j;
	}
	res %= MOD;
	if (res < 0) res += MOD;
	cout << res << '\n';
	return 0;
}
