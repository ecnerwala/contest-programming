#include "cyklib.hpp"

#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	pair<int, int64_t> last_pos = {0, circular_shift(0)};
	auto query = [&](int x) -> pair<int, int64_t> {
		assert(x > 0);
		auto v = circular_shift(x);
		return last_pos = {last_pos.first + (v <= last_pos.second), v};
	};
	auto cyc = [&](pair<int, int64_t> x) -> pair<int, int64_t> { return {x.first + 1, x.second}; };

	pair<int, int64_t> first_pos = last_pos;
	int s = 0;
	while (query(1 << s) < cyc(first_pos)) s++;
	// mi < N <= ma
	int mi = (1 << s)-1;
	int ma = (1 << (s+1))-1;

	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		pair<int, int64_t> dest = cyc(last_pos);
		query(md / 2);
		if (query((md+1)/2) < dest) {
			mi = md;
		} else {
			ma = md;
		}
	}

	give_answer(ma);

	return 0;
}
