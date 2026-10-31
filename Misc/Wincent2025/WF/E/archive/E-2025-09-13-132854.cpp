#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		cerr << "solve " << N << '\n';
		std::mt19937 mt(48);

		std::vector<std::array<int, 2>> freq(2*N, {0, 0});
		std::vector<int64_t> vals(2*N, 0);
		std::set<int64_t> seen(2*N, 0);
		auto res = [&](this auto&& self, int idx) -> bool {
			if (N > 40) return false;
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
			int64_t val = int(mt() & 1);
			for (int z = 0; z < 2; z++, val = !val) {
				vals[0+i] += (val << j);
				vals[N+j] += (val << i);

				freq[0+i][val]++;
				freq[N+j][val]++;

				if (freq[0+i][val] <= (N+1)/2 && freq[N+j][val] <= (N+1)/2) {
					if (!(j >= 2 && ((vals[0+i] >> (j-1)) & 1) == val && ((vals[0+i] >> (j-2)) & 1) == val)) {
						if (!(i >= 2 && ((vals[N+j] >> (i-1)) & 1) == val && ((vals[N+j] >> (i-2)) & 1) == val)) {
							if ([&]() -> bool {
								if (j+1 == N) {
									if (seen.count(vals[0+i])) {
										return false;
									}
									seen.insert(vals[0+i]);
								}

								if ([&]() -> bool {
									if (i+1 == N) {
										if (seen.count(vals[N+j])) return false;
										seen.insert(vals[N+j]);
									}
									if (self(idx+1)) {
										return true;
									}
									if (i+1==N) {
										seen.erase(vals[N+j]);
									}
									return false;
								}()) return true;
								if (j+1 == N) {
									seen.erase(vals[0+i]);
								}
								return false;
							}) return true;
									seen.insert(vals[0+i]);
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
