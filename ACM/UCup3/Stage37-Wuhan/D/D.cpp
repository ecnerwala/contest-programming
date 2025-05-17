#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	constexpr bool TESTING = true;
	int CASES;
	if (TESTING) {
		CASES = 10000;
	} else {
		CASES = 1;
	}

	std::mt19937 mt(48);
	for (int case_num = 0; case_num < CASES; case_num++) {
		cerr << "case_num " << case_num << '\n';
		int N, Q;
		if (TESTING) {
			N = std::uniform_int_distribution<int>(1, 10)(mt);
			Q = std::uniform_int_distribution<int>(1, 20)(mt);
			cerr << N << ' ' << Q << '\n';
		} else {
			cin >> N >> Q;
		}

		int64_t tot = 0;
		int64_t tot_odd = 0;
		int64_t rem_odd = 0;
		std::vector<std::pair<int, int64_t>> runs; runs.reserve(N);
		for (int i = 0; i < N; i++) {
			int v; int64_t l;
			if (TESTING) {
				v = std::uniform_int_distribution<int>(1, 1e9)(mt);
				l = std::uniform_int_distribution<int>(1, 1e9)(mt);
				cerr << v << ' ' << l << '\n';
			} else {
				cin >> v >> l;
			}
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
			int64_t R;
			if (TESTING) {
				R = std::uniform_int_distribution<int64_t>(1, tot)(mt);
			} else {
				cin >> R;
			}
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
	}

	return 0;
}
