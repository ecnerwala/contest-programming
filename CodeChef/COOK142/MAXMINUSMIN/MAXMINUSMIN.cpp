#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t A, B, C, K; cin >> A >> B >> C >> K;
		K--;

		auto do_step = [&]() -> void {
			assert(K > 0);
			int64_t v = std::max({A, B, C}) - std::min({A, B, C});
			A = B;
			B = C;
			C = v;
			K--;
		};
		while (K > 0) {
			if (A >= B && B >= A - C && B - 3 * C >= C && K >= 8) {
				int64_t max_jump = K / 8;
				if (C > 0) max_jump = std::min(max_jump, (B - C) / (3 * C));
				assert(max_jump >= 1);
				K -= 8 * max_jump;
				A -= 3 * C * max_jump;
				B -= 3 * C * max_jump;
			} else {
				do_step();
			}
		}
		cout << A << '\n';
	}

	return 0;
}
