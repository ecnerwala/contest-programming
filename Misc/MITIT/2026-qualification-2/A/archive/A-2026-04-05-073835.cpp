#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, K, X, Y; std::cin >> N >> K >> X >> Y;
		std::vector<int> A(K);
		for (auto& a : A) std::cin >> a;
		std::vector<int> pref(K+1);
		for (int i = 0; i < K; i++) pref[i+1] = (pref[i] + A[i]) % N;

		std::vector<int> end_idx(N, -1);
		for (int i = K; i >= 0; i--) {
			int v = (pref[i] + (X - Y) + N) % N;
			if (end_idx[v] == -1) {
				end_idx[pref[i]] = (Y + pref[K] - pref[i] + N) % N;
			} else {
				end_idx[pref[i]] = end_idx[v];
			}
		}
		std::vector<int> res; res.reserve(N-1);
		for (int i = 0; i < N; i++) {
			if (i == X) continue;
			int v = (X - i + N) % N;
			int ans;
			if (end_idx[v] == -1) {
				ans = (pref[K] + i) % N;
			} else {
				ans = end_idx[v];
			}
			res.push_back(ans);
		}
		for (int i = 0; i < N-1; i++) {
			std::cout << res[i] << " \n"[i+1==N-1];
		}
	}

	return 0;
}
