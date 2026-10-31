#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		cerr << "sovle " << N << '\n';
		std::mt19937 mt(48);

		std::vector<std::array<int, 2>> freq(2*N, {0, 0});
		std::vector<int> vals(2*N, 0);
		auto res = [&](this auto&& self, int idx) -> bool {
			if (N > 13) return false;
			if (idx == N * N) {
				auto v = vals;
				std::sort(v.begin(), v.end());
				if (std::unique(v.begin(), v.end()) == v.end()) {
					cerr << "done" << '\n';
					return true;
				}
				return false;
			}
			int i = idx / N, j = idx % N;
			int val = int(mt() & 1);
			for (int z = 0; z < 2; z++, val = !val) {
				vals[0+i] += (val << j);
				vals[N+j] += (val << i);

				freq[0+i][val]++;
				freq[N+j][val]++;

				if (freq[0+i][val] <= (N+1)/2 && freq[N+j][val] <= (N+1)/2) {
					if (!(j >= 2 && ((vals[0+i] >> (j-1)) & 1) == val && ((vals[0+i] >> (j-2)) & 1) == val)) {
						if (!(i >= 2 && ((vals[N+j] >> (i-1)) & 1) == val && ((vals[N+j] >> (i-2)) & 1) == val)) {
							if (self(idx+1)) {
								return true;
							}
						}
					}
				} 

				freq[0+i][val]--;
				freq[N+j][val]--;

				vals[0+i] -= (val << j);
				vals[N+j] -= (val << i);
			}
			return false;
		}(0);
		if (res) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					cerr << ((vals[i] >> j) & 1);
				}
				cerr << '\n';
			}
			cout << "POSSIBLE" << '\n';
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					cout << ((vals[i] >> j) & 1);
				}
				cout << '\n';
			}
		} else {
			cout << "IMPOSSIBLE" << '\n';
		}
	}

	return 0;
}
