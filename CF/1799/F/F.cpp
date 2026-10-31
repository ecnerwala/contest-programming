#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, B, K1, K2; cin >> N >> B >> K1 >> K2;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::sort(A.begin(), A.end());
		std::reverse(A.begin(), A.end());

		int64_t best = 0;

		std::vector<int64_t> pref_use_1(N+1);
		std::vector<int64_t> pref_use_2(N+1);
		std::vector<int64_t> pref_use_12(N+1);
		for (int i = 0; i < N; i++) {
			pref_use_1[i+1] = pref_use_1[i] + (A[i] / 2);
			pref_use_2[i+1] = pref_use_2[i] + std::min(A[i], B);
			pref_use_12[i+1] = pref_use_12[i] + std::min(A[i], (A[i] / 2) + B);
		}
		for (int c12 = 0; c12 <= K2 && c12 <= K1; c12++) {
			for (int c1 = c12; c1 <= K1; c1++) {
				int c2 = std::min(N, c1 + (K2 - c12));
				int c3 = std::min(N, c2 + (K1 - c1));
				int64_t cnd = 0;
				cnd += pref_use_12[c12] - pref_use_12[0];
				cnd += pref_use_1[c1] - pref_use_1[c12];
				cnd += pref_use_2[c2] - pref_use_2[c1];
				cnd += pref_use_1[c3] - pref_use_1[c2];
				best = std::max(best, cnd);
			}
		}

		int64_t ans = std::accumulate(A.begin(), A.end(), int64_t(0)) - best;
		cout << ans << '\n';
	}

	return 0;
}
