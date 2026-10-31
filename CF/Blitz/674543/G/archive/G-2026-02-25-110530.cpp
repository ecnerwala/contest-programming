#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);
	int T; std::cin >> T;
	while (T--) {
		int N, K; std::cin >> N >> K;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		assert(K >= N);
		auto max_ops = [&](int a, int p) -> int {
			int d = __builtin_ctz(a) - __builtin_ctz(p);
			if (d >= 0 && (a >> d) == p) {
				assert(d > 0);
				return (1 << d) - 1;
			} else {
				return 1 << __builtin_ctz(a);
			}
		};
		std::vector<int64_t> suff(N);
		std::vector<int64_t> pref(N);
		for (int i = 0; i+1 < N; i++) {
			pref[i+1] = pref[i] + max_ops(A[i], A[i+1]);
		}
		for (int i = N-2; i >= 0; i--) {
			suff[i] = suff[i+1] + max_ops(A[i+1], A[i]);
		}
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			ans = std::max(ans, pref[i] + suff[i] + max_ops(A[i], A[i]+1));
		}
		std::cerr << ans << '\n';
		std::cout << (ans >= K ? "YES" : "NO") << '\n';
	}
}
