#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASES; cin >> CASES;
	while (CASES--) {
		int N; int64_t M; cin >> N >> M;
		std::vector<int64_t> A(N); for (auto& a : A) { cin >> a; }
		std::sort(A.begin(), A.end());

		auto is_good = [&](int64_t TIME) -> bool {
			struct evt_t {
				int64_t t;
				int a_evt;
				int l_evt;
			};
			std::vector<evt_t> evts; evts.reserve(4*N);
			for (int i = 0; i < N; i++) {
				int64_t switch_time = (TIME / 2) + ((TIME & 1) && (i & 1));
				assert(0 <= switch_time && switch_time <= TIME);
				evts.push_back({1 + A[i], +1, 0});
				evts.push_back({switch_time + 1 + A[i], -1, 0});
				evts.push_back({switch_time - A[i], 0, +1});
				evts.push_back({TIME - A[i], 0, -1});
			}
			std::sort(evts.begin(), evts.end(), [&](auto a, auto b) { return a.t < b.t; });

			int64_t ans = 0;
			int64_t cur_open = 0;
			int64_t a_rate = 0, l_rate = 0;
			for (int i = 0; i+1 < int(evts.size()); i++) {
				int64_t t = evts[i+1].t - evts[i].t;
				a_rate += evts[i].a_evt;
				l_rate += evts[i].l_evt;
				if (t == 0) continue;
				assert(a_rate >= 0 && l_rate >= 0);
				ans += std::min(a_rate, l_rate) * t;

				if (a_rate > l_rate) {
					cur_open += (a_rate - l_rate) * t;
				} else if (l_rate > a_rate) {
					int64_t v = std::min(cur_open, (l_rate - a_rate) * t);
					cur_open -= v;
					ans += v;
				}
			}
			return ans >= M;
		};

		int64_t mi = 0;
		int64_t ma = 1;
		while (!is_good(ma)) {
			mi = ma;
			ma *= 2;
		}
		while (ma - mi > 1) {
			int64_t md = (mi + ma) / 2;
			if (is_good(md)) ma = md;
			else mi = md;
		}
		cout << ma << '\n';
	}

	return 0;
}
