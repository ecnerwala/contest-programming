#include <bits/stdc++.h>

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
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, C; cin >> N >> C;
		string A, B; cin >> A >> B;
		int start = 0;
		for (int i = 0; i < N; i++) {
			start += (A[i] == B[i]) << i;
		}
		std::vector<bool> legal(1 << N, true);
		for (int c = 0; c < C; c++) {
			string s; cin >> s;
			int cur = 0;
			for (int i = 0; i < N; i++) {
				cur += (s[i] == '=') << i;
			}
			legal[cur] = false;
		}

		std::vector<std::vector<int>> adj(1 << N);
		std::vector<std::vector<int>> adj2(1 << N);
		for (int a = 0; a < (1 << N); a++) {
			if (__builtin_popcount(a) & 1) continue;
			if (!legal[a]) continue;
			for (int i = 0; i < N; i++) {
				int b = a ^ (1 << i);
				if (!legal[b]) continue;
				adj[a].push_back(b);
				if (a != start && b != start) {
					adj2[a].push_back(b);
				}
			}
		}

		std::vector<int> btoa(1 << N, -1);
		int matching1 = kactl::hopcroftKarp(adj, btoa);

		std::vector<int> btoa2(1 << N, -1);
		int matching2 = kactl::hopcroftKarp(adj2, btoa2);
		//cerr << matching1 << ' ' << matching2 << '\n';

		if (matching1 > matching2) {
			cout << "Alice" << '\n';
		} else {
			cout << "Bob" << '\n';
		}
	}

	return 0;
}
