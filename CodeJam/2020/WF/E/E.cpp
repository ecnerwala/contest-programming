#include<bits/stdc++.h>

namespace kactl {
using namespace std;
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

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

#undef rep
#undef all
#undef sz
} // namespace kactl

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		const int V = 62;
		auto char_to_int = [](char c) -> int {
			if ('A' <= c && c <= 'Z') {
				return (c - 'A');
			} else if ('a' <= c && c <= 'z') {
				return (c - 'a') + 26;
			} else if ('0' <= c && c <= '9') {
				return (c - '0') + 26 + 26;
			} else assert(false);
		};

		string S; cin >> S;
		array<bool, V> has; has.fill(false);
		for (char c : S) {
			has[char_to_int(c)] = true;
		}
		int E; cin >> E;
		array<array<bool, V>, V> adj;
		for (int i = 0; i < V; i++) {
			for (int j = 0; j < V; j++) {
				adj[i][j] = (i == j);
			}
		}

		for (int e = 0; e < E; e++) {
			char x,y; cin >> x >> y;
			adj[char_to_int(x)][char_to_int(y)] = true;
		}

		for (int k = 0; k < V; k++) {
			for (int i = 0; i < V; i++) {
				for (int j = 0; j < V; j++) {
					if (adj[i][k] && adj[k][j]) adj[i][j] = true;
				}
			}
		}

		vector<int> sources; sources.reserve(V);
		vector<int> sinks; sinks.reserve(V);
		for (int i = 0; i < V; i++) {
			if (!has[i]) sinks.push_back(i);
		}

		// now we identify scc's
		for (int i = 0; i < V; i++) {
			bool is_root = true;
			for (int j = 0; j < i; j++) {
				if (adj[i][j] && adj[j][i]) {
					is_root = false;
					break;
				}
			}
			if (!is_root) continue;
			bool is_filled = true;
			for (int j = i; j < V; j++) {
				if (adj[i][j] && adj[j][i]) {
					if (!has[j]) {
						is_filled = false;
						break;
					}
				}
			}
			if (!is_filled) continue;
			assert(has[i]);

			bool has_edge = false;
			for (int j = 0; j < V; j++) {
				if (i != j && adj[i][j]) {
					has_edge = true;
					break;
				}
			}
			if (!has_edge) {
				// no need to include it
				continue;
			}

			// this guy is a source/sink
			sources.push_back(i);
			sinks.push_back(i);
		}

		vector<vector<int>> g(sources.size());
		for (int i = 0; i < int(sources.size()); i++) {
			for (int j = 0; j < int(sinks.size()); j++) {
				// can't connect to yourself
				if (sources[i] != sinks[j] && adj[sources[i]][sinks[j]]) {
					g[i].push_back(j);
				}
			}
		}
		vector<int> btoa(sinks.size(), -1);
		int matching = kactl::hopcroftKarp(g, btoa);
		int cost = int(sources.size()) - matching;

		int ans = accumulate(has.begin(), has.end(), int(0)) - cost;

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
