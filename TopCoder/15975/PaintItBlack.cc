#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) begin(x), end(x)
template <typename T> int sz (const T& a) { return int(a.size()); }
using ll = long long;
using pii = pair<int, int>;
using vi = vector<int>;

vi eulerWalk(vector<vector<pii>>& gr, int nedges, int src=0) {
	int n = sz(gr);
	vi D(n), its(n), eu(nedges), ret, s = {src};
	D[src]++; // to allow Euler paths, not just cycles
	while (!s.empty()) {
		int x = s.back(), y, e, &it = its[x], end = sz(gr[x]);
		if (it == end){ ret.push_back(x); s.pop_back(); continue; }
		tie(y, e) = gr[x][it++];
		if (!eu[e]) {
			D[x]--, D[y]++;
			eu[e] = 1; s.push_back(y);
		}}
	trav(x, D) if (x < 0 || sz(ret) != nedges+1) return {};
	return {ret.rbegin(), ret.rend()};
}

class PaintItBlack {

public:

	vector <int> findWalk(int N, int U, vector <int> a, vector <int> b) {
		vector<vector<int>> adj(N);
		for (int i = 0; i < int(a.size()); i++) {
			adj[a[i]].push_back(b[i]);
			adj[b[i]].push_back(a[i]);
		}
		vector<int> par(N, -1);
		vector<int> depth(N);
		vector<int> q; q.reserve(N);

		par[U] = -2;
		q.push_back(U);

		for (int z = 0; z < N; z++) {
			int cur = q[z];
			for (int nxt : adj[cur]) {
				if (par[nxt] == -1) {
					par[nxt] = cur;
					depth[nxt] = depth[cur]+1;
					q.push_back(nxt);
				}
			}
		}

		vector<int> val(N, 1);

		int numE = 0;
		vector<vector<pair<int, int>>> et_edges(N);
		auto addEdge = [&](int x, int y) {
			et_edges[x].emplace_back(y, numE);
			et_edges[y].emplace_back(x, numE);
			numE++;
		};
		if (N & 1) {
			// need to find a flip
			for (int i = 0; i < int(a.size()); i++) {
				int x = a[i], y = b[i];
				if (depth[x] % 2 == depth[y] % 2) {
					addEdge(x, y);
					while (x != y) {
						if (depth[x] > depth[y]) {
							val[x] ^= 1;
							addEdge(x, par[x]);
							x = par[x];
						} else {
							val[y] ^= 1;
							addEdge(y, par[y]);
							y = par[y];
						}
					}
					val[x] ^= 1;
					goto found;
				}
			}
			return {};
			found: ;
		}

		for (int z = N-1; z > 0; z--) {
			int cur = q[z];
			if (val[cur]) {
				addEdge(cur, par[cur]);
				addEdge(cur, par[cur]);
				val[cur] ^= 1;
				val[par[cur]] ^= 1;
			} else {
				addEdge(cur, par[cur]);
				addEdge(cur, par[cur]);
				addEdge(cur, par[cur]);
				addEdge(cur, par[cur]);
			}
		}
		assert(val[U] == 0);

		return eulerWalk(et_edges, numE, U);
	}
};

void my_tests() {
	// FILL IN TESTS
	vector<int> path = PaintItBlack().findWalk(5, 4, {0,1,2,3,4},{1,2,3,4,0});
	for (int i : path) { cerr << i << ' '; } cerr << '\n';
}

}  // namespace ecnerwala

using ecnerwala::PaintItBlack;

// vim:ft=cpp
