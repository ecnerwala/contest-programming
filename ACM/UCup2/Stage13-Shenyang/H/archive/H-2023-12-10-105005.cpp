#include <bits/stdc++.h>

int solve(int N, std::vector<std::array<int, 2>> edges, int K) {
	const int ORIG_N = N;
	assert(K > 0);
	if (K == 1) {
		return N;
	}
	assert(K >= 2);
	int ans = std::min(N, int(edges.size()));

	for (int z = 2; z < K; z++) {
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < int(edges.size()); e++) {
			auto [u, v] = edges[e];
			adj[u].push_back(e);
			adj[v].push_back(e);
		}

		int64_t nxt_edges = 0;
		for (int i = 0; i < N; i++) {
			nxt_edges += int64_t(adj[i].size()) * (int64_t(adj[i].size()) - 1) / 2;
		}
		if (nxt_edges > ans + 2 * ORIG_N) {
			return ans;
		}

		if (nxt_edges < ans) ans = int(nxt_edges);

		if (z == 4) {
			// check if they're all cycles/paths
			bool all_cyc = true;
			for (int i = 0; i < N; i++) {
				if (adj[i].size() > 2) {
					all_cyc = false;
					break;
				}
			}
			if (all_cyc) {
				std::vector<int> path_lens;
				path_lens.reserve(N);
				int cyc_len = N;
				std::vector<bool> vis(N);
				for (int i = 0; i < N; i++) {
					if (vis[i]) continue;
					if (adj[i].size() < 2) {
						int cur = i;
						int prv_e = -1;
						int cnt = 0;
						while (true) {
							vis[cur] = true;
							cnt++;

							assert(int(adj[cur].size()) <= 2);
							if (adj[cur].size() == 2) assert(prv_e != -1);

							int a = 0;
							while (a < int(adj[cur].size()) && adj[cur][a] == prv_e) a++;
							if (a == int(adj[cur].size())) break;
							int nxt_e = adj[cur][a];
							int nxt = edges[nxt_e][0] == cur ? edges[nxt_e][1] : edges[nxt_e][0];
							cur = nxt;
							prv_e = nxt_e;
						}
						path_lens.push_back(cnt);
						cyc_len -= cnt;
					}
				}
				// we want edges after K - z more ops
				assert(nxt_edges == N - int(path_lens.size()));
				int my_ans = 0;
				for (auto l : path_lens) {
					my_ans += std::max(0, l - 1 - (K - z));
				}
				my_ans += cyc_len;
				ans = std::min(ans, my_ans);
				return my_ans;
			}
		}

		// fill in the new graph
		std::vector<std::array<int, 2>> new_edges; new_edges.reserve(nxt_edges);
		for (int i = 0; i < N; i++) {
			for (int a = 0; a < int(adj[i].size()); a++) {
				for (int b = a+1; b < int(adj[i].size()); b++) {
					new_edges.push_back({adj[i][a], adj[i][b]});
				}
			}
		}
		N = int(edges.size());
		edges = std::move(new_edges);
	}

	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		std::vector<std::array<int, 2>> edges(M);
		for (auto& [u, v] : edges) {
			cin >> u >> v; u--, v--;
		}
		cout << solve(N, edges, K) << '\n';
	}

	return 0;
}
