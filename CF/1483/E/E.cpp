#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t V = 1;
		int64_t M_lo = 0;
		int64_t M_hi = int64_t(1e14)+1;

		// returns true if lucky
		auto query = [&](int64_t x) -> bool {
			assert(1 <= x && x <= int64_t(1e14));
			assert(x <= V || x <= M_lo);
			cout << '?' << ' ' << x << '\n' << flush;
			string resp; cin >> resp;
			if (resp == "Lucky!") {
				V += x;
				M_lo = max(M_lo, x);
				return true;
			} else if (resp == "Fraudster!") {
				V -= x;
				M_hi = min(M_hi, x);
				return false;
			} else if (resp == "Fired!") {
				exit(0);
			} else assert(false);
		};

		auto solve = [&]() -> void {
			if (!query(1)) return;

			assert(M_lo == 1);

			while (M_hi > 2 * M_lo) {
				assert(V == 2 * M_lo);
				query(V);
			}

			// TODO: split by 3x every 2 queries?
			// Very roughly, yes answers give you free no answers
			// The hard part is building up your balance enough to ask the questions you want
			while (M_hi - M_lo > 1) {
				int64_t diff = M_hi - M_lo;
				int64_t bank = V / M_lo;
				double frac = 0.5 * (1 - 1. / (2 + bank));
				int64_t md = M_lo + int64_t(diff * frac);
				if (V < md) {
					query(max(M_lo, min(V, M_lo+1)));
				} else {
					query(md);
				}
			}
		};

		solve();

		cout << '!' << ' ' << M_lo << '\n' << flush;
	}

	return 0;
}

// V = 1, M >= 0
//  X = 1 -> V = 0 and M = 0 or V = 2 and M >= 1
// V = 2, M >= 1
//  X = 2 -> V = 0 and M = 1 or V = 4 and M >= 2
// V = 4, M >= 2
//  X = 3 -> V = 1 and M = 2 or V = 7 and M >= 3
// V = 7, M >= 3
