#include <bits/stdc++.h>
using namespace std;

class TwoEntrances {

	typedef long long ll;

	static const ll MOD = 1000000007;

	static const int MAXN = 4000;
	int N;
	vector<int> adj[MAXN];

	// l + r choose l = l + r choose r
	ll choose[MAXN][MAXN];

	void precomp_choose() {
		for(int i = 0; i <= N; i++) {
			choose[i][0] = choose[0][i] = 1;
		}

		for(int i = 1; i <= N; i++) {
			for(int j = 1; j <= N; j++) {
				choose[i][j] = choose[i - 1][j] + choose[i][j - 1];
				if(choose[i][j] >= MOD) choose[i][j] -= MOD;
			}
		}

	}

	int P;
	int path[MAXN];
	bool in_path[MAXN];

	int S1, S2;

	bool dfs_path(int cur, int par, int end) {
		path[P++] = cur;
		in_path[cur] = true;

		if(cur == end) {
			return true;
		}

		for(int nxt : adj[cur]) {
			if(nxt == par) continue;
			if(dfs_path(nxt, cur, end)) {
				return true;
			}
		}

		P--;
		in_path[cur] = false;

		return false;
	}

	int sz[MAXN]; // size of subtree
	ll orders[MAXN]; // number of orders of the subtree of this

	void dfs_orders(int cur, int prv) {

		sz[cur] = 0;
		orders[cur] = 1;

		for(int nxt : adj[cur]) {
			if(nxt == prv) continue;
			if(in_path[nxt]) continue;

			dfs_orders(nxt, cur);

			orders[cur] *= choose[sz[cur]][sz[nxt]];
			orders[cur] %= MOD;
			sz[cur] += sz[nxt];
		}

		sz[cur] ++;
	}

	int ways[2][MAXN][MAXN]; // number of ways to get from Si to path[j + 1] with exactly k deep ones

public:

	int count(vector <int> a, vector <int> b, int s1, int s2) {
		int N = a.size() + 1;
		S1 = s1, S2 = s2;

		for(int i = 0; i < N; i++) {
			adj[a[i]].push_back(b[i]);
			adj[b[i]].push_back(a[i]);
		}

		precomp_choose();

		P = 0;
		memset(in_path, 0, sizeof(in_path));
		dfs_path(S1, -1, S2);

		assert(P >= 2);
		assert(path[0] == S1);
		assert(path[P - 1] == S2);

		memset(orders, -1, sizeof(orders));

		ll res = 1;
		for(int i = 0; i < P; i++) {
			int v = path[i];
			dfs_orders(v, -1);
			res *= orders[v];
			res %= MOD;
			sz[v] --;
		}

		for(int z = 0; z < 2; z++) {
			int totnum = 0;
			ways[z][0][0] = 1;
			for(int i = 1; i <= P; i++) {
				int s = sz[path[i - 1]];
				int totways = 0;
				for(int k = totnum; k >= 0; k--) {
					totways += ways[z][i - 1][k];
					// totways is number of ways with >= k deep ones
					ways[z][i][s + k] += totways * choose[k][s];
				}

				totnum += s;
			}
			reverse(path, path + P);
		}
	}
};

// vim:ft=cpp
