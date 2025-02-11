#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	const int MX = int(1e6);
	const int RT = int(1e3);
	int IDX = 0;
	int64_t tot = 0;
	std::set<std::pair<int, int>> cur;
	std::vector<std::set<std::pair<int, int>>> small_gaps(RT+1);
	std::vector<int> last_small_gaps(RT+1, 0);
	std::vector<int> cnt_small(RT+1, 0);

	auto upd_gap = [&](auto pit, auto nit, bool ins) -> void {
		int l = nit->first - pit->first;
		if (l > RT) return;
		if (ins) {
			auto [_, did_insert] = small_gaps[l].insert(*pit);
			assert(did_insert);
		} else {
			auto num_erased = small_gaps[l].erase(*pit);
			assert(num_erased > 0);
		}
		last_small_gaps[l] = small_gaps[l].empty() ? 0 : small_gaps[l].rbegin()->first;
	};

	auto insert = [&](int v_) -> void {
		std::pair<int, int> v(v_, IDX++);
		tot += v.first;
		if (v.first <= RT) cnt_small[v.first]++;
		auto [it, did_insert] = cur.insert(v);
		assert(did_insert);
		if (it != cur.begin() && std::next(it) != cur.end()) {
			upd_gap(std::prev(it), std::next(it), false);
		}
		if (it != cur.begin()) {
			upd_gap(std::prev(it), it, true);
		}
		if (std::next(it) != cur.end()) {
			upd_gap(it, std::next(it), true);
		}
	};
	auto remove = [&](int v_) -> void {
		auto it = cur.lower_bound({v_, -1});
		std::pair<int, int> v = *it;
		assert(v.first == v_);

		if (it != cur.begin()) {
			upd_gap(std::prev(it), it, false);
		}
		if (std::next(it) != cur.end()) {
			upd_gap(it, std::next(it), false);
		}
		if (it != cur.begin() && std::next(it) != cur.end()) {
			upd_gap(std::prev(it), std::next(it), true);
		}

		cur.erase(v);
		if (v.first <= RT) cnt_small[v.first]--;
		tot -= v.first;
	};

	auto query_slow = [&]() -> bool {
		{
			auto it = cur.rbegin();
			assert(it != cur.rend());
			int64_t cur_tot = tot;
			int cur_gap = it->first;
			int cur_val = it->first;
			for (++it; it != cur.rend(); ++it) {
				int nv = it->first;
				cur_tot -= nv;
				cur_gap = std::min({cur_gap, cur_val - nv, nv});
				cur_val = nv;
				cerr << cur_tot << ' ' << cur_gap << ' ' << cur_val << '\n';
				if (cur_tot < cur_gap) {
					return true;
				}
			}
		}
		return false;
	};
	auto query = [&]() -> bool {
		return query_slow();
		assert(!cur.empty());
		// return whether a bad k exists
		{
			auto it = cur.rbegin();
			assert(it != cur.rend());
			int64_t cur_tot = tot;
			int cur_gap = it->first;
			int cur_val = it->first;
			for (++it; it != cur.rend(); ++it) {
				int nv = it->first;
				cur_tot -= nv;
				cur_gap = std::min({cur_gap, cur_val - nv, nv});
				cur_val = nv;
				if (cur_tot < cur_gap) {
					return true;
				}
				if (cur_gap <= RT) {
					break;
				}
			}
		}
		{
			// now, we look for a small gap
			int last_small_gap = 0;
			int cnt_big = int(cur.size());
			int64_t tot_small = 0;
			for (int k = 1; k <= RT; k++) {
				last_small_gap = std::max(last_small_gap, last_small_gaps[k-1]);
				cnt_big -= cnt_small[k-1];
				tot_small += int64_t(k-1) * cnt_small[k-1];
				if (cnt_big < 2) break;
				if (last_small_gap < k && tot_small < k) {
					return true;
				}
			}
		}
		return false;
	};

	for (int i = 0; i < N; i++) {
		int a; cin >> a; insert(a);
	}
	cout << (query() ? "No" : "Yes") << '\n';
	for (int q = 0; q < Q; q++) {
		char op; int a; cin >> op >> a;
		if (op == '+') insert(a);
		else if (op == '-') remove(a);
		else assert(false);
		cout << (query() ? "No" : "Yes") << '\n';
	}

	return 0;
}

// Our goal is to find some k > 0 so that sum_(barrels <= k-1) barrel <= k-1, and there are at least 2 big barrels with all gaps of barrels >=k are >=k
// We know k * # big <= MX, so we'll do 2 sides sqrt stuff
