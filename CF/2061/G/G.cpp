#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		if (N == -1) exit(0);
		int K = (N+1) / 3;
		cout << K << '\n' << flush;
		int cur_color = 0;
		std::vector<int> stk; stk.reserve(N);
		std::vector<std::pair<int, std::array<int, 2>>> trips; trips.reserve(N / 3);
		for (int i = 0; i < N; i++) {
			if (stk.empty()) {
				stk.push_back(i);
			} else {
				cout << '?' << ' ' << stk.back()+1 << ' ' << i+1 << '\n' << flush;
				int res; cin >> res;
				if (res == -1) exit(0);
				if (stk.size() == 1) cur_color = res;
				stk.push_back(i);
				if (res != cur_color) {
					assert(int(stk.size()) >= 3);
					std::array<int, 2> opts;
					opts[cur_color] = stk.end()[-3];
					opts[res] = stk.end()[-1];
					trips.push_back({stk.end()[-2], opts});
					stk.resize(int(stk.size()) - 3);
				}
			}
		}
		while (int(trips.size()) < K) {
			assert(int(stk.size()) >= 2);
			std::array<int, 2> opts{-1, -1};
			opts[cur_color] = stk.end()[-1];
			trips.push_back({stk.end()[-2], opts});
			stk.resize(int(stk.size()) - 2);
		}
		cout << '!';
		for (int k = 0; k < K; k++) {
			cout << ' ' << trips[k].first+1 << ' ' << trips[k].second[cur_color]+1;
		}
		cout << '\n' << flush;
	}

	return 0;
}
