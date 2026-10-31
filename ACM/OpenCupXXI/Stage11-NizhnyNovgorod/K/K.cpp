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

namespace seg_tree {

// Floor of log_2(a); index of highest 1-bit
inline int log_2(int a) {
	return a ? (8 * sizeof(a)) - 1 - __builtin_clz(a) : -1;
}

inline int next_pow_2(int a) {
	assert(a > 0);
	return 1 << log_2(2*a-1);
}

struct point {
	int a;
	point() : a(0) {}
	explicit point(int a_) : a(a_) { assert(a >= 0); }

	explicit operator bool () { return bool(a); }

	// This is useful so you can directly do array indices
	/* implicit */ operator int() const { return a; }

	point c(bool z) const {
		return point((a<<1)|z);
	}

	point p() const {
		return point(a>>1);
	}

	friend std::ostream& operator << (std::ostream& o, const point& p) { return o << int(p); }

	template <typename F> void for_each(F f) const {
		for (int v = a; v > 0; v >>= 1) {
			f(point(v));
		}
	}

	template <typename F> void for_parents_down(F f) const {
		// strictly greater than 0
		for (int L = log_2(a); L > 0; L--) {
			f(point(a >> L));
		}
	}

	template <typename F> void for_parents_up(F f) const {
		for (int v = a >> 1; v > 0; v >>= 1) {
			f(point(v));
		}
	}
};

struct range {
	int a, b;
	range() : a(0), b(0) {}
	range(int a_, int b_) : a(a_), b(b_) {
		assert(1 <= a && a <= b && b <= 2 * a);
	}
	explicit range(std::array<int, 2> r) : range(r[0], r[1]) {}

	explicit operator std::array<int, 2>() const {
		return {a,b};
	}

	const int& operator[] (bool z) const {
		return z ? b : a;
	}

	friend std::ostream& operator << (std::ostream& o, const range& r) { return o << "[" << r.a << ".." << r.b << ")"; }

	template <typename F> void for_each(F f) const {
		for (int x = a, y = b; x < y; x >>= 1, y >>= 1) {
			if (x & 1) f(point(x++), false);
			if (y & 1) f(point(--y), true);
		}
	}

	template <typename F> void for_parents_down(F f) const {
		int x = a >> __builtin_ctz(a);
		int y = b >> __builtin_ctz(b);

		// TODO: dedup
		point(x).for_parents_down(f);
		point(y).for_parents_down(f);
	}

	template <typename F> void for_parents_up(F f) const {
		int x = a >> __builtin_ctz(a);
		int y = b >> __builtin_ctz(b);

		// TODO: dedup
		point(x).for_parents_up(f);
		point(y).for_parents_up(f);
	}
};

struct in_order_layout {
	// Alias them in for convenience
	using point = seg_tree::point;
	using range = seg_tree::range;

	int N, S;
	in_order_layout() : N(0), S(0) {}
	in_order_layout(int N_) : N(N_), S(N ? next_pow_2(N) : 0) {}

	point get_point(int a) const {
		assert(0 <= a && a < N);
		a += S;
		return point(a >= 2 * N ? a - N : a);
	}

	range get_range(int a, int b) const {
		assert(0 <= a && a <= b && b <= N);
		a += S, b += S;
		return range((a >= 2 * N ? 2*(a-N) : a), (b >= 2 * N ? 2*(b-N) : b));
	}

	range get_range(std::array<int, 2> p) const {
		return get_range(p[0], p[1]);
	}

	int get_leaf_index(point pt) const {
		int a = int(pt);
		assert(N <= a && a < 2 * N);
		return (a < S ? a + N : a) - S;
	}

	std::array<int, 2> get_node_bounds(point pt) const {
		int a = int(pt);
		assert(1 <= a && a < 2 * N);
		int l = __builtin_clz(a) - __builtin_clz(2*N-1);
		int x = a << l, y = (a+1) << l;
		assert(S <= x && x < y && y <= 2*S);
		return {(x >= 2 * N ? (x>>1) + N : x) - S, (y >= 2 * N ? (y>>1) + N : y) - S};
	}

	int get_node_size(point pt) const {
		auto [x, y] = get_node_bounds(pt);
		return y - x;
	}
};

struct circular_layout {
	// Alias them in for convenience
	using point = seg_tree::point;
	using range = seg_tree::range;

	int N;
	circular_layout() : N(0) {}
	circular_layout(int N_) : N(N_) {}

