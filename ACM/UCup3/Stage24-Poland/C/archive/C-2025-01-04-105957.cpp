#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int64_t> H(N);
	for (auto& h : H) cin >> h;
	auto check = [&](int64_t T) -> bool {
		struct range_t {
			int64_t l, r;
		};
		bool needs_help = false;
		range_t r1{T, T};
		range_t r2{T, T}; // only when needs_help = false
		for (int i = 0; i < N; i++) {
			if (needs_help) {
				assert(r1.l < r1.r);
				int64_t needs_extra = H[i] - r1.l;
				if (needs_extra > 0) {
					if (needs_extra > r1.l) return false;
					needs_help = true;
					r1 = {r1.l - needs_extra, r1.l};
				} else {
					// we're done in time
					needs_help = false;
					r2 = {r1.r, T};
					r1 = {H[i], r1.l};
				}
			} else {
				assert(r1.l <= r1.r);
				assert(r1.r <= r2.l);
				assert(r2.l <= r2.r);
				int64_t needs_extra = H[i] - T - (r1.r - r1.l) - (r2.r - r2.l);
				if (needs_extra > 0) {
					if (needs_extra > T) return false;
					needs_help = true;
					r1 = {T - needs_extra, T};
				} else {
					// find the first time we're free
					range_t nr1;
					needs_help = false;
					int64_t need = H[i];
					if (need <= r1.l) {
						nr1 = {need, T};
					} else {
						need -= r1.l;
						if (need <= (r1.r - r1.l) * 2) {
							nr1 = {r1.l + (need + 1) / 2, T};
						} else {
							need -= (r1.r - r1.l) * 2;
							if (need <= (r2.l - r1.r)) {
								nr1 = {r1.r + need, T};
							} else {
								need -= (r2.l - r1.r);
								if (need <= (r2.r - r2.l) * 2) {
									nr1 = {r2.l + (need + 1) / 2, T};
								} else {
									need -= (r2.r - r2.l) * 2;
									assert(need <= T - r2.r);
									nr1 = {r2.r + need, T};
								}
							}
						}
					}
					r1 = nr1;
					r2 = {T, T};
				}
			}
		}
		return !needs_help;
	};

	int64_t mi = 0;
	int64_t ma = *std::max_element(H.begin(), H.end());
	while (ma - mi > 1) {
		int64_t md = mi + (ma - mi) / 2;
		if (check(md)) {
			ma = md;
		} else {
			mi = md;
		}
	}
	cout << ma << '\n';

	return 0;
}
