#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		// With n flavors you get n * (n-1) / 2 + k
		// n * (n-1) / 2 <= k
		// (2n-1)^2 <= 8k+1
		int64_t W; cin >> W;
		int64_t a = (int64_t(std::sqrt(8*W+1)) + 1) / 2;
		if (a < 2) a = 2;
		while ((a+1) * a / 2 <= W) a++;
		while (a * (a-1) / 2 > W) a--;
		std::cout << a + (W - a * (a-1) / 2) << '\n';
	}

	return 0;
}
