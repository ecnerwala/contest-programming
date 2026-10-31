#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;

		std::vector<std::tuple<int64_t, int, int64_t>> evts; evts.reserve(N+Q);
		for (int i = 0; i < N; i++) {
			int64_t l, r, z; cin >> l >> r >> z;
			int64_t d = r-l;
			int64_t s = r+l;
			evts.push_back({d, -1-z, s});
		}
		for (int q = 0; q < Q; q++) {
			int64_t l, r; cin >> l >> r;
			int64_t d = r-l;
			int64_t s = r+l;
			evts.push_back({d, q, s});
		}

		std::sort(evts.begin(), evts.end());

		std::vector<int> ans(Q);

		std::set<int64_t> cur;
		cur.insert(-1);

		int64_t last_d = -1;
		std::set<int64_t> to_fix;

		for (auto [d, op, s] : evts) {
			assert(d >= last_d);
			if (d > last_d) {
				for (int64_t i : to_fix) {
					if ((i & 1) != (last_d & 1)) {
						--i;
					}
					if (cur.count(i) && cur.count(i+1)) {
						cur.erase(i);
						cur.erase(i+1);
					}
				}
				to_fix.clear();
				last_d = d;
			}
			assert((s & 1) == (d & 1));
			assert(s >= 0);
			if (op < 0) {
				int z = -1-op;
				auto it = std::prev(cur.upper_bound(s));
				if (((*it & 1) == (s & 1)) == z) {
					// do nothing
					continue;
				}

				auto nt = std::next(it);

				if ((*it & 1) == (s & 1)) {
					assert(z == 0);
					// Flip W to L
					if (nt != cur.end() && *nt == s+1) {
						cur.erase(nt);
					} else {
						cur.insert(s+2);
						to_fix.insert(s+2);
					}
					if (*it == s) {
						cur.erase(it);
					} else {
						cur.insert(s-1);
						to_fix.insert(s-1);
					}
				} else {
					assert(z == 1);
					// Flip L to W
					if (nt != cur.end() && *nt == s+2) {
						cur.erase(nt);
					} else {
						cur.insert(s+1);
						to_fix.insert(s+1);
					}
					if (*it == s-1) {
						cur.erase(it);
					} else {
						cur.insert(s);
						to_fix.insert(s);
					}
				}
			} else {
				int q = op;
				auto it = std::prev(cur.upper_bound(s));
				ans[q] = (*it & 1) == (s & 1);
			}
		}

		for (int v : ans) { cout << v; } cout << '\n';
	}

	return 0;
}
