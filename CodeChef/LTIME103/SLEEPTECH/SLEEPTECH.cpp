#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int A, B; cin >> A >> B;
		// half-open range
		auto is_good = [&](int64_t lo, int64_t hi) -> bool {
			assert(lo <= hi);
			if (lo == hi) return false;
			hi--;
			int64_t mi = 0;
			int64_t ma = B-A+2;
			while (ma - mi > 1) {
				int64_t md = mi + (ma - mi) / 2;
				if (hi >= A * md + (md) * (md - 1) / 2) {
					mi = md;
				} else {
					ma = md;
				}
			}
			int64_t k = mi;
			return B * k - (k * (k-1) / 2) >= lo;
		};

		std::vector<std::pair<int64_t, int>> evts; evts.reserve(2*N);
		for (int i = 0; i < N; i++) {
			int64_t l, r; cin >> l >> r; r++;
			evts.push_back({l, +1});
			evts.push_back({r, -1});
		}
		std::sort(evts.begin(), evts.end());
		int ans = 0;
		for (int z = 0; z+1 < int(evts.size()); z++) {
			int64_t lo = evts[z].first;
			int64_t hi = evts[z+1].first;
			if (is_good(lo, hi)) {
				ans = std::max(ans, evts[z].second);
			}
			evts[z+1].second += evts[z].second;
		}
		assert(evts.empty() || evts.back().second == 0);
		cout << ans << '\n';
	}

	return 0;
}
