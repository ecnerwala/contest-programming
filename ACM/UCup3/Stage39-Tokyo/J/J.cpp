#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int L; int64_t K; cin >> L >> K;
	K++;
	int64_t singles = K / 2;
	int64_t bigs = K - singles;
	int64_t used = 0;
	__int128_t ans = 0;
	for (int i = L-1; i >= 1 && used < bigs; i--) {
		int64_t v = std::min(bigs - used, int64_t(1) << (L - 1 - i));
		ans += __int128_t(v) * i;
		used += v;
	}
	singles = std::min(singles, used + 1);
	ans += singles;
	cout << int(ans % 998244353) << '\n';

	return 0;
}
