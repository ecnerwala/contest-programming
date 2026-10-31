#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto sq_hypot = [](int x, int y) -> int64_t { return x * x + y * y; };

	int T; cin >> T;
	while (T--) {
		int xt, yt, xo, yo, sb, sp; cin >> xt >> yt >> xo >> yo >> sb >> sp;
		auto ans = [&]() -> char {
			int64_t a = sq_hypot(xt, yt) * sp * sp;
			int64_t b = sq_hypot(xo, yo) * sp * sp;
			int64_t c = sq_hypot(xo - xt, yo - yt) * sb * sb;
			// if sqrt(a) > sqrt(b) + sqrt(c) return '-'
			a -= b + c;
			if (a < 0) {
				return '+';
			}
			int64_t d = a * a - 4 * b * c;
			if (d > 0) {
				return '-';
			} else if (d == 0) {
				return sb == sp ? '-' : '0';
			} else {
				return '+';
			}
		}();
		cout << ans;
	}
	cout << '\n';

	return 0;
}
