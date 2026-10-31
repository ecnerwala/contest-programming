#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

// https://github.com/kth-competitive-programming/kactl/blob/master/content/graph/hopcroftKarp.h

bool dfs(int a, int L, vector<vi>& g, vi& btoa, vi& A, vi& B) {
	if (A[a] != L) return 0;
	A[a] = -1;
	for (int b : g[a]) if (B[b] == L + 1) {
		B[b] = 0;
		if (btoa[b] == -1 || dfs(btoa[b], L + 1, g, btoa, A, B))
			return btoa[b] = a, 1;
	}
	return 0;
}

int hopcroftKarp(vector<vi>& g, vi& btoa) {
	int res = 0;
	vi A(g.size()), B(btoa.size()), cur, next;
	for (;;) {
		fill(all(A), 0);
		fill(all(B), 0);
		/// Find the starting nodes for BFS (i.e. layer 0).
		cur.clear();
		for (int a : btoa) if(a != -1) A[a] = -1;
		rep(a,0,sz(g)) if(A[a] == 0) cur.push_back(a);
		/// Find all layers using bfs.
		for (int lay = 1;; lay++) {
			bool islast = 0;
			next.clear();
			for (int a : cur) for (int b : g[a]) {
				if (btoa[b] == -1) {
					B[b] = lay;
					islast = 1;
				}
				else if (btoa[b] != a && !B[b]) {
					B[b] = lay;
					next.push_back(btoa[b]);
				}
			}
			if (islast) break;
			if (next.empty()) return res;
			for (int a : next) A[a] = lay;
			cur.swap(next);
		}
		/// Use DFS to scan for augmenting paths.
		rep(a,0,sz(g))
			res += dfs(a, 0, g, btoa, A, B);
	}
}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M; cin >> N >> M;
	int L = 0;
	int R = 0;
	vector<int> V_id(N*N, -1);

	vector<vector<int>> G; G.reserve(min(N*N, M));
	for (int e = 0; e < M; e++) {
		int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2; x1--, y1--, x2--, y2--;
		int v1 = x1 * N + y1;
		int v2 = x2 * N + y2;
		assert((x1 ^ y1 ^ x2 ^ y2) & 1);
		if ((x1 ^ y1) & 1) swap(v1, v2);
		if (V_id[v1] == -1) {
			V_id[v1] = L++;
			G.emplace_back();
		}
		v1 = V_id[v1];
		if (V_id[v2] == -1) {
			V_id[v2] = R++;
		}
		v2 = V_id[v2];
		G[v1].push_back(v2);
	}

	vector<int> btoa(R, -1);
	int max_matching = kactl::hopcroftKarp(G, btoa);
	cout << N * N - max_matching << '\n';

	return 0;
}
