#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int Q; cin >> Q;
		// lo is exclusive
		int64_t H_lo = 0;
		int64_t H_hi = 8e18;
		for (int q = 0; q < Q; q++) {
			int op; cin >> op;
			if (op == 1) {
				int64_t a, b, n; cin >> a >> b >> n;
				int64_t cur_lo = (n-1 == 0 ? 0 : (n-2) * (a-b) + a);
				int64_t cur_hi = (n-1) * (a-b) + a;
				if (std::max(H_lo, cur_lo) < std::min(H_hi, cur_hi)) {
					cout << 1 << " \n"[q+1==Q];
					H_lo = std::max(H_lo, cur_lo);
					H_hi = std::min(H_hi, cur_hi);
				} else {
					cout << 0 << " \n"[q+1==Q];
				}
			} else if (op == 2) {
				int64_t a, b; cin >> a >> b;
				auto get_time = [&](int64_t h) -> int64_t {
					if (h <= a) return 1;
					return (h-a + (a-b-1)) / (a-b) + 1;
				};
				int64_t x = get_time(H_lo+1);
				int64_t y = get_time(H_hi);
				if (x < y) {
					cout << -1 << " \n"[q+1==Q];
				} else {
					cout << x << " \n"[q+1==Q];
				}
			} else assert(false);
		}
	}

	return 0;
}
