#include "mushrooms.h"

#include <bits/stdc++.h>

namespace {

enum class MOVE_TYPE {
	NONE,
	AxAx,
	AxAxAxAx,
	xAx_xBx,
};

std::pair<int, MOVE_TYPE> get_max_cnt(int ka, int kb, int q) {
	if (q == 0) return {0, MOVE_TYPE::AxAxAxAx};

	if (ka < kb) std::swap(ka, kb);
	assert(ka >= kb);

	static std::map<std::tuple<int, int, int>, std::pair<int, MOVE_TYPE>> mem;
	auto it = mem.find({ka, kb, q});
	if (it != mem.end()) return it->second;

	std::pair<int, MOVE_TYPE> ans = {0, MOVE_TYPE::AxAxAxAx};

	// AxAx
	if (ka >= 2 || kb >= 2) {
		ans = std::max(ans,
			{
				2 + std::min({
					get_max_cnt(ka+2, kb+0, q-1).first,
					get_max_cnt(ka+1, kb+1, q-1).first,
					get_max_cnt(ka+0, kb+2, q-1).first,
				}), MOVE_TYPE::AxAx
			}
		);
	}

	// AxAxAxAxAx
	ans = std::max(ans, 
		{
			std::max(ka, kb) + std::min(get_max_cnt(ka+1, kb, q-1).first, get_max_cnt(ka, kb+1, q-1).first), MOVE_TYPE::AxAxAxAx
		}
	);

	/*
	// xAxxAxxAx - xBxxBxxBx
	if (q >= 2 && std::min(ka, kb) >= 1) {
		ans = std::max(ans,
			{ std::min(ka, kb) * 2 + get_max_cnt(ka, kb, q-2).first, MOVE_TYPE::xAx_xBx }
		);
	}
	*/

	return mem[{ka, kb, q}] = ans;
}

int get_max_cnt(int q) {
	return get_max_cnt(1, 0, q).first+1;
}

}

int count_mushrooms(int N) {
	int Q = 245;
	while (get_max_cnt(Q) < N) Q++;

	std::array<std::vector<int>, 2> knowns;
	knowns[0].reserve(N);
	knowns[1].reserve(N);

	knowns[0].push_back(0);
	std::vector<int> unknowns(N-1);
	std::iota(unknowns.begin(), unknowns.end(), 1);

	int ans = 0;
	while (!unknowns.empty()) {
		MOVE_TYPE t = get_max_cnt(int(knowns[0].size()), int(knowns[1].size()), Q).second;
		Q--;

		if (int(unknowns.size()) == 1) t = MOVE_TYPE::AxAxAxAx;

		switch (t) {
		case MOVE_TYPE::AxAx:
			{
				int z = knowns[0].size() >= knowns[1].size() ? 0 : 1;
				assert(knowns[z].size() >= 2);
				assert(unknowns.size() >= 2);
				int u = unknowns.back(); unknowns.pop_back();
				int v = unknowns.back(); unknowns.pop_back();
				int r = use_machine({u, knowns[z][0], v, knowns[z][1]});
				knowns[z ^ bool(r & 1)].push_back(u);
				knowns[z ^ bool(r & 2)].push_back(v);
			}
			break;
		case MOVE_TYPE::AxAxAxAx:
			{
				int z = knowns[0].size() >= knowns[1].size() ? 0 : 1;
				int s = std::min(int(knowns[z].size()), int(unknowns.size()));
				assert(s >= 1);

				std::vector<int> a; a.reserve(2*s);
				for (int i = 0; i < s; i++) {
					a.push_back(unknowns.back()); unknowns.pop_back();
					a.push_back(knowns[z][i]);
				}
				int u = a[0];
				int r = use_machine(a);
				knowns[z ^ bool(r & 1)].push_back(u);
				ans += z ? (r/2) : s-1-(r/2);
			}
			break;
		case MOVE_TYPE::xAx_xBx:
			{
				int s = std::min(
					2 * std::min(int(knowns[0].size()), int(knowns[1].size())),
					int(unknowns.size())
				);
				assert(s >= 1);
				int cur_val = 0;
				for (int z = 0; z < 2; z++) {
					std::vector<int> a; a.reserve(s + (s+1)/2);
					for (int i = 0; i < s; i++) {
						a.push_back(unknowns.back()); unknowns.pop_back();
						if (i % 2 == 0) {
							a.push_back(knowns[z][i/2]);
						}
					}
					int r = use_machine(a);
					if (z == 0) cur_val -= r;
					else cur_val += r;
				}
				assert((cur_val + s) % 2 == 0);
				ans += (cur_val + s) / 2;
			}
			break;
		default: assert(false);
		}
	}

	return ans + int(knowns[0].size());
}

// A few options:
// AxAx -> +2 known mushrooms
// AxAxAxAxAxAx -> +1 known, +(#A-1) counted
// xAxxAxxAxxAx - xBxxBxxBxxBx -> 2 queries, +2*min(#A, #B) counted
//
// xAxxAxxAxxAxxAxxAx
// xBxxBxxBxxBxxBxxBx
// subtract gives you the right counts
//
// Run k, we can get either 2/3k effective
// If we run k queries, we can get 2k known, 2/3 * 2k at a time
// k + 3/4 * n/k
// k ~ 4/3 n/k
