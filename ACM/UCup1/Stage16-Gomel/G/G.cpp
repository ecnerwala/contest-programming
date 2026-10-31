#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASES; cin >> CASES;
	while (CASES--) {
		int N, M; cin >> N >> M;
		std::vector<std::array<int, 2>> edges(M);
		for (auto& [u, v] : edges) {
			cin >> u >> v; u--, v--;
		}

		std::vector<std::vector<std::pair<int, int>>> adj(N);
		std::vector<int> deg(N);
		for (int e = 0; e < M; e++) {
			auto [u, v] = edges[e];
			adj[u].push_back({v, e});
			adj[v].push_back({u, e});
			deg[u]++;
			deg[v]++;
		}

		std::vector<int> num_leaves(N);
		for (int i = 0; i < N; i++) {
			if (deg[i] == 1) {
				num_leaves[adj[i][0].first]++;
			}
		}

		for (int i = 0; i < N; i++) {
			assert(deg[i] >= 1);
			assert(num_leaves[i] <= 2);
		}

		std::vector<bool> is_alive(M, true);
		auto prune_alive = [&](int u) -> void {
			int l = 0;
			for (int z = 0; z < int(adj[u].size()); z++) {
				if (is_alive[adj[u][z].second]) {
					adj[u][l++] = adj[u][z];
				}
			}
			adj[u].resize(l);
			assert(l == deg[u]);
		};
		for (int e = 0; e < M; e++) {
			auto [u, v] = edges[e];
			assert(u != v);
			assert(deg[u] >= 1 && deg[v] >= 1);
			if (deg[u] == 1 || deg[v] == 1) continue;

			int ou = -1;
			if (deg[u] == 2) {
				prune_alive(u);
				ou = adj[u][0].first ^ adj[u][1].first ^ v;
				assert(ou != -1 && ou != u && ou != v);
			}
			if (ou != -1 && num_leaves[ou] == 2) continue;

			int ov = -1;
			if (deg[v] == 2) {
				prune_alive(v);
				ov = adj[v][0].first ^ adj[v][1].first ^ u;
				assert(ov != -1 && ov != u && ov != v);
			}
			if (ov != -1 && num_leaves[ov] == 2) continue;

			if (ou != -1 && ov != -1 && ou == ov && num_leaves[ou] >= 1) {
				continue;
			}

			// otherwise, we can prune
			is_alive[e] = false;
			deg[u]--;
			deg[v]--;
			if (ou != -1) {
				assert(deg[u] == 1);
				num_leaves[ou]++;
				assert(num_leaves[ou] <= 2);
			}
			if (ov != -1) {
				assert(deg[v] == 1);
				num_leaves[ov]++;
				assert(num_leaves[ov] <= 2);
			}
		}

		for (int i = 0; i < N; i++) {
			prune_alive(i);
			assert(deg[i] >= 1);
			assert(num_leaves[i] <= 2);
		}

		std::vector<int> side(N);
		int cur_cnt = 0;

		int tgt_cnt = N / 2;

		for (int i = 0; i < N; i++) {
			if (num_leaves[i] == 1) {
				assert(deg[i] == 1);
				int j = adj[i][0].first;
				assert(deg[j] == 1);
				assert(num_leaves[j] == 1);
				if (i < j) {
					side[i] = 0;
				} else {
					side[i] = 1;
					cur_cnt++;
				}
			} else if (num_leaves[i] == 2) {
				side[i] = 1;
				cur_cnt++;
			} else if (deg[i] == 1) {
				// we're just a leaf
				side[i] = 0;
				assert(num_leaves[adj[i][0].first] == 2);
			} else {
				assert(deg[i] == 2);
				side[i] = 0;
				assert(num_leaves[adj[i][0].first] == 2);
				assert(num_leaves[adj[i][1].first] == 2);
			}
		}

		for (int i = 0; cur_cnt < tgt_cnt; i++) {
			if (num_leaves[i] != 2) continue;
			// flip the current guy
			assert(side[i] == 1);
			side[i] = 0;
			cur_cnt--;
			int c_leaves = 0;
			for (auto [j, _] : adj[i]) {
				if (deg[j] == 1) {
					c_leaves++;
					assert(side[j] == 0);
					side[j] = 1;
					cur_cnt++;
				}
			}
			assert(c_leaves == 2);
			assert(cur_cnt <= tgt_cnt);
			if (cur_cnt == tgt_cnt) break;

			for (auto [j, _] : adj[i]) {
				if (cur_cnt < tgt_cnt && side[j] == 0) {
					side[j] = 1;
					cur_cnt++;
					if (cur_cnt == tgt_cnt) break;
				}
			}
		}

		assert(cur_cnt == tgt_cnt);

		std::vector<int> ans;
		ans.reserve(tgt_cnt);
		for (int i = 0; i < N; i++) {
			if (side[i] == 1) {
				ans.push_back(i);
			}
		}
		assert(int(ans.size()) == tgt_cnt);
		for (int z = 0; z < tgt_cnt; z++) {
			cout << ans[z]+1 << " \n"[z+1==tgt_cnt];
		}
	}

	return 0;
}
