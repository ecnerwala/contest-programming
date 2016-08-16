#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class CliqueCuts {

	static const ll MOD = 1e9 + 7;
	static const int MAXN = 50;
	static const int MAXR = 23;

	int N, L, R;
	int M;
	ll alltot;
	ll val[MAXN][MAXN];
	ll tot[MAXN];
	ll adj[MAXN];
	ll subcnt[1 << MAXR];
	ll subval[1 << MAXR];

	ll go() {
		R = min(N - 1, 23);
		L = N - R;
		subcnt[0] = 1;
		subval[0] = 0;
		for(ll m = 1; m < (1ll << R); m++) {
			if(subcnt[m & (m - 1)] == 0) {
				subcnt[m] = 0;
				subval[m] = 0;
			} else {
				int i = 0;
				while(!(m & (1ll << i))) i++;
				assert(((m & (m - 1)) + (1ll << i)) == m);
				if((adj[i + L] | m) == adj[i + L]) {
					subcnt[m] = 1;
					subval[m] = (subval[m & (m - 1)] + tot[i + L]) % MOD;
				} else {
					subcnt[m] = 0;
					subval[m] = 0;
				}
			}
		}
		// sum over subsets
		for(int i = 0; i < R; i++) {
			for(ll m = 0; m < (1ll << R); m++) {
				if(m & (1ll << i)) {
					subcnt[m] += subcnt[m ^ (1ll << i)];
					if(subcnt[m] >= MOD) subcnt[m] -= MOD;
					subval[m] += subval[m ^ (1ll << i)];
					if(subval[m] >= MOD) subval[m] -= MOD;
				}
			}
		}

		ll res = alltot;
		for(ll m = 0; m < (1ll << L); m++) {
			ll cover = (1ll << N) - 1;
			ll lval = 0;
			for(int i = 0; i < L; i++) {
				if(m & (1ll << i)) {
					cover = cover & adj[i];
					lval += tot[i];
					if(lval >= MOD) lval -= MOD;
				}
			}
			if((cover & m) != m) continue;
			//cerr << m << ' ' << lval << ' ' << alltot << '\n';
			ll right = cover >> L;
			//cerr << right << ' ' << subcnt[right] << ' ' << subval[right] << '\n';
			res += subcnt[right] * lval + subval[right] - subcnt[right] * alltot;
			res %= MOD;
			if(res < 0) res += MOD;
		}
		return res;
	}

public:

	int sum(int n, vector <int> a, vector <int> b, vector <int> c) {
		N = n;
		M = int(a.size());
		alltot = 0;
		memset(tot, 0, sizeof(tot));
		memset(adj, 0, sizeof(adj));
		for(int i = 0; i < M; i++) {
			adj[a[i]] |= (1ll << b[i]);
			adj[b[i]] |= (1ll << a[i]);
			tot[a[i]] += c[i];
			tot[b[i]] += c[i];
			alltot += c[i];
		}
		alltot %= MOD;
		for(int i = 0; i < N; i++) {
			adj[i] |= (1ll << i);
			tot[i] %= MOD;
		}
		return int(((go() % MOD) + MOD) % MOD);
	}
};

// vim:ft=cpp
