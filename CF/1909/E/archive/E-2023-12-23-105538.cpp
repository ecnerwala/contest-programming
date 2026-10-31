#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::array<int, 2>> edges(M);
		for (auto& [u, v] : edges) cin >> u >> v;

		// If you press all buttons, the squares will be lit
		if (N >= 20) {
			cout << N << '\n';
			for (int i = 1; i <= N; i++) {
				cout << i << " \n"[i==N];
			}
			continue;
		}

		// Otherwise, brute force?
		assert(N / 5 <= 3);
		auto solve = [&]() -> int {
			std::vector<int> implies(N+1);
			for (auto [u, v] : edges) { implies[u] |= (1 << v); }
			auto check = [&](int seq) -> bool {
				for (int v = seq; v > 0; v &= (v-1)) {
					if (implies[__builtin_ctz(v)] & ~seq) {
						return false;
					}
				}
				return true;
			};
			int K = N / 5;
			std::vector<int> light_mask(N+1);
			for (int i = N; i >= 1; i--) {
				light_mask[i] = (1 << i);
				for (int j = 2*i; j <= N; j += i) {
					light_mask[i] ^= light_mask[j];
				}
			}
			if (K >= 1) {
				for (int i = N; i >= 1; i--) {
					int cnd = light_mask[i];
					if (check(cnd)) return cnd;
				}
			}
			if (K >= 2) {
				for (int i = N; i >= 1; i--) {
					for (int j = i-1; j >= 1; j--) {
						int cnd = light_mask[i] ^ light_mask[j];
						if (check(cnd)) return cnd;
					}
				}
			}
			if (K >= 3) {
				for (int i = N; i >= 1; i--) {
					for (int j = i-1; j >= 1; j--) {
						for (int k = j-1; k >= 1; k--) {
							int cnd = light_mask[i] ^ light_mask[j] ^ light_mask[k];
							if (check(cnd)) return cnd;
						}
					}
				}
			}
			return -1;
		};
		auto ans = solve();
		if (ans == -1) {
			cout << -1 << '\n';
		} else {
			cout << __builtin_popcount(ans) << '\n';
			for (int v = ans; v > 0; v &= (v-1)) {
				cout << __builtin_ctz(v) << " \n"[(v & (v-1)) == 0];
			}
		}
	}

	return 0;
}
