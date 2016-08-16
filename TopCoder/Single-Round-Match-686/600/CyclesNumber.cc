#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class CyclesNumber {

public:

	static const int MOD = 1e9 + 7;

	static const int MAXN = 2e5;
	static const int MAXM = 500;
	static const int N = 1e5 + 20;
	static const int M = 320;

	int Q;

	ll pow(ll a, ll e, ll m = MOD) {
		ll r = 1;
		while(e) {
			if(e & 1) r *= a, r %= m;
			a *= a, a %= m;
			e >>= 1;
		}
		return r;
	}

	ll dp[MAXM];
	ll dp2[MAXM][MAXM];
	ll choose[MAXM][MAXM];

	vector <int> getExpectation(vector <int> n, vector <int> m) {
		for(int i = 0; i <= M; i++) {
			choose[i][0] = choose[i][i] = 1;
			for(int j = 1; j < i; j++) {
				choose[i][j] = choose[i - 1][j - 1] + choose[i - 1][j];
				if(choose[i][j] >= MOD) choose[i][j] -= MOD;
			}
		}
		memset(dp2, 0, sizeof(dp2));
		dp2[0][0] = 1;
		for(int i = 1; i <= M; i++) {
			for(int l = 1; l <= i; l++) {
				for(int a = 0; a < i; a++) {
					dp2[i][l] += dp2[a][l - 1] * choose[i][a];
					dp2[i][l] %= MOD;
				}
			}
		}

		Q = int(n.size());
		assert(Q == int(m.size()));
		vector<pii> queries;
		vector<int> res(Q);

		for(int i = 0; i < Q; i++) {
			queries.push_back(make_pair(n[i], i));
		}
		sort(queries.begin(), queries.end());


		memset(dp, 0, sizeof(dp));
		dp[0] = 1;
		for(int i = 1, q = 0; i <= N && q < Q; i++) {
			for(int j = min(i, 320); j >= 0; j--) {
				dp[j] *= i;
				if(j) dp[j] += dp[j - 1];
				dp[j] %= MOD;
			}
			while(q < Q && queries[q].first == i) {
				int m2 = m[queries[q].second];
				ll val = 0;

				for(int l = 0; l <= m2; l++) {
					val += dp2[m2][l] * dp[l];
					val %= MOD;
				}

				res[queries[q].second] = val;
				q++;
			}
		}

		return res;
	}
};

// vim:ft=cpp
