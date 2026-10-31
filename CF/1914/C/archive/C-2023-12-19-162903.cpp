#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::vector<int> A(N); for (auto& a : A) { cin >> a; }
		int best_b = 0;
		int64_t pref_a = 0;
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			pref_a += A[i];
			int b; cin >> b;
			best_b = std::max(best_b, b);
			if (i+1 <= K) ans = std::max(ans, pref_a + int64_t(K-(i+1)) * best_b);
		}
		cout << ans << '\n';
	}

	return 0;
}
