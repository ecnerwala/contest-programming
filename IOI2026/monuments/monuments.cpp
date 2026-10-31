#include "monuments.h"
#include <vector>
#include <cassert>
#include <cmath>
#include <utility>

long long get_cost(std::vector<int> X, std::vector<int> P) {
	int N = int(X.size());
	int M = int(P.size());
	int cur_x = std::max(X.back(), -X.front());
	int cur_balance = 0;
	int start_height = 2*N+1;
	int cur_height = start_height;
	std::vector<int> move_cost(4*N+2);
	std::vector<int> height_val(4*N+2);
	std::vector<int> max_source(4*N+2);
	int64_t ans = 0;
	for (int lo = 0, p_lo = 0, hi = N-1, p_hi = M-1; true; ) {
		int sources = 0;
		int fixed_lo = 0, fixed_hi = 0;
		for (; lo <= hi && X[hi] == cur_x; hi--) {
			sources ++;
			if (p_hi >= p_lo && P[p_hi] == hi) { fixed_hi++, p_hi--; }
			cur_balance ++;
		}
		for (; lo <= hi && X[lo] == -cur_x; lo++) {
			sources ++;
			if (p_hi >= p_lo && P[p_lo] == lo) { fixed_lo++, p_lo++; }
			cur_balance --;
		}
		int sinks;
		if (cur_x == 0) {
			sinks = fixed_lo + fixed_hi;
			if ((N ^ sinks) & 1) sinks++;
		} else {
			sinks = 2 * std::max(fixed_lo, fixed_hi);
		}
		while (sources--) {
			int cur_cost = std::exchange(move_cost[cur_height], 0);

			ans += height_val[cur_height] = cur_cost - height_val[cur_height];
			max_source[cur_height] = std::max(max_source[cur_height], height_val[cur_height]);

			cur_height++;
			move_cost[cur_height] += cur_cost;
		}
		while (sinks--) {
			int cur_cost = std::exchange(move_cost[cur_height - 1], 0);
			--cur_height;

			ans += height_val[cur_height] = cur_cost - height_val[cur_height];

			move_cost[cur_height - 1] += cur_cost;
		}
		if (cur_x == 0) break;
		{
			int nxt_x = hi >= lo ? std::max(X[hi], -X[lo]) : 0;
			assert(nxt_x >= 0);
			assert(cur_x >= nxt_x);
			ans += int64_t(std::abs(cur_balance)) * int64_t(cur_x - nxt_x);
			move_cost[cur_height + std::abs(cur_balance)] += cur_x - nxt_x;
			move_cost[cur_height - std::abs(cur_balance) - 1] += cur_x - nxt_x;
			cur_x = nxt_x;
		}
	}
	assert(cur_x == 0);
	if (cur_height < start_height) {
		return -1;
	}
	for (int h = start_height; h < cur_height; h++) {
		ans -= max_source[h];
	}
	return ans;
}
