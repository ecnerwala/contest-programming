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

	// xAxxAxxAx - xBxxBxxBx
	if (q >= 2 && std::min(ka, kb) >= 1) {
		ans = std::max(ans,
			{ std::min(ka, kb) * 2 + get_max_cnt(ka, kb, q-2).first, MOVE_TYPE::xAx_xBx }
		);
	}

	return mem[{ka, kb, q}] = ans;
}

int get_max_cnt(int q) {
	return get_max_cnt(1, 0, q).first+1;
}

}

int main() {
	for (int q = 1; q <= 50; q++) {
		std::cerr << q << ' ' << get_max_cnt(2*q, 2*q, q).first << '\n';
	}

	return 0;
}
