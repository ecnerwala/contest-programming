#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> D(N);
		for (auto& d : D) cin >> d;
		std::vector<std::array<int, 2>> edges; edges.reserve(N);
		auto solve = [&]() -> bool {
			if (std::accumulate(D.begin(), D.end(), int64_t(0)) != 2 * N) {
				return false;
			}
			if (*std::min_element(D.begin(), D.end()) == 2) {
				for (int i = 0; i < N; i++) {
					assert(D[i] == 2);
				}
				for (int i = 0; i < N; i++) {
					edges.push_back({i, (i+1)%N});
				}
				return true;
			}

			// we have some leaves and some paths which need to get consumed
		};

		if (solve()) {
			cout << "Yes" << '\n';
			assert(int(edges.size()) == N);
			for (auto [u, v] : edges) {
				assert(u != v);
				cout << u+1 << ' ' << v+1 << '\n';
			}
		} else {
			cout << "No" << '\n';
		}
	}

	return 0;
}
