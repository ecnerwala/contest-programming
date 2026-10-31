#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 3>> X(N);
		std::array<std::vector<pair<int, int>>, 3> P;
		for (int z = 0; z < 3; z++) {
			P[z].resize(N);
		}
		for (int i = 0; i < N; i++) {
			for (int z = 0; z < 3; z++) {
				int v; cin >> v;
				X[i][z] = v;
				P[z][i] = {v, i};
			}
		}

		for (int z = 0; z < 3; z++) {
			std::sort(P[z].begin(), P[z].end());
		}

		int ans = 0;
		std::vector<bool> used(N);
		std::array<decltype(P[0].begin()), 3> cur{P[0].begin(), P[1].begin(), P[2].begin()};
		for (int iter = 0; iter < N; iter++) {
			std::array<int, 3> coords;
			for (int z = 0; z < 3; z++) {
				while (used[cur[z]->second]) cur[z]++;
				coords[z] = cur[z]->first;
			}
			std::pair<int, int> best{-1, -1};
			for (int z = 0; z < 3; z++) {
				int i = cur[z]->second;
				int v = 0;
				for (int d = 0; d < 3; d++) {
					v = max(v, X[i][d] - coords[d]);
				}
				if (z == 0 || v < best.first) best = {v, i};
			}
			ans = max(ans, best.first);
			used[best.second] = true;
		}

		ans = (ans+1)/2;
		cout << ans << '\n';
	}

	return 0;
}
