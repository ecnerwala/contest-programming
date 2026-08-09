#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		const int B = 30;
		std::array<int, B+1> freq; freq.fill(0);
		for (auto a : A) {
			freq[a == 1 ? 0 : 32 - __builtin_clz(a-1)]++;
		}
		int ans = 0;
		for (int i = 0; i <= B; i++) {
			if (freq[i]) ans = std::max(ans, freq[i] + i);
		}
		assert(ans >= N);
		ans -= [&]() -> bool {
			// see if we can do ans-1
			int t = ans-1;
			if (t < N) return false;
			int num_bigs = std::max(0, t-B);
			assert(num_bigs <= N);
			std::nth_element(A.begin(), A.end() - num_bigs, A.end());
			std::priority_queue<int> pq;
			for (auto a : std::span(A.begin(), A.end() - num_bigs)) {
				pq.push(a);
			}
			for (int i = std::min(B-1, t-1); i >= 0 && !pq.empty(); i--) {
				int v = pq.top(); pq.pop();
				v -= (1 << i);
				if (v > 0) pq.push(v);
			}
			return pq.empty();
		}();
		std::cout << ans << '\n';
	}

	return 0;
}
