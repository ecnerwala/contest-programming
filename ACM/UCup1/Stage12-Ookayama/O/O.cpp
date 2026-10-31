#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, SA, SB; cin >> N >> M >> SA >> SB; SA--, SB--;

	std::vector<std::vector<int>> out_edges(N);
	std::vector<std::vector<int>> in_edges(N);

	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		out_edges[u].push_back(v);
		in_edges[v].push_back(u);
	}

	int K; cin >> K;
	std::vector<int> jewel_id(N, 0);
	std::vector<int> jewel_weight(N, 0);
	for (int i = 0; i < K; i++) {
		int v, w; cin >> v >> w; v--;
		assert(jewel_id[v] == 0);
		jewel_id[v] += 1 << i;
		jewel_weight[v] += w;
	}

	// vector of size N*N for the two coordinates
	std::vector<std::vector<int>> all_dp(1 << K);
	for (int rem_jewels = 0; rem_jewels < (1 << K); rem_jewels++) {
		std::vector<std::pair<int, int>> jewel_moves;
		for (int v = 0; v < N; v++) {
			if (rem_jewels & jewel_id[v]) {
				for (int x = 0; x < N; x++) {
					if (jewel_id[x] & rem_jewels) {
						continue;
					}
					int value = all_dp[rem_jewels ^ jewel_id[v]][v*N+x] - jewel_weight[v];
					jewel_moves.push_back({v*N+x, value});
				}
			}
		}
		std::sort(jewel_moves.begin(), jewel_moves.end(), [&](auto a, auto b) -> bool {
			return std::abs(a.second) > std::abs(b.second);
		});

		std::vector<int> cur_dp(N*N, 0);
		std::vector<int> losing_deg(N*N);
		for (int s = 0; s < N*N; s++) {
			losing_deg[s] = int(out_edges[s%N].size());
		}

		std::vector<int> q; q.reserve(N*N);
		int q_idx = 0;
		for (auto [jewel_st, jewel_value] : jewel_moves) {
			if (jewel_value == 0) continue;
			cur_dp[jewel_st] = jewel_value;
			q.push_back(jewel_st);
			for (; q_idx < int(q.size()); q_idx++) {
				auto st = q[q_idx];
				assert(cur_dp[st] != 0);
				int move_dest = st / N;
				int other_pos = st % N;
				for (int move_src : in_edges[move_dest]) {
					if (jewel_id[move_src] & rem_jewels) continue;
					int prv_st = other_pos * N + move_src;
					if (cur_dp[st] > 0) {
						// everything that can go to it is a little more losing
						losing_deg[prv_st]--;
						assert(losing_deg[prv_st] >= 0);
						if (losing_deg[prv_st] == 0) {
							assert(cur_dp[prv_st] == 0);
							cur_dp[prv_st] = -cur_dp[st];
							q.push_back(prv_st);
						}
					} else {
						assert(cur_dp[prv_st] >= 0);
						assert(cur_dp[prv_st] == 0 || cur_dp[prv_st] >= -cur_dp[st]);
						if (cur_dp[prv_st] == 0) {
							cur_dp[prv_st] = -cur_dp[st];
							q.push_back(prv_st);
						}
					}
				}
			}
		}
		all_dp[rem_jewels] = cur_dp;
	}
	cout << all_dp[(1<<K) - 1][SB*N+SA] << '\n';

	return 0;
}
