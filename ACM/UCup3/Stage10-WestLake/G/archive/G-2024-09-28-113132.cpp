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
		int N, M, K; cin >> N >> M >> K;
		std::vector<std::pair<std::array<int, 2>, int>> pts; pts.reserve(N+M);
		for (int i = 0; i < N; i++) {
			int x, y; cin >> x >> y;
			pts.push_back({std::array<int, 2>{x, y}, ~i});
		}
		for (int e = 0; e < M; e++) {
			int x, y; cin >> x >> y;
			pts.push_back({std::array<int, 2>{x, y}, e});
		}
		int num_see = 0;

		// We have N pts on each side
		std::vector<std::array<int, 2>> cover(M, {-1, -1});
		std::vector<std::array<int, 2>> any_edge(N, {-1, -1});
		for (int z = 0; z < 2; z++) {
			std::sort(pts.begin(), pts.end());
			for (int l = 0; l < int(pts.size()); l++) {
				if (pts[l].second >= 0) continue;
				int r = l+1;
				while (r < int(pts.size()) && pts[r].second >= 0) r++;
				if (r == int(pts.size()) || pts[r].first[0] != pts[l].first[0]) continue;
				int a = ~pts[l].second;
				num_see++;
				for (int m = l+1; m <= r-1; m++) {
					int e = pts[m].second;
					cover[e][z] = a;
					any_edge[a][z] = e;
				}
			}
			for (auto& p : pts) { std::swap(p.first[0], p.first[1]); }
		}
		std::vector<int> btoa(N, -1);
		std::vector<std::vector<int>> adj(N);
		std::map<std::array<int, 2>, int> edge_idx;
		for (int e = 0; e < M; e++) {
			if (cover[e][0] != -1 && cover[e][1] != -1) {
				adj[cover[e][0]].push_back(cover[e][1]);
				edge_idx[cover[e]] = e;
			}
		}
		int matching = kactl::hopcroftKarp(adj, btoa);
		int NUM_KEEP = M - K;
		std::vector<int> keep(M);

		std::vector<bool> used_a0(N);
		std::vector<bool> used_a1(N);
		for (int a1 = 0; a1 < N; a1++) {
			if (btoa[a1] != -1) {
				int a0 = btoa[a1];
				assert(!used_a0[a0]);
				used_a0[a0] = true;
				assert(!used_a1[a1]);
				used_a1[a1] = true;
				if (NUM_KEEP > 0) {
					num_see -= 2;
					NUM_KEEP--;
					keep[edge_idx[{a0, a1}]] = true;
				}
			}
		}
		for (int a0 = 0; a0 < N; a0++) {
			if (!used_a0[a0] && any_edge[a0][0] != -1) {
				used_a0[a0] = true;
				if (NUM_KEEP > 0) {
					num_see -= 1;
					NUM_KEEP--;
					keep[any_edge[a0][0]] = true;
				}
			}
		}
		for (int a1 = 0; a1 < N; a1++) {
			if (!used_a1[a1] && any_edge[a1][1] != -1) {
				used_a1[a1] = true;
				if (NUM_KEEP > 0) {
					num_see -= 1;
					NUM_KEEP--;
					keep[any_edge[a1][1]] = true;
				}
			}
		}
		for (int e = 0; e < M; e++) {
			if (!keep[e] && NUM_KEEP > 0) {
				NUM_KEEP--;
				keep[e] = true;
			}
		}
		std::vector<int> to_delete; to_delete.reserve(K);
		assert(NUM_KEEP == 0);
		for (int e = 0; e < M; e++) {
			if (!keep[e]) to_delete.push_back(e);
		}
		cout << num_see << '\n';
		assert(int(to_delete.size()) == K);
		for (int i = 0; i < K; i++) {
			cout << to_delete[i]+1 << " \n"[i+1==K];
		}
	}

	return 0;
}
