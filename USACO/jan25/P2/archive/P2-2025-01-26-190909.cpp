#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto ceil_div = [](int64_t a, int64_t b) -> int64_t { return (a + b - 1) / b; };

	auto is_poss_ends = [&](const std::vector<int64_t>& V, int64_t ops) -> bool {
		int N = int(V.size());
		int64_t l_lo = 0;
		int64_t r_lo = 0;
		for (int i = 0; i < N; i++) {
			if (V[i] <= 0) continue;
			int w_l = i;
			int w_r = N-1-i;
			int64_t v_smaller = std::min(w_l, w_r) == 0 ? ops + 1 : ceil_div(V[i], std::min(w_l, w_r));
			if (ops >= v_smaller) {
				// always satisfied
				continue;
			}
			assert(ops < v_smaller);
			int64_t need_bigger = V[i] - ops * std::min(w_l, w_r);
			assert(need_bigger > 0);
			if (w_l == w_r) {
				return false;
			} else if (w_l > w_r) {
				l_lo = std::max(l_lo, ceil_div(need_bigger, w_l - w_r));
			} else if (w_r > w_l) {
				r_lo = std::max(r_lo, ceil_div(need_bigger, w_r - w_l));
			} else assert(false);
		}
		return l_lo + r_lo <= ops;
	};


	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> P(N);
		for (auto& p : P) cin >> p;

		int64_t mi = -1;
		int64_t ma = 2 * *std::max_element(P.begin(), P.end());
		while (ma - mi > 1) {
			int64_t md = mi + (ma - mi) / 2;
			if (is_poss_ends(P, md)) {
				ma = md;
			} else {
				mi = md;
			}
		}
		bool can_do = false;
		if (ma >= 2) {
			// check ma - 1 by using something in the middle
			for (int i = 0; i < N; i++) {
				std::vector<int64_t> tmp = P;
				for (int z = 0; z < N; z++) {
					tmp[z] -= std::abs(z - i);
				}
				if (is_poss_ends(tmp, ma-2)) {
					can_do = true;
					break;
				}
			}
		}
		if (can_do) ma--;
		cout << ma << '\n';
	}

	return 0;
}
