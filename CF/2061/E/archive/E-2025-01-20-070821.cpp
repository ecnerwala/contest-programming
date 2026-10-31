#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		std::vector<int> A(N); for (auto& a : A) cin >> a;
		std::vector<int> B(M); for (auto& b : B) cin >> b;
		std::vector<int> deltas; deltas.reserve(N*M);
		std::vector<int> val(1 << M);
		std::vector<int> best(M+1);

		int64_t ans = 0;
		for (auto a : A) {
			val[0] = a;
			for (int i = 0; i < M; i++) {
				for (int m = 0; m < (1 << i); m++) {
					val[(1 << i) + m] = val[m] & B[i];
				}
			}
			std::fill(best.begin(), best.end(), a);
			for (int m = 0; m < (1 << M); m++) {
				setmin(best[__builtin_popcount(m)], val[m]);
			}
			ans += a;
			for (int i = 0; i < M; i++) {
				deltas.push_back(best[i+1] - best[i]);
			}
		}
		std::nth_element(deltas.begin(), deltas.begin() + K, deltas.end());
		cout << std::accumulate(deltas.begin(), deltas.begin() + K, ans) << '\n';
	}

	return 0;
}
