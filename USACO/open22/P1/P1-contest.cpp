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

	std::vector<int> A(N); for (auto& a : A) cin >> a;

	std::vector<std::pair<int, int>> seg(2*N);
	for (int i = 0; i < N; i++) {
		seg[N+i] = {A[i], i};
	}
	for (int a = N-1; a; a--) seg[a] = std::max(seg[2*a], seg[2*a+1]);
	auto get_min = [&](int l, int r) -> std::pair<int, int> {
		std::pair<int, int> res(-1, -1);
		for (auto a = N+l, b = N+r; a < b; a >>= 1, b >>= 1) {
			if (a & 1) res = std::max(res, seg[a++]);
			if (b & 1) res = std::max(res, seg[--b]);
		}
		return res;
	};

	int64_t ans = 0;

	auto c2 = [&](int a) -> int64_t {
		assert(a >= 0);
		return int64_t(a) * int64_t(a+1) / 2;
	};
	std::vector<std::array<int, 2>> ranges; ranges.reserve(N);
	auto get_num_segs = [&](int lo, int hi) -> int64_t {
		int64_t res = 0;
		for (int i = lo; i < hi; i++) {
			res += c2(ranges[i][1] - ranges[i][0]);
		}
		for (int i = lo; i+1 < hi; i++) {
			res -= c2(ranges[i][1] - ranges[i+1][0]);
		}
		return res;
	};
	std::y_combinator([&](auto self, int l, int r, int max_val) -> int64_t {
		assert(r > l);
		if (r - l == 1) {
			assert(A[l] <= max_val);
			ans += A[l];
			ranges.push_back({l, r});
			return 1;
		}
		int st = int(ranges.size());
		int cur_val = get_min(l, r).first;
		int64_t num_segs = 0;
		for (int cur_l = l; cur_l < r; ) {
			auto [v, idx] = get_min(cur_l, r);
			if (v != cur_val) {
				idx = r;
			} else if (idx == cur_l) {
				idx++;
			}
			num_segs += self(cur_l, idx, cur_val);
			cur_l = idx;
		}

		while (cur_val < max_val) {
			int en = int(ranges.size());
			if (en - st == 1) {
				cur_val = max_val;
				break;
			}
			cur_val++;
			int n_en = st;
			for (int i = st, j = st+1; j < en; ) {
				while (ranges[i][1] < ranges[j][0]) i++;
				assert(i < j);
				while (j+1 < en && ranges[j+1][0] <= ranges[i][1]) j++;
				ranges[n_en++] = {ranges[i][0], ranges[j][1]};
				j++;
				i++;
			}
			en = n_en;
			ranges.resize(en);
			int64_t n_num_segs = get_num_segs(st, en);
			ans += (n_num_segs - num_segs) * cur_val;
			num_segs = n_num_segs;
		}
		return num_segs;
	})(0, N, int(1e9));
	cout << ans << '\n';

	return 0;
}
