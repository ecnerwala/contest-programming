#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> B(N); for (auto& b : B) cin >> b;
		int last_bad = -1;
		int res = 1;
		for (int i = 0; i+1 < N; i++) {
			if (B[i+1] % B[i] != 0) {
				if (last_bad != -1) {
					int must_mult = B[i] / std::gcd(B[i], B[i+1]);
					res = std::lcm(res, must_mult);
				}
				last_bad = i;
			}
		}
		cout << res << '\n';
	}

	return 0;
}
