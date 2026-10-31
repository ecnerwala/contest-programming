#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, K; cin >> N >> K;

		//cerr << "Case #" << case_num << ": " << N << " " << K << '\n';

		cout << "Case #" << case_num << ": ";
		assert(N % 2 == 1);
		if ((K & 1) || K < N-1) {
			cout << "IMPOSSIBLE" << '\n';
			continue;
		}

		int extra = N * N - 1 - K;

		std::vector<std::pair<int, int>> shortcuts;

		int x = 1, y = 4 * (N-1);
		int iter = 0;
		auto advance = [&]() {
			x += (2 * N - iter) / 2;
			y += (2 * N - iter) / 2 - 2;
			iter++;
			//cerr << x << ' ' << y << ' ' << iter << ' ' << (y-x-1) / 2 << ' ' << '\n';
		};

		// 5, 4, 4, 3, 3, 2, 2, 1, 1
		// 3, 2, 2, 1, 1, 0, 0, 
		//cerr << extra << '\n';
		for (int save = 4 * (N-1) - 2; save > 0; save -= 2, advance()) {
			assert(save == (y-x-1));
			if (extra >= save) {
				extra -= save;
				shortcuts.push_back({x, y});
				//cerr << "save" << ' ' << save << '\n';
				// advance by 3 extra
				advance();
				advance();
				advance();
				save -= 6;
			}
		}
		assert(extra == 0);

		cout << shortcuts.size() << '\n';
		for (auto [x, y] : shortcuts) {
			cout << x << ' ' << y << '\n';
		}

	}

	return 0;
}