	point get_point(int a) const {
		assert(0 <= a && a < N);
		return point(N + a);
	}

	range get_range(int a, int b) const {
		assert(0 <= a && a <= b && b <= N);
		return range(N + a, N + b);
	}

	range get_range(std::array<int, 2> p) const {
		return get_range(p[0], p[1]);
	}

	int get_leaf_index(point pt) const {
		int a = int(pt);
		assert(N <= a && a < 2 * N);
		return a - N;
	}

	// Returns {x,y} so that 0 <= x < N and 1 <= y <= N
	// If the point is non-wrapping, then 0 <= x < y <= N
	std::array<int, 2> get_node_bounds(point pt) const {
		int a = int(pt);
		assert(1 <= a && a < 2 * N);
		int l = __builtin_clz(a) - __builtin_clz(2*N-1);
		int S = next_pow_2(N);
		int x = a << l, y = (a+1) << l;
		assert(S <= x && x < y && y <= 2*S);
		return {(x >= 2 * N ? x >> 1 : x) - N, (y > 2 * N ? y >> 1 : y) - N};
	}

	int get_node_size(point pt) const {
		auto [x, y] = get_node_bounds(pt);
		int r = y - x;
		return r > 0 ? r : r + N;
	}
};

} // namespace seg_tree

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

std::mt19937 mt(48);

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t S = 0;
	auto read_encoded = [&S]() -> int {
		int64_t v; cin >> v;
		v ^= S;
		assert(0 <= v && v <= int(1e6));
		return int(v);
	};

	const int X = 1 << 18;
	const int Y = 2.1e5;
	const int INF = 1e9;
	struct seg_node {
		set<int> ys;
		int val;
	};

	seg_tree::circular_layout layout(X);

	vector<seg_node> seg(2*X);
	auto update_node = [&](int a) {
		if (seg[a].ys.empty()) {
			seg[a].val = INF;
		} else {
			seg[a].val = *seg[a].ys.begin();
		}
		if (a < X) {
			seg[a].val = min(seg[a].val, max(seg[2*a].val, seg[2*a+1].val));
		}
	};

	vector<set<pair<int, int>>> intervals(Y);
	for (int y = 0; y < Y; y++) {
		intervals[y].insert({0, X});
		seg[1].ys.insert(y);
	}

	for (int a = 2*X-1; a > 0; a--) {
		update_node(a);
	}

	auto seg_insert = [&](int y, int l, int r) {
		assert(l < r);
		auto rng = layout.get_range(l, r);
		rng.for_each([&](auto a, bool) { seg[a].ys.insert(y); update_node(a); });
		rng.for_parents_up(update_node);
	};
	auto seg_erase = [&](int y, int l, int r) {
		assert(l < r);
		auto rng = layout.get_range(l, r);
		rng.for_each([&](auto a, bool) { seg[a].ys.erase(y); update_node(a); });
		rng.for_parents_up(update_node);
	};

	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int y = read_encoded();
			int l = read_encoded();
			int r = read_encoded();
			r++;

			auto& y_intervals = intervals[y];
			for (int x : {l, r}) {
				auto it = y_intervals.lower_bound({x, -1});
				if (it != y_intervals.begin()) {
					--it;
					assert(it->first < x);
					if (it->second > x) {
						int l2 = it->first, r2 = it->second;
						seg_erase(y, l2, r2);
						y_intervals.erase(it);

						y_intervals.insert({l2, x});
						y_intervals.insert({x, r2});
						seg_insert(y, l2, x);
						seg_insert(y, x, r2);
					}
				}
			};
			auto it = y_intervals.lower_bound({l, -1});
			while (it != y_intervals.end() && it->second <= r) {
				seg_erase(y, it->first, it->second);
				it = y_intervals.erase(it);
			}
		} else if (op == 2) {
			int l = read_encoded();
			int r = read_encoded();
			r++;

			int ans = std::y_combinator([&](auto self, int a, int lo, int hi) -> int {
				if (l <= lo && hi <= r) {
					// just take the whole range
					return seg[a].val;
				}
				int md = (lo + hi) / 2;
				int res = -1;
				if (l < md) {
					res = max(res, self(2*a+0, lo, md));
				}
				if (md < r) {
					res = max(res, self(2*a+1, md, hi));
				}
				if (!seg[a].ys.empty()) res = min(res, *seg[a].ys.begin());
				return res;
			})(1, 0, X);

			cout << ans << '\n';
			S += ans;
		}
	}

	return 0;
}
