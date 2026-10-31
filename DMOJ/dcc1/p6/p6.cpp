#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> P(N);
	for (auto& p : P) { cin >> p; p--; }

	std::vector<std::vector<int8_t>> ops;
	ops.reserve(50);

	std::y_combinator([&](auto self, int level, int lo1, int hi1, int lo2, int hi2) -> void {
		if ((hi2 - lo2) == (hi1 - lo1) + 1) {
			assert(hi1 == lo2);
		} else {
			assert((hi2 - lo2) == (hi1 - lo1));
		}
		int sz = (hi1 - lo1) + (hi2 - lo2);
		if (sz == 1) {
			return;
		} else if (sz == 2) {
			if (P[lo1] != lo1) {
				if (int(ops.size()) <= level) ops.emplace_back(N);
				ops[level][lo1] = true;
				ops[level][lo2] = true;
			}
			return;
		} else {
			int s_outer = (hi1 - lo1 + 1) / 2;
			int md1 = lo1 + s_outer;
			int md2 = hi2 - s_outer;
			auto is_inner = [&](int a) -> bool {
				assert((lo1 <= a && a < hi1) || (lo2 <= a && a < hi2));
				return md1 <= a && a < md2;
			};

			int swap_lo1 = 0;
			for (int i = lo1; i < md1; i++) swap_lo1 += is_inner(P[i]);
			int swap_hi1 = 0;
			for (int i = md1; i < hi1; i++) swap_hi1 += !is_inner(P[i]);
			int swap_lo2 = 0;
			for (int i = lo2; i < md2; i++) swap_lo2 += !is_inner(P[i]);
			int swap_hi2 = 0;
			for (int i = md2; i < hi2; i++) swap_hi2 += is_inner(P[i]);
			// now pick which guy wants to swap
			assert(swap_lo1 + swap_hi2 == swap_hi1 + swap_lo2);
			// we need to clear 2 of them so we can just swap to finish
			// clear the los entirely: flip 
			if (swap_lo1 >= swap_lo2) {
				for (int i = lo1; i < md1; i++) {
					if (is_inner(P[i])) ops[level][i] = true;
				}
				for (int i = lo2; i < hi2; i++) {
					if (!is_inner(P[i])) ops[level][i] = true;
				}
			} else {
			}
			
			self(level, md1, hi1, lo2, md2);
			self(level, lo1, md1, md2, hi2);
		}
	})(0, 0, N/2, N/2, N);
	cout << ops.size() << '\n';
	for (const auto& op : ops) {
		for (int i = 0; i < N; i++) { cout << int(op[i]); } cout << '\n';
	}

	return 0;
}
