#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int CASES; std::cin >> CASES;
	while (CASES--) {
		int N; std::cin >> N;
		std::string S; std::cin >> S; assert(int(S.size()) == N);
		std::vector<std::array<int, 2>> locs; locs.reserve(N+1);
		locs.push_back({0, 0});
		for (char c : S) {
			locs.push_back(locs.back());
			if (c == 'U') {
				locs.back()[1]++;
			} else if (c == 'D') {
				locs.back()[1]--;
			} else if (c == 'L') {
				locs.back()[0]--;
			} else if (c == 'R') {
				locs.back()[0]++;
			} else assert(false);
		}
		// final length <= N+1
		std::vector<std::vector<std::pair<int, int64_t>>> q(N+2);
		int max_L = 0;
		std::map<std::array<int, 2>, int> visit_time;
		for (int t = 0; t <= N; t++) {
			auto it = visit_time.find(locs[t]);
			int prv_t;
			if (it == visit_time.end()) {
				prv_t = -1;
				visit_time.insert({locs[t], t});
			} else {
				prv_t = it->second;
				it->second = t;
			}
			if (t == 0) {
				max_L = 1;
				q[1].push_back({1, -1});
			} else {
				{
					int max_gap = t - prv_t;
					assert(max_gap > 0);
					while (max_L > max_gap) {
						q[max_L].clear();
						max_L --;
					}
					assert(max_L <= max_gap);
				}
				// if we do take a step
				for (int l = max_L; l >= 1; l--) {
					int mi = -1, ma = int(q[l].size());
					while (ma - mi > 1) {
						int md = (mi + ma) / 2;
						if (q[l][md].second >= prv_t) {
							ma = md;
						} else {
							mi = md;
						}
					}
					if (ma == int(q[l].size())) continue;
					int v = q[l][ma].first + (l+1);
					std::cerr << t << ' ' << l+1 << ' ' << v << '\n';
					while (!q[l+1].empty() && v >= q[l+1].back().first) q[l+1].pop_back();
					q[l+1].push_back({v, t - (l+1)});
					max_L = std::max(max_L, l+1);
					assert(max_L <= t - prv_t);
				}
			}
		}
		int ans = 0;
		for (int i = 1; i <= max_L; i++) {
			ans = std::max(ans, q[i].front().first);
		}
		std::cout << ans << '\n';
	}

	return 0;
}

// Each apple must be empty at the previous apple time
