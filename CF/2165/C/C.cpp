#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		const int B = 30;
		int N, Q; cin >> N >> Q;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		if (N > B) {
			std::nth_element(A.begin(), A.begin() + B, A.end(), std::greater<>());
			A.resize(B);
			N = B;
		}

		std::vector<int> buf(N);
		for (int q = 0; q < Q; q++) {
			for (int i = 0; i < N; i++) buf[i] = A[i];

			int c; cin >> c;
			int64_t ans = 0;
			for (int b = B-1; b >= 0; b--) {
				bool want = (c >> b) & 1;
				int cnt = 0;
				for (int i = 0; i < N; i++) {
					cnt += (buf[i] >> b) & 1;
				}
				if (cnt > want) break;
				if (cnt < want) {
					assert(!cnt);
					int best_v = -1;
					int best_i = -1;
					for (int i = 0; i < N; i++) {
						int cnd = buf[i] & ((1 << b) - 1);
						if (cnd > best_v) { best_v = cnd, best_i = i; }
					}
					ans += (1 << b) - best_v;
					buf[best_i] += (1 << b) - best_v;
				}
			}
			cout << ans << '\n';
		}
	}

	return 0;
}
