#include <bits/stdc++.h>

int64_t solve(int64_t N) {
	int64_t ans = 0;
	for (int64_t g = 1; g * g <= N; g++) {
		for (int64_t b = 1; b * g * std::max(b, g) <= N; b++) {
			// b <= d and g < d
			int64_t max_d = ((N / g) - 1) / b;
			// d > b and d > g
			ans += 2 * std::max<int64_t>(0, max_d - std::max(b, g));
			if (max_d >= b && b > g) {
				// d == b and d > g
				ans ++;
			}
		}
	}

	for (int64_t b = 1; b * b <= N; b++) {
		for (int64_t d = 1; b * d * std::max(b, d) <= N; d++) {
			// g >= b and d
			int64_t max_g = N / (1 + b * d);
			ans += std::max<int64_t>(0, max_g - std::max(b, d) + 1);
		}
	}
	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Z; cin >> Z;
	while (Z--) {
		int64_t N; cin >> N;
		cout << solve(N) << '\n';
	}

	return 0;
}
