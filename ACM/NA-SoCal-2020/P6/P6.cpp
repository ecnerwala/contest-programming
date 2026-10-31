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

	int N, D_vert, D_horiz, D_eyelet, F_min, F_max;
	cin >> N >> D_vert >> D_horiz >> D_eyelet >> F_min >> F_max;

	vector<double> lens;
	std::y_combinator([&](auto self, int cur_loc, double cur_cost, int cur_msk) -> void {
		assert(cur_msk & (1 << cur_loc));
		cur_msk ^= (1 << cur_loc);
		if (cur_loc == N-1) {
			lens.push_back(cur_cost);
			return;
		}
		for (int v = cur_msk; v; v &= v-1) {
			int nxt_loc = __builtin_ctz(v);
			if (abs(cur_loc - nxt_loc) == 1) {
				// we can go vertically
				self(nxt_loc, cur_cost + 2 * D_vert + 2 * D_eyelet, cur_msk);
			}
			self(nxt_loc, cur_cost + 2 * hypot(abs(cur_loc - nxt_loc) * D_vert, D_horiz) + 2 * D_eyelet, cur_msk);
		}
	})(0, D_horiz + 2 * D_eyelet, (1 << N) - 1);
	sort(lens.begin(), lens.end());

	int query_len;
	while (cin >> query_len) {
		int mi = query_len - 2 * F_max;
		int ma = query_len - 2 * F_min;
		cout << (lower_bound(lens.begin(), lens.end(), ma) - lower_bound(lens.begin(), lens.end(), mi)) << '\n';
	}

	return 0;
}
