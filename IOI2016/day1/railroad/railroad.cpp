#include "railroad.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

struct coaster {
	static const ll INF = 1e9 + 20;
	static const int MAXN = 3e5;
	int N;
	vector<int> S;
	vector<int> T;

	static const int MAXV = 2 * MAXN;
	int V;
	vector<ll> coords;

	struct dsu {
		int par[MAXV];
		int rnk[MAXV];

		int get_par(int a) { return (par[a] == -1) ? (a) : (par[a] = get_par(par[a])); }

		bool merge(int a, int b) {
			a = get_par(a), b = get_par(b);
			if(a == b) return false;
			if(rnk[a] == rnk[b]) {
				par[a] = b;
				rnk[b] ++;
			} else if(rnk[a] < rnk[b]) {
				par[a] = b;
			} else {
				par[b] = a;
			}
			return true;
		}

		void reset() {
			memset(par, -1, sizeof(par));
			memset(rnk, 0, sizeof(rnk));
		}

		dsu() { reset(); }

	} conn;

	ll go() {
		vector<int> cnt(V);
		for(int i : S) cnt[i] ++;
		for(int i : T) cnt[i] --;
		for(int i = 1; i < V; i++) {
			cnt[i] += cnt[i-1];
		}
		assert(cnt[V-1] == 0);
		cnt.pop_back();
		ll res = 0;
		vector<pair<ll, int>> edges;
		for(int i = 0; i+1 < V; i++) {
			// cnt[i] connects i and i+1
			ll d = coords[i+1] - coords[i];
			if(cnt[i]) {
				conn.merge(i, i+1);
				res += d * max(cnt[i], 0);
			} else {
				edges.emplace_back(d, i);
			}
		}
		sort(edges.begin(), edges.end());
		for(auto it : edges) {
			if(conn.merge(it.second, it.second + 1)) res += it.first;
		}
		return res;
	}

	coaster(vector<int> s, vector<int> t): N(int(s.size())), S(s), T(t), conn() {
		S.push_back(INF);
		T.push_back(1);
		N++;
		for(int i : S) coords.push_back(i);
		for(int i : T) coords.push_back(i);
		sort(coords.begin(), coords.end());
		V = int(unique(coords.begin(), coords.end()) - coords.begin());
		coords.resize(V);
		for(int &i : S) i = int(lower_bound(coords.begin(), coords.end(), i) - coords.begin());
		for(int &i : T) i = int(lower_bound(coords.begin(), coords.end(), i) - coords.begin());
		for(int i = 0; i < N; i++) conn.merge(S[i], T[i]);
	}
};

ll plan_roller_coaster(vector<int> S, vector<int> T) {
	return coaster(S, T).go();
}
