#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, Q; std::cin >> N >> Q;
		std::vector<int64_t> A(N); for (auto& a : A) std::cin >> a;

		std::vector<int64_t> x_costs(N+1);
		std::vector<int64_t> y_costs(N+1);
		for (int st = 0, en = 0; st < N; st = en) {
			if (!A[st]) {
				en++;
				continue;
			}
			while (en < N && A[en] > 0) en++;
			// solve A[st..en]

			// first do the X >= Y case
			{
				std::vector<int> bad_pos; bad_pos.reserve(en - st);
				for (int i = st; i < en; i++) {
					assert(A[i] >= 1);
					x_costs[0] += A[i] / 2;
					if (A[i] & 1) {
						bad_pos.push_back(i);
					}
				}
				std::vector<int> bad_dist; bad_dist.reserve(bad_pos.size());
				for (int z = 0; z+1 < int(bad_pos.size()); z++) {
					int cost = bad_pos[z+1] - bad_pos[z];
					bool should_ins = true;
					while (!bad_dist.empty() && cost >= bad_dist.back()) {
						assert(cost > 0);
						assert(bad_dist.back() > 0);
						x_costs[bad_dist.back()]++;
						cost -= bad_dist.back();
						bad_dist.pop_back();
						if (bad_dist.empty()) {
							should_ins = false;
							break;
						}
						cost += bad_dist.back();
						bad_dist.pop_back();
					}
					if (should_ins) {
						assert(cost > 0);
						bad_dist.push_back(cost);
					}
				}
				while (!bad_dist.empty()) {
					assert(bad_dist.back() > 0);
					x_costs[bad_dist.back()]++;
					bad_dist.pop_back();
					if (bad_dist.empty()) break;
					bad_dist.pop_back();
				}
			}

			// now, do the Y >= X case
			{
				int L = en - st;
				std::vector<int64_t> cnt_odd(L);
				std::vector<int> max_odd(L); std::iota(max_odd.begin(), max_odd.end(), 0);
				std::vector<int> max_any(L); std::iota(max_any.begin(), max_any.end(), 0);
				auto add_interval = [&](int l, int r, int64_t cnt) -> void {
					l -= st, r -= st;
					y_costs[0] += int64_t((r - l) / 2) * int64_t(cnt);
					if ((r - l) & 1) {
						cnt_odd[l] += cnt;
						max_odd[l] = std::max(max_odd[l], r);
					} else {
					}
					max_any[l] = std::max(max_any[l], r);
				};

				{
					// are all y_costs == 1 or 0??
					// let's just find all the maximal intervals
					int lo = en - st, hi = en - st;
					struct inter_t {
						std::array<int64_t, 2> bounds;
						int l_idx;
					};
					std::vector<inter_t> intervals(2 * (en - st));
					std::vector<std::array<int, 2>> odds; odds.reserve(2 * (en - st));
					intervals[hi++] = {{0, A[st]}, st};
					int64_t cur = A[st];
					for (int i = st+1; i < en; i++) {
						if (!((i-st) & 1)) {
							int64_t nxt = cur + A[i];
							//std::cerr << "incr " << cur << ' ' << nxt << '\n';
							// go increasing
							assert(lo < hi);
							if (nxt > intervals[hi-1].bounds[1]) {
								intervals[hi] = {{intervals[hi-1].bounds[1], nxt}, i};
								hi++;
							}
							cur = nxt;
							while (intervals[hi-1].bounds[1] > cur) {
								assert(lo < hi);
								if (intervals[hi-1].bounds[0] >= cur) {
									add_interval(intervals[hi-1].l_idx, i, intervals[hi-1].bounds[1] - intervals[hi-1].bounds[0]);
									hi--;
								} else {
									add_interval(intervals[hi-1].l_idx, i, intervals[hi-1].bounds[1] - cur);
									intervals[hi-1].bounds[1] = cur;
								}
							}
						} else {
							// go decreasing
							int64_t nxt = cur - A[i];
							//std::cerr << "decr " << cur << ' ' << nxt << '\n';
							assert(lo < hi);
							if (nxt < intervals[lo].bounds[0]) {
								intervals[lo-1] = {{nxt, intervals[lo].bounds[0]}, i};
								lo--;
							}
							cur = nxt;
							while (intervals[lo].bounds[0] < cur) {
								assert(lo < hi);
								if (intervals[lo].bounds[1] <= cur) {
									// remove the whole thing
									add_interval(intervals[lo].l_idx, i, intervals[lo].bounds[1] - intervals[lo].bounds[0]);
									lo++;
								} else {
									add_interval(intervals[lo].l_idx, i, cur - intervals[lo].bounds[0]);
									intervals[lo].bounds[0] = cur;
								}
							}
						}
						//for (int z = lo; z < hi; z++) { std::cerr << intervals[z].bounds[0] << '-' << intervals[z].bounds[1] << '@' << intervals[z].l_idx << '\n'; }
					}
					// close off all the intervals at the end
					for (int z = lo; z < hi; z++) {
						add_interval(intervals[z].l_idx, en, intervals[z].bounds[1] - intervals[z].bounds[0]);
					}
				}

				for (int i = 0; i+1 < L; i++) {
					max_odd[i+1] = std::max(max_odd[i+1], max_odd[i]);
					max_any[i+1] = std::max(max_any[i+1], max_any[i]);
				}
				std::vector<std::array<int, 2>> odds; odds.reserve(L);
				for (int i = 0, j = 0; i < L; i = j) {
					if (max_odd[i] == i) {
						j = i+1;
						continue;
					}
					while (max_odd[j] > j) {
						j = max_odd[j];
					}
					int64_t tot_cnt = 0;
					for (int k = i; k < j; k++) {
						tot_cnt += cnt_odd[k];
					}
					y_costs[1] += tot_cnt / 2;
					tot_cnt &= 1;
					odds.push_back({i, j});

				}
				std::vector<int> bad_dist; bad_dist.reserve(odds.size());
				for (int z = 0; z+1 < int(odds.size()); z++) {
					int cost = 0;
					{
						int i = odds[z][1]-1;
						while (i < odds[z+1][0]) {
							i = max_any[i]-1;
							cost++;
						}
					}
					bool should_ins = true;
					while (!bad_dist.empty() && cost >= bad_dist.back()) {
						assert(cost > 0);
						assert(bad_dist.back() > 0);
						x_costs[bad_dist.back()]++;
						cost -= bad_dist.back();
						bad_dist.pop_back();
						if (bad_dist.empty()) {
							should_ins = false;
							break;
						}
						cost += bad_dist.back();
						bad_dist.pop_back();
					}
					if (should_ins) {
						assert(cost > 0);
						bad_dist.push_back(cost);
					}
				}
				while (!bad_dist.empty()) {
					assert(bad_dist.back() > 0);
					x_costs[bad_dist.back()]++;
					bad_dist.pop_back();
					if (bad_dist.empty()) break;
					bad_dist.pop_back();
				}
			}
		}

		std::vector<std::array<int64_t, 2>> x_vals(N+1);
		for (int i = 0; i <= N; i++) {
			x_vals[i] = (i ? x_vals[i-1] : std::array<int64_t, 2>{0, 0});
			x_vals[i][0] += x_costs[i];
			x_vals[i][0] -= int64_t(x_costs[i]) * i;
			x_vals[i][1] += int64_t(x_costs[i]) * i;
		}
		std::vector<std::array<int64_t, 2>> y_vals(N+1);
		for (int i = 0; i <= N; i++) {
			y_vals[i] = (i ? y_vals[i-1] : std::array<int64_t, 2>{0, 0});
			y_vals[i][0] += y_costs[i];
			y_vals[i][0] -= int64_t(y_costs[i]) * i;
			y_vals[i][1] += int64_t(y_costs[i]) * i;
		}

		for (int q = 0; q < Q; q++) {
			int x, y; std::cin >> x >> y;
			if (x >= y) {
				int max_cost = (x == y) ? N : std::min(N, x / (x - y));
				auto [cx, cy] = x_vals[max_cost];
				std::cout << (x * cx + y * cy) << '\n';
			} else {
				int max_cost = (y == x) ? N : std::min(N, y / (y - x));
				auto [cy, cx] = y_vals[max_cost];
				std::cout << (x * cx + y * cy) << '\n';
			}
		}
	}

	return 0;
}
