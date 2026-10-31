#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N; cin >> N;

	// x^2 - y = z^2
	// y = (x+z) * (x-z)
	// y > 0, so x-z >= 1 and y >= 2x+1
	// 1 <= y <= N and y factorizes into a * b of the same parity
	// Number of pairs a, b of the same parity such that a*b <= N
	
	int64_t ans = 0;
	for (int64_t a = 1; a * a <= N; a++) {
		int64_t ma = N / a;
		ans += (ma - a) / 2 + 1;
		ans %= 998244353;
	}
	cout << ans << '\n';

	return 0;
}
