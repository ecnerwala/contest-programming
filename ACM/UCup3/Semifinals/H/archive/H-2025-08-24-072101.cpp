#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N);
		P[0] = -1;
		for (int i = 1; i < N; i++) { cin >> P[i]; P[i]--; }

		std::vector<std::array<int, 2>> ops; ops.reserve(N);
		std::vector<bool> has(N, false);
		for (int i = N-1; i > 0; i--) {
			if (has[i]) {
				ops.push_back({0, i});
			} else {
				ops.push_back({i, 0});
				has[P[i]] = true;
			}
		}

		cout << "Yes" << '\n';
		cout << ops.size() << '\n';
		for (auto [u, v] : ops) {
			cout << u+1 << ' ' << v+1 << '\n';
		}
	}

	return 0;
}
