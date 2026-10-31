#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		const int MOD = 998244353;
		int cnt = 0; int64_t ways = 1;

		struct stk_t { int loc, sz, free; };
		std::vector<stk_t> stk; stk.reserve(N);
		std::vector<int> last_loc(N, -1);
		for (int i = 0; i < 2*N; i++) {
			int c; cin >> c; c--;
			int j = std::exchange(last_loc[c], i);
			if (j == -1) {
				stk.push_back({i, 1, 1});
			} else {
				int sz = 1, free = -1;
				while (stk.back().loc > j) {
					sz += stk.back().sz;
					free += stk.back().free;
					stk.pop_back();
				}
				int s = stk.back().sz += sz;
				if (!(stk.back().free += free)) {
					stk.pop_back();
					if (stk.empty()) {
						cnt++;
						ways *= s;
						ways %= MOD;
					}
				}
			}
		}
		cout << cnt << ' ' << ways << '\n';
	}

	return 0;
}
