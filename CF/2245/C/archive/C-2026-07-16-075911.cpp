#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, K; std::cin >> N >> K;

		// f is just suffix mins, where f(n-1) = n
		int b = 0;
		assert((1 << b) <= N);
		while ((1 << (b+1)) <= N) b++;
		assert((1 << b) <= N);
		K ^= N;
		if (K >= (1 << (b+1))) {
			std::cout << "NO" << '\n';
			continue;
		}
		std::vector<int> ans; ans.reserve(N);
		if (K >= (1 << b)) {
			// N ^ something smaller
			if (N == (1 << b)) {
				std::cout << "NO" << '\n';
				continue;
			}
			assert(N-1 >= (1 << b));
			K ^= N-1;
			ans.push_back(N-1);
			assert(K < (1 << b));
		}
		assert(K < (1 << b));
		if (K > 0) {
			ans.push_back(K);
		}
		ans.push_back(0);
		std::vector<bool> used(N);
		for (auto v : ans) used[v] = true;
		for (int i = 0; i < N; i++) if (!used[i]) ans.push_back(i);
		std::reverse(ans.begin(), ans.end());
		std::cout << "YES" << '\n';
		for (int i = 0; i < N; i++) {
			std::cout << ans[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
