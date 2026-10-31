#include <bits/stdc++.h>

int main () {
	int N; std::cin >> N;
	std::vector<int> S(N); for (auto& s : S) std::cin >> s;
	std::vector<int> C(N); for (auto& c : C) std::cin >> c;
	const int64_t INF = 1e9;
	int64_t ans = INF;
	for (int j = 1; j+1 < N; j++) {
		int64_t lo = INF;
		int64_t hi = INF;
		for (int i = 0; i < j; i++) {
			if (S[i] < S[j]) lo = std::min(lo, int64_t(C[i]));
		}
		for (int k = j+1; k < N; k++) {
			if (S[j] < S[k]) hi = std::min(hi, int64_t(C[k]));
		}
		ans = std::min(ans, lo + C[j] + hi);
	}
	if (ans == INF) ans = -1;
	std::cout << ans << '\n';
}
