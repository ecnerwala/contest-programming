#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, L, C; cin >> N >> L >> C;
		std::vector<int> X(N);
		for (auto& x : X) cin >> x;
		std::vector<int> K(N);
		for (auto& k : K) { cin >> k; k--; }
		auto solve = [&]() -> int {
		std::array<int, 2> cur_cap{C, C};
			int cur_pos = 0;
			for (int i = 0; i < N; i++) {
				// burn the fuel we can get first
				int nxt_pos = X[i];
				int d = nxt_pos - cur_pos;
				int vk = std::min(d, cur_cap[K[i]]);
				cur_cap[K[i]] -= vk;
				d -= vk;
				int vnk = std::min(d, cur_cap[!K[i]]);
				cur_cap[!K[i]] -= vnk;
				d -= vnk;
				if (d > 0) {
					// out of fuel!
					return -1;
				}
				cur_cap[K[i]] = C;
				cur_pos = nxt_pos;
			}
			return 0;
		};
		cout << solve() << '\n';
	}

	return 0;
}
