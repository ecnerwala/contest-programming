#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> P(N);
		for (auto& p : P) cin >> p;

		auto ceil_div = [](int64_t a, int64_t b) -> int64_t { return (a + b - 1) / b; };

		auto is_poss_ends = [&](int64_t ops) -> bool {
			int64_t l_lo = 0;
			int64_t r_lo = 0;
			for (int i = 0; i < N; i++) {
				if (P[i] == 0) continue;
				int w_l = i;
				int w_r = N-1-i;
				int64_t v_smaller = std::min(w_l, w_r) == 0 ? ops + 1 : ceil_div(P[i], std::min(w_l, w_r));
				if (ops >= v_smaller) {
					// always satisfied
					continue;
				}
				assert(ops < v_smaller);
				int64_t need_bigger = P[i] - ops * std::min(w_l, w_r);
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

		int64_t mi = -1;
		int64_t ma = 2 * *std::max_element(P.begin(), P.end());
		while (ma - mi > 1) {
			int64_t md = mi + (ma - mi) / 2;
			if (is_poss_ends(md)) {
				ma = md;
			} else {
				mi = md;
			}
		}
		if (ma >= 2) {
			// check ma - 1 by using something in the middle
			int64_t ops = ma-2;
			const int LG = 31 - __builtin_clz(N);
			assert((1 << LG) <= N);
			assert((2 << LG) > N);
			std::vector<std::vector<int64_t>> l_lo(LG+1, std::vector<int64_t>(N, 0));
			std::vector<std::vector<int64_t>> r_lo(LG+1, std::vector<int64_t>(N, 0));
			for (int i = 0; i < N; i++) {
				if (P[i] == 0) continue;
				int w_l = i;
				int w_r = N-1-i;
				int64_t v_smaller = std::min(w_l, w_r) == 0 ? ops + 1 : ceil_div(P[i], std::min(w_l, w_r));
				if (ops >= v_smaller) {
					// always satisfied
					continue;
				}
				assert(ops < v_smaller);
				int64_t need_bigger = P[i] - ops * std::min(w_l, w_r);
				assert(need_bigger > 0);
				int64_t delta = std::abs(w_l - w_r);
				if (w_l == w_r) {
					// we need abs(x - i) >= need_bigger
					for (int x = 0; x < N; x++) {
						if (std::abs(x - i) < need_bigger) l_lo[x] = ops + 1;
					}
				} else {
					// we need abs(x - i) + cnt * delta >= need_bigger
					for (int64_t cnt = ceil_div(need_bigger, delta); true; cnt--) {
						int64_t min_abs = (cnt - 1) * delta - need_bigger;
						assert(min_abs > 0);
						// everything in +- min_abs exclusive must be at least cnt
						int64_t lo = i - min_abs + 1;
						int64_t hi = i + min_abs - 1;
						if (lo < 0) lo = 0;
						if (hi >= N) hi = N-1;
						assert(lo <= hi);
						hi++;
						int len = int(hi - lo);
						int l = 31 - __builtin_clz(len);
						if (w_l > w_r) {
							setmax(l_lo[l][lo], cnt);
							setmax(l_lo[l][hi - (1 << l)], cnt);
						} else {
							setmax(r_lo[l][lo], cnt);
							setmax(r_lo[l][hi - (1 << l)], cnt);
						}
					}
				} assert(false);
			}
			for (int l = LG; l > 0; l--) {
				for (int i = 0; i + (1 << l) <= N; i++) {
					setmax(l_lo[l-1][i], l_lo[l][i]);
					setmax(l_lo[l-1][i + (1 << (l-1))], l_lo[l][i]);
				}
			}
			bool can_do = false;
			for (int x = 0; x < N; x++) {
				if (l_lo[0][x] + r_lo[0][x] <= ops) {
					can_do = true;
					break;
				}
			}
			if (can_do) ma--;
		}
		cout << ma << '\n';
	}

	return 0;
}
