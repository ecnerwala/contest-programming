#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int64_t M; cin >> M;
		std::vector<int64_t> A(N); for (auto& a	: A) cin >> a;
		int64_t mx = *std::max_element(A.begin(), A.end());
		int64_t ans = mx * 2;
		for (int64_t v : A) {
			if (v < mx) ans = std::max(ans, 2 * v + M);
		}
		cout << ans << '\n';
		// If we have hi > lo, then we can get
		// >= lo + hi + M - (hi - lo) = 2 * lo + M
		// if hi - lo >= M, then lo + hi + (lo - hi) % M is worse than 2 * hi
	}

	return 0;
}
