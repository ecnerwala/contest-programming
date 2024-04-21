#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int> cnt1(N);
		std::vector<std::array<int, 2>> v2; v2.reserve(M);
		std::vector<std::array<int, 3>> v3; v3.reserve(M);
		for (int e = 0; e < M; e++) {
			int u, v, w; cin >> u >> v >> w;
			u--, v--, w--;
			assert(u <= v && v <= w);
			if (u == w) {
				cnt1[u] ^= 1;
			} else if (u == v || v == w) {
				v2.push_back({u, w});
			} else {
				v3.push_back({u, v, w});
			}
		}
		std::vector<int> ans(N, 0);
		if ([&]() -> bool {
			for (int i = 0; i < N; i++) {
				if (cnt1[i] & 1) {
					ans[i] = 1;
					return true;
				}
			}
			std::sort(v2.begin(), v2.end());
			for (auto it = v2.begin(); it != v2.end(); ++it) {
				if (std::next(it) != v2.end() && *it == *(std::next(it))) {
					++it;
				} else {
					for (auto i : *it) {
						ans[i] = 1;
					}
					return true;
				}
			}
			std::sort(v3.begin(), v3.end());
			for (auto it = v3.begin(); it != v3.end(); ++it) {
				if (std::next(it) != v3.end() && *it == *(std::next(it))) {
					++it;
				} else {
					for (auto i : *it) {
						ans[i] = 1;
					}
					return true;
				}
			}
			return false;
		}()) {
			cout << "YES" << '\n';
			for (int i = 0; i < N; i++) {
				cout << ans[i] << " \n"[i+1==N];
			}
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
