#include<bits/stdc++.h>

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
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);

	using i64 = int64_t;
	using i128 = __int128_t;
	auto solve = std::y_combinator([&](auto self, i64 L, i64 R) -> tuple<i64, i64, i64> {
		if (R > L) {
			auto res = self(R, L);
			swap(get<1>(res), get<2>(res));
			return res;
		}
		assert(L >= R);
		i64 taken;
		{
			auto is_good = [&](i64 cnt) -> bool { return L - i128(cnt) * i128(cnt + 1) / 2 * 2 < R; };
			i64 mi = 0, ma = 1;
			while (!is_good(ma)) ma *= 2;
			while (ma - mi > 1) {
				i64 md = (mi + ma) / 2;
				if (is_good(md)) {
					ma = md;
				} else {
					mi = md;
				}
			}
			L -= mi * (mi + 1) / 2 * 2;
			taken = mi;
		}

		assert(L >= R);
		assert(L - (taken+1) * 2 < R);

		{
			auto is_good = [&](i64 cnt2) -> bool {
				i64 cntL = (cnt2 + 1) / 2;
				i64 cntR = (cnt2) / 2;
				return L >= cntL * i128(cntL + taken) * 2 && R >= cntR * i128(cntR + taken + 1) * 2;
			};
			i64 mi = 0, ma = 1;
			while (is_good(ma)) ma *= 2;
			while (ma - mi > 1) {
				i64 md = (mi + ma) / 2;
				if (is_good(md)) {
					mi = md;
				} else {
					ma = md;
				}
			}
			i64 cntL = (mi + 1) / 2;
			i64 cntR = (mi) / 2;
			L -= cntL * (cntL + taken) * 2;
			R -= cntR * (cntR + taken + 1) * 2;
			taken += mi;
		}
		return {taken, L, R};
	});

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		i64 L, R; cin >> L >> R;
		i64 n, l, r; tie(n, l, r) = solve(2 * L + 1, 2 * R);

		cout << "Case #" << case_num << ": " << n << ' ' << l / 2 << ' ' << r / 2 << '\n';
	}

	return 0;
}
