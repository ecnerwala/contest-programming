#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		std::vector<std::pair<int64_t, int>> pref(N);
		pref[0] = {0, 0};
		for (int i = 0; i+1 < N; i++) {
			pref[i+1] = {pref[i].first + A[i], i+1};
		}
		std::sort(pref.begin(), pref.end());
		std::vector<int> P(N);
		for (int i = 0; i < N; i++) {
			P[pref[i].second] = N-1-i;
		}
		for (int i = 0; i < N; i++) std::cout << P[i]+1 << " \n"[i+1==N];
	}

	return 0;
}
