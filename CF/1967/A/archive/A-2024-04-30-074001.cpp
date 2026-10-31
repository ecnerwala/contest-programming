#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int64_t K; cin >> K;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;
		std::sort(A.begin(), A.end(), std::greater<>());

		int64_t mi = 0, ma = std::accumulate(A.begin(), A.end(), K) + 1;
		while (ma - mi > 1) {
			int64_t md = (mi + ma) / 2;
			if ([&]() -> bool {
				int64_t cnt = md + (N - 1);
				int64_t cur_K = K;
				for (int i = 0; i < N; i++) {
					int64_t tgt = cnt / N + (i < cnt % N);
					if (tgt > A[i]) {
						cur_K -= (tgt - A[i]);
						if (cur_K < 0) return false;
					}
				}
				return true;
			}()) {
				mi = md;
			} else {
				ma = md;
			}
		}
		cout << mi << '\n';
	}

	return 0;
}
