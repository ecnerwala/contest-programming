#include <bits/stdc++.h>

const int MOD = int(1e9)+7;
int mod_plus(int a, int b) {
	a += b;
	return a >= MOD ? a - MOD : a;
}
int mod_minus(int a, int b) {
	a -= b;
	return a < 0 ? a + MOD : a;
}
int mod_mul(int a, int b) {
	return int(int64_t(a) * int64_t(b) % MOD);
}
int mod_pow(int a, int b) {
	int r = 1;
	while (b) {
		if (b & 1) r = mod_mul(r, a);
		a = mod_mul(a, a);
		b >>= 1;
	}
	return r;
}

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
				vector<int> dist_cnt(N);
				for (int i = 0; i < N; i++) dist_cnt[dist[i]]++;
				int ans = 1;
				for (int i = 1; i < N; i++) {
					if (!dist_cnt[i]) break;
					// luckily for us, 2^k != 0
					ans = mod_mul(ans, mod_pow(mod_pow(2, dist_cnt[i-1]) - 1, dist_cnt[i]));
				}
				return ans;
			}

			vector<vector<int>> groups(2*N); // should be a big enough upper bound...
			for (int i = 0; i < N; i++) {
				assert(dist[i] % 2 != dist2[i] % 2);
				int group = (dist[i] + dist2[i] - 1) / 2;
				int layer = (dist2[i] - dist[i] - 1) / 2;
				groups[group].push_back(layer);
			}

			vector<int> fact(N+1);
			fact[0] = 1;
			for (int i = 1; i <= N; i++) fact[i] = mod_mul(fact[i-1], i);
			vector<int> ifact(N+1);
			ifact[N] = mod_pow(fact[N], MOD-2);
			for (int i = N; i >= 1; i--) ifact[i-1] = mod_mul(ifact[i], i);
			auto choose = [&](int n, int r) {
				assert(0 <= r && r <= n);
				return mod_mul(fact[n], mod_mul(ifact[n-r], ifact[r]));
			};

			vector<int> dp; dp.reserve(N+1);
			dp.push_back(1);
			vector<int> ndp; ndp.reserve(N+1);

			// Let anyone who's covered claim to be uncovered
			auto relax_covering = [&]() {
				int tot = int(dp.size())-1;
				for (int i = tot; i >= 0; i--) {
					for (int j = i-1; j >= 0; j--) {
						dp[i] = mod_plus(dp[i], mod_mul(dp[j], choose(tot-j, i-j)));
					}
				}
			};

			vector<int> bottom_ways(N+1);
			for (int i = 0; i <= N; i++) {
				bottom_ways[i] = mod_pow(2, i * (i+1) / 2);
				for (int j = 0; j < i; j++) {
					bottom_ways[i] = mod_minus(bottom_ways[i], mod_mul(bottom_ways[j], choose(i, j)));
				}
			}

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

				int last_l = N;
				dp.resize(1); // shrink down to nothing uncovered
				for (auto [l, cnt] : cur_cnts) {
					if (std::exchange(last_l, l) > l+1) {
						dp.resize(1);
					}

					{
						relax_covering();

						ndp.assign(cnt+1, 0);
						for (int j = 0; j < int(ndp.size()); j++) {
							int num_opts = mod_pow(2, j) - 1;
							int cur_ways = 1;
							for (int i = 0; i < int(dp.size()); i++, cur_ways = mod_mul(cur_ways, num_opts)) {
								ndp[j] = mod_plus(ndp[j], mod_mul(dp[i], cur_ways));
							}
						}
						for (int j = 0; j < int(ndp.size()); j++) {
							for (int i = 0; i < j; i++) {
								ndp[j] = mod_minus(ndp[j], mod_mul(ndp[i], choose(j, i)));
							}
						}
						for (int j = 0; j < int(ndp.size()); j++) {
							ndp[j] = mod_mul(ndp[j], choose(cnt, j));
						}
						reverse(ndp.begin(), ndp.end());
						std::swap(dp, ndp);
					}

					if (l == g) {
						// Special case! This guy is necessarily unmatched because he's the root!
						// Just let him through
						assert(cnt == 1);
						assert(dp[0] == 0);
						dp[0] = dp[1];
						dp[1] = 0;
					}

					int num_edges;
					if (last_layer_count[l].first == g-1) {
						num_edges = last_layer_count[l].second;
					} else {
						num_edges = 0;
					}

					// pointwise-connected
					int num_opts = mod_pow(2, num_edges) - 1;
					relax_covering();
					for (int i = 0; i < int(dp.size()); i++) {
						dp[i] = mod_mul(dp[i], mod_pow(num_opts, i));
					}
					reverse(dp.begin(), dp.end());

					last_layer_count[l] = {g, cnt};
				}
				if (last_l > 0) {
					dp.resize(1);
				} else {
					relax_covering();
					int res = 0;
					for (int j = 0; j < int(dp.size()); j++) {
						res = mod_plus(res, mod_mul(dp[j], bottom_ways[j]));
					}
					dp.resize(1);
					dp[0] = res;
				}
			}

			return dp[0];
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
//  We want to find the number of edge covers of each group.
