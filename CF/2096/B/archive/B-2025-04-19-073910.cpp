#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<std::array<int, 2>> C(N);
		for (int z = 0; z < 2; z++) {
			for (auto& c : C) {
				cin >> c[z];
			}
		}

		int64_t ans = 0;
		std::vector<int> V; V.reserve(N);
		for (auto& c : C) {
			V.push_back(std::min(c[0], c[1]));
			ans += std::max(c[0], c[1]);
		}
		std::nth_element(V.begin(), V.end() - (K-1), V.end());
		ans = std::accumulate(V.end() - (K-1), V.end(), ans);
		ans++;
		cout << ans << '\n';
	}

	return 0;
}
