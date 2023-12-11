#include <bits/stdc++.h>

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
	explicit point(int a_) : a(a_) { assert(a >= -1); }

	explicit operator bool () { return bool(a); }

	// This is useful so you can directly do array indices
	/* implicit */ operator int() const { return a; }

	point c(bool z) const {
		return point((a<<1)|z);
	}

	point operator [] (bool z) const {
		return c(z);
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

	point& operator ++ () { ++a; return *this; }
	point operator ++ (int) { return point(a++); }
	point& operator -- () { --a; return *this; }
	point operator -- (int) { return point(a--); }
};

struct range {
	int a, b;
	range() : a(1), b(1) {}
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

	// Iterate over the range from outside-in.
	//   Calls f(point a)
	template <typename F> void for_each(F f) const {
		for (int x = a, y = b; x < y; x >>= 1, y >>= 1) {
			if (x & 1) f(point(x++));
			if (y & 1) f(point(--y));
		}
	}

	// Iterate over the range from outside-in.
	//   Calls f(point a, bool is_right)
	template <typename F> void for_each_with_side(F f) const {
		for (int x = a, y = b; x < y; x >>= 1, y >>= 1) {
			if (x & 1) f(point(x++), false);
			if (y & 1) f(point(--y), true);
		}
	}

	// Iterate over the range from left to right.
	//    Calls f(point)
	template <typename F> void for_each_l_to_r(F f) const {
		int anc_depth = log_2((a-1) ^ b);
		int anc_msk = (1 << anc_depth) - 1;
		for (int v = (-a) & anc_msk; v; v &= v-1) {
			int i = __builtin_ctz(v);
			f(point(((a-1) >> i) + 1));
		}
		for (int v = b & anc_msk; v; ) {
			int i = log_2(v);
			f(point((b >> i) - 1));
			v ^= (1 << i);
		}
	}

	// Iterate over the range from right to left.
	//    Calls f(point)
	template <typename F> void for_each_r_to_l(F f) const {
		int anc_depth = log_2((a-1) ^ b);
		int anc_msk = (1 << anc_depth) - 1;
		for (int v = b & anc_msk; v; v &= v-1) {
			int i = __builtin_ctz(v);
			f(point((b >> i) - 1));
		}
		for (int v = (-a) & anc_msk; v; ) {
			int i = log_2(v);
			f(point(((a-1) >> i) + 1));
			v ^= (1 << i);
		}
	}

	template <typename F> void for_parents_down(F f) const {
		int x = a, y = b;
		if ((x ^ y) > x) { x <<= 1, std::swap(x, y); }
		int dx = __builtin_ctz(x);
		int dy = __builtin_ctz(y);
		int anc_depth = log_2((x-1) ^ y);
		for (int i = log_2(x); i > dx; i--) {
			f(point(x >> i));
		}
		for (int i = anc_depth; i > dy; i--) {
			f(point(y >> i));
		}
	}

	template <typename F> void for_parents_up(F f) const {
		int x = a, y = b;
		if ((x ^ y) > x) { x <<= 1, std::swap(x, y); }
		int dx = __builtin_ctz(x);
		int dy = __builtin_ctz(y);
		int anc_depth = log_2((x-1) ^ y);
		for (int i = dx+1; i <= anc_depth; i++) {
			f(point(x >> i));
		}
		for (int v = y >> (dy+1); v; v >>= 1) {
			f(point(v));
		}
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
		if (N == 0) return range();
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

	int get_node_split(point pt) const {
		int a = int(pt);
		assert(1 <= a && a < N);
		int l = __builtin_clz(2*a+1) - __builtin_clz(2*N-1);
		int x = (2*a+1) << l;
		assert(S <= x && x < 2*S);
		return (x >= 2 * N ? (x>>1) + N : x) - S;
	}

	int get_node_size(point pt) const {
		auto bounds = get_node_bounds(pt);
		return bounds[1] - bounds[0];
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
		if (N == 0) return range();
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

	// Returns the split point of the node, such that 1 <= s <= N.
	int get_node_split(point pt) const {
		int a = int(pt);
		assert(1 <= a && a < N);
		return get_node_bounds(pt.c(0))[1];
	}

	int get_node_size(point pt) const {
		auto bounds = get_node_bounds(pt);
		int r = bounds[1] - bounds[0];
		return r > 0 ? r : r + N;
	}
};

} // namespace seg_tree

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// for each starting point, there's some point where the hall condition becomes bad, where we have more days than lecturers
	int N; cin >> N;
	struct inter_t {
		int64_t st, en;
	};
	std::vector<inter_t> intervals(N);
	for (auto& [st, en] : intervals) {
		cin >> st >> en;
		assert(st <= en);
	}

	const int Q = N;

