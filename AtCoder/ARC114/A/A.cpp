#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> X(N);
	for (auto& x : X) cin >> x;
	vector<int> primes({2,3,5,7,11,13,17,19,23,29,31,37,41,43,47});
	assert(int(primes.size()) == 15);
	int64_t ans = int64_t(1e18);
	for (int m = 0; m < (1 << 15); m++) {
		int64_t v = 1;
		for (int i = 0; i < 15; i++) {
			if (m & (1 << i)) v *= primes[i];
		}
		bool is_good = true;
		for (int x : X) {
			if (std::gcd(x, v) == 1) {
				is_good = false;
				break;
			}
		}
		if (is_good) {
			ans = min(ans, v);
		}
	}
	cout << ans << '\n';

	return 0;
}
