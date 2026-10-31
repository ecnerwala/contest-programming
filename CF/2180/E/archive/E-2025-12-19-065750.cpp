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
		ans <<= cnt;
		cout << ans - 1 << '\n';
	}

	return 0;
}
