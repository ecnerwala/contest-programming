#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		int M; cin >> M;
		vector<int> C(N); for (int i = 0; i < N; i++) cin >> C[i];

		auto solve = [&]() -> int64_t {
			deque<pair<int64_t, int>> tank;
			tank.emplace_back(0, 0);

			for (int i = 1; i < N; i++) {
				while (!tank.empty() && i - tank.front().second > M) {
					tank.pop_front();
				}
				if (tank.empty()) return -1;
				if (i == N-1) {
					return tank.front().first;
				}
				if (C[i]) {
					int64_t cnd = tank.front().first + C[i];
					while (!tank.empty() && tank.back().first >= cnd) tank.pop_back();
					tank.emplace_back(cnd, i);
				}
			}
			assert(false);
		};

		auto ans = solve();
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
