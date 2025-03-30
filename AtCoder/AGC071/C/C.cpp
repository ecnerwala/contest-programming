#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::vector<std::pair<int, int>>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back({v, e});
			adj[v].push_back({u, e});
		}
		std::vector<std::vector<int>> deg_list(N);

		bool is_good = true;
		{
			std::vector<int> depth(N, -1);
			std::vector<int> lowval(N, -1);
			depth[0] = 0;
			std::vector<int> vert_stk; vert_stk.reserve(N);
			std::vector<std::pair<int, int>> edge_stk; edge_stk.reserve(M);

			std::vector<int> deg(N);
			[&](this auto&& self, int cur, int prv_e) -> void {
				lowval[cur] = depth[cur];
				for (auto [nxt, e] : adj[cur]) {
					if (e == prv_e) continue;
					if (depth[nxt] == -1) {
						depth[nxt] = depth[cur] + 1;
						int v_idx = int(vert_stk.size());
						int e_idx = int(edge_stk.size());
						vert_stk.push_back(nxt);
						edge_stk.push_back({cur, nxt});
						self(nxt, e);
						lowval[cur] = std::min(lowval[cur], lowval[nxt]);
						if (lowval[nxt] >= depth[cur]) {
							// bridge or bcc
							while (int(edge_stk.size()) > e_idx) {
								auto [u, v] = edge_stk.back();
								deg[u]++;
								deg[v]++;
								edge_stk.pop_back();
							}
							while (int(vert_stk.size()) > v_idx) {
								int v = vert_stk.back();
								deg_list[v].push_back(deg[v]);
								deg[v] = 0;
								vert_stk.pop_back();
							}
							deg_list[cur].push_back(deg[cur]);
							deg[cur] = 0;
						}
					} else if (depth[nxt] < depth[cur]) {
						if ((depth[nxt] ^ depth[cur]) & 1) is_good = false;
						edge_stk.push_back({cur, nxt});
						lowval[cur] = std::min(lowval[cur], depth[nxt]);
					}
				}
			}(0, -1);
			assert(vert_stk.empty());
			assert(edge_stk.empty());
		}
		for (auto& v : deg_list) {
			std::sort(v.begin(), v.end());
			int pref = 0;
			for (auto a : v) {
				if (a > pref + 1) is_good = false;
				pref += a;
			}
		}
		cout << (is_good ? "No" : "Yes") << '\n';
	}

	return 0;
}
