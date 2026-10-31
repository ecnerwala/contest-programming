#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto sq_hypot = [](int x, int y) -> int64_t { return x * x + y * y; };

	int T; cin >> T;
	while (T--) {
		int xt, yt, xo, yo, sb, sp; cin >> xt >> yt >> xo >> yo >> sb >> sp;
		auto ans = [&]() -> char {
			if (sb > sp) return '+';
			if (xt * yo - yt * xo == 0 && xt * xo + yt * yo > 0) {
				// collinear
				if (xt * xt + yt * yt < xo * xo + yo * yo) {
					return '+';
				} else {
					return '-';
				}
			}
			// guaranteed not collinear
			if (sb >= sp) return '+';
			assert(sb < sp);
			// otherwise
			int64_t a = sq_hypot(xt, yt) * sp * sp;
			int64_t b = sq_hypot(xo, yo) * sp * sp;
			int64_t c = sq_hypot(xo - xt, yo - yt) * sb * sb;
			// if sqrt(a) > sqrt(b) + sqrt(c) return '-'
			if (a < b + c) {
				return '+';
			}
			if ((a - b - c) * (a - b - c) > 4 * b * c) {
				return '-';
			} else if ((a - b - c) * (a - b - c) == 4 * b * c) {
				return '0';
			} else {
				return '+';
			}
		}();
		cout << ans;
	}
	cout << '\n';

	return 0;
}