	// first filter intervals down to some matching
	{
		std::vector<int64_t> en_matching; en_matching.reserve(N);
		std::vector<int64_t> st_matching; st_matching.reserve(N);

		for (int z = 0; z < 2; z++) {
			std::sort(intervals.begin(), intervals.end(), [](auto a, auto b) { return a.en > b.en; });
			auto it = intervals.begin();
			std::priority_queue<std::pair<int, int>> pq;
			for (int64_t lo = it->en; !pq.empty() || it != intervals.end(); lo--) {
				if (pq.empty()) {
					// jump ahead
					assert(it != intervals.end());
					assert(it->en <= lo);
					lo = it->en;
				}
				for (; it != intervals.end() && it->en >= lo; ++it) {
					pq.push({it->st, it - intervals.begin()});
				}
				while (!pq.empty() && pq.top().first > lo) {
					pq.pop();
				}
				if (!pq.empty()) {
					en_matching.push_back(lo);
					pq.pop();
				}
			}

			for (auto& a : intervals) a = {-a.en, -a.st};

			for (auto& v : st_matching) v = -v;
			std::reverse(st_matching.begin(), st_matching.end());

			for (auto& v : en_matching) v = -v;
			std::reverse(en_matching.begin(), en_matching.end());

			std::swap(st_matching, en_matching);
		}
		std::reverse(st_matching.begin(), st_matching.end());
		std::reverse(en_matching.begin(), en_matching.end());
		assert(st_matching.size() == en_matching.size());
		N = int(en_matching.size());
		intervals.resize(N);
		for (int i = 0; i < N; i++) {
			intervals[i] = {st_matching[i], en_matching[i]};
			cerr << st_matching[i] << ' ' << en_matching[i] << '\n';
		}
	}

	std::vector<int> ans(Q+1);
	int first_inter = N;
	int cur_cnt = 0;
	for (int64_t lo = intervals.back().en; true; lo--) {
		if (first_inter > 0 && lo == intervals[first_inter-1].en) {
			first_inter--;
			cur_cnt++;
		}
		while (cur_cnt > 0 && intervals[first_inter + cur_cnt - 1].st == lo + cur_cnt) {
			cur_cnt --;
		}
		ans[cur_cnt] ++;
		if (first_inter == 0 && cur_cnt == 0) break;
	}

	for (int i = Q; i >= 1; i--) {
		ans[i-1] += ans[i];
	}
	for (int i = 1; i <= Q; i++) {
		cout << ans[i] << '\n';
	}

	if constexpr (false) {
	auto it = intervals.begin();
	int V = int(1e5);
	seg_tree::in_order_layout layout(V);
	struct seg_t {
		int64_t tot = 0;
		int64_t mi = 0;
	};
	// no need to initialize
	std::vector<seg_t> seg(2*V);
	auto update_node = [&](seg_tree::point a) -> void {
		seg[a].tot = seg[a.c(0)].tot + seg[a.c(1)].tot;
		seg[a].mi = std::min(seg[a.c(0)].mi, seg[a.c(0)].tot + seg[a.c(1)].mi);
	};
	// we pick some intervals, then we must waste at least # intervals - size of union
	int cur_bound = V;
	std::vector<int64_t> ans(N+1);
	// bad if # of things covering each - # things covering each pair - # pts < 0
	// all intervals that cover the left endpoint must cover the next one too
	for (int lo = V-1; lo >= 0; lo--) {
		for (; it != intervals.end() && it->en >= lo; ++it) {
			//if (skip_inter[it - intervals.begin()]) continue;
			assert(it->en == lo);
			// +1 at it->st;
			assert(1 <= it->st && it->st <= lo);
			seg_tree::point a = layout.get_point(int(it->st));
			seg[a].tot++;
			seg[a].mi = std::min<int64_t>(0, seg[a].tot);
			a.for_parents_up(update_node);
		}
		// -1 at lo
		{
			seg_tree::point a = layout.get_point(lo);
			seg[a].tot--;
			seg[a].mi = std::min<int64_t>(0, seg[a].tot);
			a.for_parents_up(update_node);
		}

		// find the first point with negative sum
		assert(seg[1].mi <= 0);
		int hi;
		if (seg[1].mi == 0) {
			hi = V;
			assert(cur_bound <= hi);
		} else {
			seg_tree::point a(1);
			int64_t lhs = 0;
			while (a < layout.N) {
				assert(lhs + seg[a].mi < 0);
				if (lhs + seg[a.c(0)].mi < 0) {
					a = a.c(0);
				} else {
					lhs += seg[a.c(0)].tot;
					assert(lhs + seg[a.c(1)].mi < 0);
					a = a.c(1);
				}
			}
			assert(lhs + seg[a].mi < 0);
			assert(lhs + seg[a].mi == -1);
			assert(lhs == 0);
			hi = layout.get_leaf_index(a);
		}
		assert(hi >= lo);
		// lo..hi is bad
		cur_bound = std::min(cur_bound, hi);
		//cerr << lo << ' ' << cur_bound - lo << '\n';
		assert(cur_bound >= lo);
		assert(cur_bound - lo <= N);
		ans[0]++;
		ans[cur_bound - lo]--;
		if (lo == 0 || lo == V-1) assert(cur_bound == lo);
	}
	for (int i = 0; i < N; i++) {
		ans[i+1] += ans[i];
	}
	assert(ans[N] == 0);
	for (int i = 0; i < N; i++) {
		cout << ans[i] << '\n';
	}
	}
	return 0;
}
