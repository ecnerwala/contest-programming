#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int64_t L, R; cin >> L >> R; R++;
		int64_t coeff = 1;
		int64_t ans = 0;
		while (L < R) {
			assert(1 <= L && L < R);
			auto is_good = [&](int64_t v) {
				assert(v > 0);
				bool d = v % 2;
				while (v > 0) {
					if (v % 2 != d) return false;
					d = !d;
					v /= 10;
				}
				return d == 0;
			};
			while (L < R && L % 10 != 0) {
				if (is_good(L)) {
					ans += coeff;
				}
				L++;
			}
			while (L < R && R % 10 != 0) {
				--R;
				if (is_good(R)) {
					ans += coeff;
				}
			}

			if (L == R) break;
			assert(L % 10 == 0 && R % 10 == 0);
			assert(L >= 10);

			L /= 10;
			R /= 10;

			coeff *= 5;
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
