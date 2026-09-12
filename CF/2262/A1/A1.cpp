#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> cnt(N+1);
		for (int i = 1; i <= N; i++) {
			int a; std::cin >> a;
			// the buckets are half-open [a * i, (a+1) * i)
			assert(a * i <= N);
			cnt[a*i]++;
			cnt[std::min(N, a*i+i)]--;
		}
		std::vector<int> res; res.reserve(N);
		for (int i = 0; i < N; i++) {
			cnt[i+1] += cnt[i];
			if (!cnt[i]) res.push_back(i);
		}
		assert(!cnt[N]);
		std::cout << res.size() << '\n';
		for (int i = 0; i < int(res.size()); i++) {
			std::cout << res[i] << " \n"[i+1==int(res.size())];
		}
		if (res.empty()) std::cout << '\n';
	}

	return 0;
}
