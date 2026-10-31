#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<int> mana(N);
	for (auto& x : mana) cin >> x;

	std::vector<int64_t> tot_mana(1<<N);
	for (int i = 0; i < N; i++) {
		for (int v = 0; v < (1 << i); v++) {
			tot_mana[(1<<i) + v] = tot_mana[v] + mana[i];
		}
	}

	const int64_t INF = 4e18;
	std::vector<int64_t> dist(N*N, INF);
	for (int i = 0; i < N; i++) dist[i*N+i] = 0;
	for (int e = 0; e < M; e++) {
		int a, b; int64_t t; cin >> a >> b >> t; a--, b--;
		dist[a*N+b] = std::min(dist[a*N+b], t);
	}

	for (int k = 0; k < N; k++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				dist[i*N+j] = std::min(dist[i*N+j], dist[i*N+k] + dist[k*N+j]);
			}
		}
	}

	std::vector<int64_t> dp(N<<N, -INF);
	for (int i = 0; i < N; i++) {
		dp[(1<<i)*N+i] = 0;
	}
	for (int m = 1; m < (1 << N); m++) {
		int64_t wait_cost = tot_mana[m];
		for (int i = 0; i < N; i++) {
			if (!(m & (1 << i))) continue;
			int64_t cur_val = dp[m*N+i];
			if (cur_val == -INF) continue;
			for (int j = 0; j < N; j++) {
				if (m & (1 << j)) continue;
				int64_t t = dist[i*N+j];
				if (t == INF) continue;

				int64_t nxt_val = cur_val - wait_cost * t;
				int64_t& ndp = dp[(m + (1 << j)) * N + j];
				if (nxt_val > ndp) ndp = nxt_val;
			}
		}
	}

	std::vector<std::pair<int64_t, int>> sorted_masks(1<<N);
	for (int m = 0; m < (1<<N); m++) {
		sorted_masks[m] = {tot_mana[m], m};
	}
	std::sort(sorted_masks.begin(), sorted_masks.end());

	std::vector<std::vector<std::pair<int64_t, std::pair<int64_t, int64_t>>>> hulls(N);
	for (int i = 0; i < N; i++) {
		hulls[i].reserve(1<<(N-1));
	}

	for (auto [rate, m] : sorted_masks) {
		for (int i = 0; i < N; i++) {
			if (!(m & (1 << i))) continue;

			int64_t k = dp[m*N+i];
			assert(k <= 0);
			if (k == -INF) continue;

			// now, delete as we go
			while (true) {
				if (hulls[i].empty()) {
					hulls[i].push_back({-1, {rate, k}});
					break;
				}
				auto [cut, b] = hulls[i].back();
				assert(rate >= b.first);
				if (b.first == rate) {
					if (b.second >= k) {
						// we're always worse, just give up
						break;
					} else {
						hulls[i].pop_back();
						continue;
					}
				}
				assert(rate > b.first);
				if (k >= b.second) {
					// we're always better
					hulls[i].pop_back();
					continue;
				}
				int64_t cutoff2 = (b.second - k) / (rate - b.first);
				if (cutoff2 <= cut) {
					hulls[i].pop_back();
					continue;
				} else {
					hulls[i].push_back({cutoff2, {rate, k}});
					break;
				}
				assert(false);
			}
		}
	}

	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		int64_t t; int en; cin >> t >> en; en--;
		int mi = 0;
		int ma = int(hulls[en].size());
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if (t > hulls[en][md].first) mi = md;
			else ma = md;
		}
		auto [rate, k] = hulls[en][mi].second;
		cout << rate * t + k << '\n';
	}

	return 0;
}
