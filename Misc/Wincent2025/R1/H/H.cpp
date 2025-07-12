#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int L = 1;
		while (N > (1 << L) - 1) L++;
		assert((1 << (L-1)) <= N && N < (1 << L));
		int64_t ans = 0;
		int num_bottom_leaves = N - (1 << (L-1)) - 1;
		for (int i = 1; i <= L; i++) {
			int num_big = num_bottom_leaves >> (L - i);
			int extra_bottom = num_bottom_leaves & ((1 << (L-i)) - 1);
			int num_small = (1 << (i-1)) - bool(extra_bottom) - num_big;
			auto check = [&](int cnt, int sz) -> void {
				int mi = i;
				int ma = N - sz + 1;
				assert(mi <= ma);
				ans += int64_t(cnt) * int64_t(mi + ma) * int64_t(ma - mi - 1) / 2;
			};
			if (num_big) {
				int sz_big = (1 << (L-i+1)) - 1;
				check(num_big, sz_big);
			}
			if (extra_bottom) {
				int sz_mid = (1 << (L-i)) - 1 + extra_bottom;
				check(1, sz_mid);
			}
			if (num_small && i < L) {
				int sz_small = (1 << (L-i)) - 1;
				check(num_small, sz_small);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
