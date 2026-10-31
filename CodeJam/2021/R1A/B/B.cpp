#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		vector<pair<int, int64_t>> P(N);
		int64_t tot = 0;
		for (auto& x : P) {
			cin >> x.first >> x.second;
			tot += x.first * x.second;
		}

		auto is_good = [&](int64_t cnd) -> bool {
			if (cnd <= 1) return false;

			int64_t cur_sum = 0;
			int64_t v = cnd;

			for (auto x : P) {
				int cur_cnt = 0;
				while (v % x.first == 0) {
					v /= x.first;
					cur_cnt ++;
				}
				if (cur_cnt > x.second) {
					return false;
				}
				cur_sum += x.first * (x.second - cur_cnt);
			}
			if (v > 1) {
				return false;
			}
			if (cur_sum != cnd) {
				return false;
			}
			return true;
		};

		int64_t ans = 0;
		for (int s = 2; s <= 4000; s++) {
			int64_t v = tot - s;
			if (is_good(v)) {
				ans = v;
				break;
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
