#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) std::cin >> a;
		std::cout << ([&]() -> bool {
			std::vector<int64_t> pref(N+1);
			for (int i = 0; i < N; i++) pref[i+1] = pref[i] + A[i];
			bool can_eq = true;
			int last_dist = 1;
			for (int i = 0; i <= N; i++) {
				if (pref[i] * N < pref[0] * (N-i) + pref[N] * i) {
					return true;
				} else if (pref[i] * N == pref[0] * (N-i) * pref[N] * i) {
					last_dist = 0;
				}
				last_dist++;
				if (last_dist > 2) can_eq = false;
			}
			return can_eq;
		}() ? "Yes" : "No") << '\n';
	}

	return 0;
}
