#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 2>> pts(N);
		for (auto& [x, y] : pts) cin >> x >> y;
		std::sort(pts.begin(), pts.end());
		[&]() -> void {
			using ld = long double;
			auto query = [&](int numer, int denom) -> ld {
				assert(denom == 1 || denom == 2);
				if (denom == 2 && numer % 2 == 0) {
					numer /= 2;
					denom /= 2;
				}
				cout << '?' << ' ' << numer << ' ' << denom << '\n' << flush;
				std::string sp, sq; cin >> sp >> sq;
				std::reverse(sp.begin(), sp.end());
				ld p = 0;
				for (char c : sp) {
					assert('0' <= c && c <= '9');
					p /= 10;
					p += (c - '0');
				}
				std::reverse(sq.begin(), sq.end());
				ld q = 0;
				for (char c : sq) {
					assert('0' <= c && c <= '9');
					q /= 10;
					q += (c - '0');
				}
				ld r = p / q;
				r *= pow(10, int(sp.size()) - int(sq.size()));
				return r;
			};

			bool has_eq = false;
			for (int i = 0; i+1 < N; i++) {
				if (pts[i][0] == pts[i+1][0]) {
					has_eq = true;
					break;
				}
			}
			ld area = 0;
			if (has_eq) {
				for (int i = 0; i+1 < N; i++) {
					if (pts[i][0] != pts[i+1][0]) {
						int numer = pts[i][0] + pts[i+1][0];
						int denom = 2;
						ld r = query(numer, denom);
						area += r * ld(pts[i+1][0] - pts[i][0]);
					}
				}
			} else {
				std::vector<ld> heights(N, 0);
				for (int i = 1; i+1 < N; i++) {
					heights[i] = query(pts[i][0], 1);
				}
				for (int i = 0; i+1 < N; i++) {
					ld r = (heights[i] + heights[i+1]) / 2;
					area += r * ld(pts[i+1][0] - pts[i][0]);
				}
			}
			int64_t numer = int64_t(area * 2 + 0.5), denom = 2;
			if (numer % 2 == 0) { numer /= 2,  denom /= 2; }
			cout << '!' << ' ' << numer << ' ' << denom << '\n' << flush;
		}();
	}

	return 0;
}
