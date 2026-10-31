#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;

		std::vector<std::pair<int, int>> opt; opt.reserve(32);
		int cur_cost = 0;
		int v = 0;
		for (auto a : A) v |= a;
		opt.push_back({cur_cost, __builtin_popcount(v)});
		for (int b = 0; b < 31; b++) {
			if ((v >> b) & 1) continue;
			for (int x = b; x >= 0; x--) {
				if ((v >> x) & 1) continue;
				int cheapest = (1 << x) + 1;
				int idx = -1;
				for (int i = 0; i < N; i++) {
					assert(!((A[i] >> x) & 1));
					int cnd = (1 << (x+1)) - (A[i] & ((1 << x) - 1));
					if (cnd < cheapest) { cheapest = cnd, idx = i; }
				}
				assert(idx != -1);
				A[idx] += cheapest;
				cur_cost += cheapest;
				v = 0; for (auto a : A) v |= a;
			}
			assert(__builtin_popcount(v) == opt.back().second + 1);
			opt.push_back({cur_cost, __builtin_popcount(v)});
		}

		for (int q = 0; q < Q; q++) {
			int b; cin >> b;
			auto it = std::lower_bound(opt.begin(), opt.end(), std::pair<int, int>{b+1, -1});
			assert(it != opt.begin());
			cout << std::prev(it)->second << '\n';
		}
	}

	return 0;
}
