#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> C(N);
		for (auto& c : C) cin >> c;
		std::vector<std::pair<int, int>> locs(N);
		for (int i = 0; i < N; i++) {
			locs[i] = {C[i], i};
		}
		std::sort(locs.begin(), locs.end());

		std::vector<std::vector<std::pair<int, int>>> starts(N);
		int K = 0;
		for (int l = 0, r = 0; l < N; l = r) {
			int c = locs[l].first;
			while (r < N && locs[r].first == c) r++;
			int cur_x = N;
			int balance = 0;
			int st = N;
			for (int i = r-1; i >= l; i--) {
				int x = locs[i].second;
				if (balance > 0 && balance - (cur_x - (x+1)) <= 0) {
					starts[cur_x - balance].push_back({c, st});
				}
				balance -= cur_x - (x+1);
				if (balance <= 0) {
					balance = 0;
					st = x+1;
				}
				balance++; cur_x++;
				K = std::max(K, balance);
			}
			if (balance > 0) {
				// TODO: fixme
			}
		}
		cout << K << '\n';
		for (int i = 0; i < K-1; i++) {
			cout << 1 << ' ' << i+1 << '\n';
		}
		cout << N-(K-1); for (int i = K-1; i < N; i++) { cout << ' ' << i+1; } cout << '\n';
	}

	return 0;
}
