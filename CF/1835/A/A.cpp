#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int A, B, C; cin >> A >> B >> C;

		int64_t K; cin >> K;

		auto pow_10 = [](int a) -> int64_t {
			int64_t r = 1;
			for (int z = 0; z < a; z++) r *= 10;
			return r;
		};

		int64_t A_lo = pow_10(A-1);
		int64_t A_hi = pow_10(A);
		int64_t B_lo = pow_10(B-1);
		int64_t B_hi = pow_10(B);
		int64_t C_lo = pow_10(C-1);
		int64_t C_hi = pow_10(C);

		[&]() -> void {
			for (int64_t a = A_lo; a < A_hi; a++) {
				int64_t lo = std::max(B_lo, C_lo - a);
				int64_t hi = std::min(B_hi, C_hi - a);
				if (lo < hi) {
					int64_t cnt = hi - lo;
					if (K > cnt) {
						K -= cnt;
						continue;
					}
					int64_t b = lo + K - 1;
					int64_t c = a + b;
					cout << a << " + " << b << " = " << c << '\n';
					return;
				}
			}
			cout << -1 << '\n';
		}();
	}

	return 0;
}
