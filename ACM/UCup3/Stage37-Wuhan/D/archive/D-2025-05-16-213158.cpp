#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	int64_t tot = 0;
	int64_t tot_odd = 0;
	int64_t rem_odd = 0;
	std::vector<std::pair<int, int64_t>> runs; runs.reserve(N);
	for (int i = 0; i < N; i++) {
		int v; int64_t l; cin >> v >> l;
		v &= 1;
		tot += l;
		if (v) tot_odd += l;

		if (!runs.empty() && runs.back().first == v) {
			l += runs.back().second;
			runs.pop_back();
		}
		if (v == 0) {
			if (runs.empty()) {
			} else {
				runs.push_back({v, l});
			}
		} else {
			rem_odd += l / 2;
			l &= 1;
			if (l) {
				runs.push_back({v, l});
			}
		}
	}
	std::vector<int64_t> costs; costs.reserve(N);
	std::vector<int64_t> stk; stk.reserve(N);
	for (int i = 1; i < int(runs.size()); i += 2) {
		assert(runs[i].first == 0);
		int64_t cur = runs[i].second;
		cerr << "even run " << cur << '\n';
		while (true) {
			if (stk.empty() || cur < stk.back()) {
				stk.push_back(cur);
				break;
			} else {
				costs.push_back(stk.back() + 1);
				cur -= stk.back();
				stk.pop_back();
				if (stk.empty()) {
					break;
				} else {
					cur += stk.back();
					stk.pop_back();
					continue;
				}
			}
			assert(false);
		}
	}
	while (!stk.empty()) {
		costs.push_back(stk.back() + 1);
		stk.pop_back();
		if (!stk.empty()) {
			stk.pop_back();
		}
	}
	assert(int(costs.size()) == (tot_odd - 2 * rem_odd) / 2);

	std::sort(costs.begin(), costs.end());

	std::vector<int64_t> pref_costs(costs.size() + 1);
	for (int i = 0; i < int(costs.size()); i++) {
		pref_costs[i+1] = pref_costs[i] + costs[i];
	}

	for (int q = 0; q < Q; q++) {
		int64_t R; cin >> R;
		int64_t max_odd = std::min(R, tot_odd);
		if ((max_odd & 1) != (tot_odd & 1)) max_odd--;

		int64_t min_odd = tot_odd;
		int64_t num_merges = tot - R;
		if (num_merges <= rem_odd) {
			min_odd -= 2 * num_merges;
		} else {
			min_odd -= 2 * rem_odd;
			num_merges -= rem_odd;
			assert(num_merges >= 0);
			min_odd -= 2 * (int(std::upper_bound(pref_costs.begin(), pref_costs.end(), num_merges) - pref_costs.begin()) - 1);
		}
		cout << max_odd << ' ' << R - min_odd << '\n';
	}

	return 0;
}
