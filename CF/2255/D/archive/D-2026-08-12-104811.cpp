#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	auto ceil_log_2 = [](int a) -> int {
		return a == 1 ? 0 : 32 - __builtin_clz(a-1);
	};
	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		const int B = 30;
		std::array<int, B+1> freq; freq.fill(0);
		std::array<int, B+1> min_v; min_v.fill(1 << B);
		for (auto a : A) {
			int l = ceil_log_2(a);
			freq[l]++;
			min_v[l] = std::min(min_v[l], a);
		}
		int ans = 0;
		for (int i = 0; i <= B; i++) {
			if (freq[i]) {
				ans = std::max(ans, freq[i] + i);
				if (i < B) freq[i+1] += freq[i] - 1;
			}
		}
		assert(ans >= N);
		ans -= [&]() -> bool {
			// see if we can do ans-1
			int t = ans-1;
			if (t < N) return false;
			int l = B;
			for (int i = t-1; ; i--) {
				while (l >= 0 && freq[l] == 0) l--;
				if (l < 0) return true;
				assert(l >= 0 && freq[l] > 0);
				if (l > i) return false;
				assert(i >= l);
				freq[l]--;
				if (l == i) {
					// note that if freq[l] > 0, we'll fail next iteration anyways
					int a = min_v[l] - (1 << i);
					if (a) {
						int nl = ceil_log_2(a);
						freq[nl]++;
						min_v[nl] = std::min(min_v[nl], a);
					}
				}
			}
		}();
		std::cout << ans << '\n';
	}

	return 0;
}
