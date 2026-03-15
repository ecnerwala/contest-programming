#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
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
		std::map<std::array<int, 2>, int> visit_time;
		std::vector<int> max_L(N+1);
		std::iota(max_L.begin(), max_L.end(), 1);
		for (int t = 0; t <= T; t++) {
			auto it = visit_time.find(locs[t]);
			if (it == visit_time.end()) {
				visit_time.insert({locs[t], t});
			} else {
				max_L[t] = t - it->second;
				it->second = t;
			}
		}
		// you can grow by at most 1 every time
		for (int i = N; i > 0; i--) {
			max_L[i-1] = std::min(max_L[i-1], max_L[i]);
		}
		for (int i = 0; i < N; i++) {
			max_L[i+1] = std::min(max_L[i+1], max_L[i]+1);
		}
		for (auto l : max_L) { std::cerr << l << ' '; } std::cerr << '\n';
		std::cout << std::accumulate(max_L.begin(), max_L.end(), int64_t(0)) << '\n';
	}

	return 0;
}
