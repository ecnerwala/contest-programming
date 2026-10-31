#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t L, R; cin >> L >> R;
		R++;
		int cnt = __builtin_ctzll(R);
		cnt = std::min(cnt, __builtin_ctzll(L));
		int64_t ans = 1;
		// check if flipping works
		int split = 63 - __builtin_clzll(L ^ R);
		int64_t r_sz = R & ((int64_t(1) << split) - 1);
		int64_t l_sz = L & ((int64_t(1) << split) - 1);
		ans += (l_sz + r_sz == (int64_t(1) << split));
		ans <<= cnt;
		cout << ans - 1 << '\n';
	}

	return 0;
}
