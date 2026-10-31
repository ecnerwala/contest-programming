#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int64_t A, B, C; cin >> A >> B >> C;

		auto solve = [&]() -> int64_t {
			if (C < A && C < B) return 0;
			if (2 * A <= B) {
				return C / A;
			}
			if (B <= A) {
				assert(C >= B);
				return (C / B) * 2 - 1;
			}
			assert(C >= A);
			return 1 + ((C - A) / B) * 2 + ((C - A) % B >= A);
		};
		int64_t ans = solve();
		assert(ans >= 0);

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
