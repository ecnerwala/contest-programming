#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		vector<vector<int>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		vector<int> dist(N, -1);
		vector<int> dist2(N, -1);
		vector<int> q; q.reserve(2*N);
		dist[0] = 0;
		q.push_back(0);
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			if (cur >= 0) {
				for (int nxt : adj[cur]) {
					if (dist[nxt] == -1) {
						dist[nxt] = dist[cur]+1;
						q.push_back(nxt);
					} else if (dist[nxt] == dist[cur] && dist2[nxt] == -1) {
						dist2[nxt] = dist[cur]+1;
						q.push_back(~nxt);
					}
				}
			} else {
				cur = ~cur;
				for (int nxt : adj[cur]) {
					if (dist2[nxt] == -1) {
						dist2[nxt] = dist2[cur]+1;
						q.push_back(~nxt);
					}
				}
			}
		}
		cout << [&]() -> int {
			if (dist2[0] == -1) {
				return N-1;
			}
			vector<vector<int>> groups(2*N); // should be a big enough upper bound...
			for (int i = 0; i < N; i++) {
				assert(dist[i] % 2 != dist2[i] % 2);
				int group = (dist[i] + dist2[i] - 1) / 2;
				int layer = (dist2[i] - dist[i] - 1) / 2;
				groups[group].push_back(layer);
			}
			int ans = 0;
			vector<pair<int, int>> last_layer_count(N, {-2, -1});
			for (int g = 0; g < int(groups.size()); g++) {
				auto& layers = groups[g];
				if (layers.empty()) continue;
				sort(layers.begin(), layers.end());
				vector<pair<int, int>> cur_cnts; cur_cnts.reserve(layers.size());
				for (int i : layers) {
					if (cur_cnts.empty() || cur_cnts.back().first != i) {
						cur_cnts.push_back({i, 0});
					}
					cur_cnts.back().second++;
				}
				reverse(cur_cnts.begin(), cur_cnts.end());
				int num_uncovered = 0;
				int last_l = N;
				for (auto [l, cnt] : cur_cnts) {
					if (std::exchange(last_l, l) > l+1) {
						ans += num_uncovered;
						num_uncovered = 0;
					}
					ans += num_uncovered;
					num_uncovered = max(cnt - num_uncovered, 0);
					if (last_layer_count[l].first == g-1) {
						ans += num_uncovered;
						num_uncovered = cnt - num_uncovered;
					} else {
						ans += num_uncovered;
						num_uncovered = cnt;
					}
					last_layer_count[l] = {g, cnt};
				}
				if (last_l > 0) {
					ans += num_uncovered;
				} else {
					ans += (num_uncovered + 1) / 2;
				}
			}

			ans--; // Extra root edge

			return ans;
		}() << '\n';
	}

	return 0;
}


// The graph is undirected, so if f_G(a,b) then f_G(a, b+2), because we can go back and forth along any edge
// This means, that f_G encodes the shortest paths and the shortest paths of opposite parity.
//
// We can consider the edges partitioned by distance (bfs tree/dag):
//   There are edges between levels, and edges within levels (including self loops)
//  * A path for dist2 never needs to take >= 2 in-level edge, because then we
//     could've taken the shortest path to the dest of the 2nd in-level-edge
//  * Therefore, dist2 is just the shortest path to an in-level edge's endpoint
//  * dist2[v] == dist[v]+1 if v has at least 1 in-level edge
//  Now, cross-level edges are more interesting:
//   * Each vertex must have at least 1 upwards cross-level edge (which just be to dist2[v]+-1)
//   * Each vertex must either have dist2[v] == dist[v]+1 and have an in-level edge, or must have at least 1 cross-level edge to dist2[v]-1
//
//  Any candidate edge either satisfies both requirements for a single vertex,
//    or satisfies 1 from each (which only happens if dist[u]+dist2[u] == dist[v]+dist2[v] is the same)
//    or is an in-level edge (and dist[u]+1 == dist2[u])
//  This means we can treat every equivalence class of dist[u]+dist2[u] almost totally separately.
//  We want to find the min-edge-cover of each group, which we can do by greedy.
