#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	while (true) {
		int64_t A, B;
		cin >> A >> B;
		if (A == -1 && B == -1) break;

		// half-open interval
		B++;
		auto get_zeros = [&](int64_t N) -> int {
			if (N == 0) return 1;
			int res = 0;
			for (; N; N /= 10) res += (N%10 == 0);
			return res;
		};

		int64_t ans = 0;
		for (int64_t mult = 1; A < B; mult *= 10) {
			while (A < B && (A == 0 || A % 10)) {
				ans += mult * get_zeros(A);
				A++;
			}
			while (A < B && B % 10) {
				--B;
				ans += mult * get_zeros(B);
			}

			if (A == B) continue;

			ans += mult * ((B - A) / 10);

			A /= 10;
			B /= 10;
		}

		cout << ans << '\n';
	}

	return 0;
}
