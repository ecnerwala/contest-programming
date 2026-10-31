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

	int CASES; cin >> CASES;
	while (CASES--) {
		int N; int64_t T; cin >> N >> T;
		struct prob_t {
			int64_t a; int b; int idx;
		};
		std::vector<prob_t> probs(N);
		for (auto& p : probs) cin >> p.a >> p.b;
		for (int i = 0; i < N; i++) {
			probs[i].idx = i;
		}
		std::sort(probs.begin(), probs.end(), [&](const auto& x, const auto& y) -> bool {
			return x.a < y.a;
		});

		std::vector<std::vector<int>> evts(N+1);
		for (int i = 0; i < N; i++) {
			evts[probs[i].b].push_back(i);
		}

		seg_tree::in_order_layout layout(N);
		struct seg_t {
			int cnt = 0;
			int64_t sum = 0;
		};
		auto update_node = [&](std::vector<seg_t>& seg, seg_tree::point a) -> void {
			seg[a].cnt = seg[a.c(0)].cnt + seg[a.c(1)].cnt;
			seg[a].sum = seg[a.c(0)].sum + seg[a.c(1)].sum;
		};
		std::vector<seg_t> seg_goods(2*N);
		auto update_good_node = [&](seg_tree::point a) -> void { update_node(seg_goods, a); };
		std::vector<seg_t> seg_bads(2*N);
		auto update_bad_node = [&](seg_tree::point a) -> void { update_node(seg_bads, a); };

		for (int i = 0; i < N; i++) {
			seg_tree::point a = layout.get_point(i);
			seg_bads[a].cnt = 1;
			seg_bads[a].sum = probs[i].a;
		}
		for (seg_tree::point a(N-1); a > 0; a--) {
			update_bad_node(a);
		}
		auto get_sum = [&](const std::vector<seg_t>& seg, int cnt) -> int64_t {
			if (cnt == 0) return 0;
			assert(cnt > 0);
			if (cnt == seg[1].cnt) return seg[1].sum;

			seg_tree::point a(1);
			int64_t res = 0;
			while (cnt > 0) {
				assert(a < N);
				assert(0 < cnt && cnt < seg[a].cnt);
				if (cnt >= seg[a.c(0)].cnt) {
					cnt -= seg[a.c(0)].cnt;
					res += seg[a.c(0)].sum;
					a = a.c(1);
				} else {
					a = a.c(0);
				}
			}
			return res;
		};

		std::vector<int64_t> pref(N+1);
		for (int i = 0; i < N; i++) {
			pref[i+1] = pref[i] + probs[i].a;
		}

		int best_ans = 0;
		int best_k = 0;
		for (int k = 1; k <= N; k++) {
			if (pref[k] > T) break;
			for (int i : evts[k]) {
				auto a = layout.get_point(i);
				std::swap(seg_goods[a], seg_bads[a]);
				a.for_parents_up(update_good_node);
				a.for_parents_up(update_bad_node);
			}
			int mi = 0, ma = std::min(k, seg_goods[1].cnt) + 1;
			{
				auto rng = layout.get_range(0, k);
				rng.for_each([&](auto a) -> void {
					mi += seg_goods[a].cnt;
				});
			}
			// we should take at least the prefix
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				int cnt_good = md;
				int cnt_bad = k - cnt_good;
				auto s_good = get_sum(seg_goods, cnt_good);
				auto s_bad = get_sum(seg_bads, cnt_bad);
				if (s_good + s_bad <= T) {
					mi = md;
				} else {
					ma = md;
				}
			}
			if (mi > best_ans) {
				best_ans = mi;
				best_k = k;
			}
		}

		cout << best_ans << '\n';
		std::vector<int> ans; ans.reserve(best_k);
		{
			int num_good = best_ans;
			int num_bad = best_k - num_good;
			for (int i = 0; i < N; i++) {
				if (probs[i].b <= best_k) {
					// it's good
					if (num_good) {
						num_good--;
						ans.push_back(probs[i].idx);
					}
				} else {
					if (num_bad) {
						num_bad--;
						ans.push_back(probs[i].idx);
					}
				}
			}
		}
		assert(int(ans.size()) == best_k);
		cout << best_k << '\n';
		for (int i = 0; i < best_k; i++) {
			cout << ans[i]+1 << " \n"[i+1==best_k];
		}
	}

	return 0;
}
