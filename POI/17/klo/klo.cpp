#include<bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);
	int N, Q; std::cin >> N >> Q;
	std::vector<int> X(N); for (auto& x : X) std::cin >> x;

	std::vector<std::pair<int64_t, int>> los; los.reserve(N+1);
	std::vector<std::pair<int64_t, int>> his; his.reserve(N);

	for (int q = 0; q < Q; q++) {
		int K; std::cin >> K;

		int64_t pref = 0;
		los.clear(), his.clear();
		los.push_back({pref, -1});
		for (int i = 0; i < N; i++) {
			pref += X[i] - K;
			if (pref < los.back().first) {
				los.push_back({pref, i});
			} else {
				while (!his.empty() && pref >= his.back().first) {
					his.pop_back();
				}
				his.push_back({pref, i});
			}
		}

		int ans = 0;
		for (int i = 0, j = 0; j < int(his.size()); j++) {
			while (los[i].first > his[j].first) i++;
			ans = std::max(ans, his[j].second - los[i].second);
		}

		std::cout << ans << " \n"[q+1==Q];
	}

	return 0;
}
