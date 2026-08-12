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
		const int B = 30;
		std::array<int, B+1> freq; freq.fill(0);
		std::array<int, B+1> min_v; min_v.fill(1 << B);
		while (N--) {
			int a; std::cin >> a;
			int l = ceil_log_2(a);
			freq[l]++;
			min_v[l] = std::min(min_v[l], a);
		}
		int ans = 0;
		{
			int cur_freq = 0;
			for (int i = 0; i <= B; i++) {
				cur_freq += freq[i];
				if (cur_freq) {
					ans = std::max(ans, cur_freq + i);
					cur_freq--;
				}
			}
		}
		ans -= [&]() -> bool {
			// see if we can do ans-1
			int t = ans-1;
			int l = B;
			for (int i = t-1; ; i--) {
				while (l >= 0 && freq[l] == 0) l--;
				if (l < 0) return true;
				assert(l >= 0 && freq[l] > 0);
				if (i == -1) return false;
				if (l > i+1) return false;
				assert(i+1 >= l);
				freq[l]--;
				if (l == i+1) {
					// note that if freq[l] > 0, we'll fail next iteration anyways
					if (freq[l]) return false;
					int a = min_v[l] - (1 << i);
					int nl = ceil_log_2(a);
					assert(nl < l);
					freq[nl]++;
					min_v[nl] = std::min(min_v[nl], a);
				}
			}
		}();
		std::cout << ans << '\n';
	}

	return 0;
}
