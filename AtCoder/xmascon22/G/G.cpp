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

std::vector<int> Generate(int N, long long S) {
	long long s = S;
	std::vector<int> A(4 * N);
	for (int i = 0; i < 4 * N; ++i) {
		A[i] = i / 4 + 1;
	}
	for (int i = 0; i < 4 * N; ++i) {
		s = (s * 2022) % 998244353;
		if (s % 2 != 0) {
			A[i] = -A[i];
		}
	}
	for (int i = 0; i < 4 * N; ++i) {
		s = (s * 2022) % 998244353;
		int j = s % (i + 1);
		int t = A[j];
		A[j] = A[i];
		A[i] = t;
	}
	return A;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; long long S; cin >> N >> S;
		std::vector<int> A = Generate(N, S);
		std::vector<std::vector<int>> adj(N);
		for (int i = 0; i < N; i++) {
			adj[i].reserve(4);
			for (int z = 0; z < 4; z++) {
				adj[i].push_back(abs(A[i*4+z])-1);
			}
		}
		std::vector<int> btoa(N, -1);
		int r = kactl::hopcroftKarp(adj, btoa);
		assert(r == N);
		string res; res.reserve(N);
		for (int v = 0; v < N; v++) {
			int i = btoa[v];
			for (int z = 0; z < 4; z++) {
				if (abs(A[i*4+z])-1 == v) {
					res.push_back(A[i*4+z] > 0 ? '+' : '-');
					break;
				}
			}
		}
		cout << res << '\n';
	}

	return 0;
}
