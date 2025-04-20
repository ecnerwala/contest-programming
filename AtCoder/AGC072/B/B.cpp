#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	std::string S; S.reserve(2*N); cin >> S;
	assert(int(S.size()) == 2*N);
	std::vector<int> val(2*N+1);
	for (int i = 0; i < 2*N; i++) {
		int v;
		if (S[i] == 'A') {
			v = +1;
		} else if (S[i] == 'C') {
			v = -1;
		} else assert(false);
		val[i+1] = val[i] + v;
	}

	std::vector<std::pair<int, int64_t>> pref_costs; pref_costs.reserve(N+1);
	std::vector<std::pair<int, int64_t>> suff_costs; suff_costs.reserve(N+1);
	for (int z = 0; z < 2; z++) {
		int64_t cur = 0;
		for (int i = 0; i <= 2*N; i++) {
			if (pref_costs.empty() || val[i] > pref_costs.back().first) {
				if (!pref_costs.empty()) {
					assert(cur >= pref_costs.back().second);
					if (pref_costs.back().second == cur) {
						// we're just better, erase the back
						pref_costs.pop_back();
					}
				}
				pref_costs.push_back({val[i], cur});
			}
			if (val[i] < 0) cur += (-val[i] + 1) / 2;
		}
		std::swap(pref_costs, suff_costs);
		std::reverse(val.begin(), val.end());
	}

	std::vector<int> pos_pref_freq(N+1);
	for (auto v : val) {
		if (v >= 0) {
			assert(v <= N);
			pos_pref_freq[v]++;
		}
	}
	std::vector<int64_t> sum_cost(N+1);
	{
		int cnt_odds = 0;
		int cnt_evens = 0;
		int64_t cost = 0;
		for (int i = N; i >= 0; i--) {
			cost += cnt_odds;
			cnt_evens += pos_pref_freq[i];
			sum_cost[i] = cost;
			std::swap(cnt_evens, cnt_odds);
		}
	}

	std::vector<std::optional<int64_t>> best_costs(N+1);
	for (auto [vi, ci] : pref_costs) {
		for (auto [vj, cj] : suff_costs) {
			int v = vi + vj;
			int64_t c = ci + cj;
			v = std::min(v, N);
			// add the line sum_cost[v] * k + c;
			if (!best_costs[v] || c < *best_costs[v]) {
				best_costs[v] = c;
			}
		}
	}

	struct line_t {
		int64_t s, c;
	};
	std::vector<line_t> hull; hull.reserve(N+1);
	for (int v = 0; v <= N; v++) {
		if (best_costs[v]) {
			int64_t s = sum_cost[v];
			int64_t c = *best_costs[v];
			// s * k + c
			// s is decreasing, so we get better for bigger k
			bool should_push = true;
			while (true) {
				if (hull.empty()) break;
				assert(s <= hull.back().s);
				if (c <= hull.back().c) {
					hull.pop_back();
					continue;
				}
				assert(c > hull.back().c);
				if (s == hull.back().s) {
					// we're worse
					should_push = false;
					break;
				}
				assert(s < hull.back().s);
				if (int(hull.size()) == 1) break;

				auto a = hull.end()[-2];
				auto b = hull.end()[-1];
				assert(a.s - b.s > 0);
				assert(c - b.c > 0);
				assert(b.c - a.c > 0);
				assert(b.s - s > 0);
				if ((a.s - b.s) * (c - b.c) <= (b.c - a.c) * (b.s - s)) {
					hull.pop_back();
				} else {
					break;
				}
			}
			if (should_push) {
				hull.push_back({s, c});
			}
		}
	}

	assert(!hull.empty());
	int idx = 0;
	for (int k = 1; k <= K; k++) {
		while (idx + 1 < int(hull.size()) && k * hull[idx].s + hull[idx].c >= k * hull[idx+1].s + hull[idx+1].c) idx++;
		cout << k * hull[idx].s + hull[idx].c << '\n';
	}

	return 0;
}
