#include <bits/stdc++.h>

/** Binary-indexed tree
 *
 *  A binary indexed tree with N nodes of type T provides the
 *  following two functions for 0 <= i <= N:
 *
 *      prefix(int i) -> prefix_iterator<T>
 *      suffix(int i) -> suffix_iterator<T>
 *
 *  such that size(suffix(i) intersect prefix(j)) = (1 if i < j else 0).
 *  Furthermore, the resulting lists always have size at most log_2(N).
 *
 *  This can be used to implement either point-update/(prefix|suffix)-query or
 *  (prefix|suffix)-update/point-query over a virtual array of size N of a
 *  commutative monoid. This can be generalized to implement
 *  point-update/range-query or range-update/point-query over a virtual array
 *  of size N of a commutative group.
 *
 *  With 0-indexed data, prefixes are more natural:
 *   * For range update/query, use for_prefix for the ranges and for_suffix for the points.
 *   * For prefix update/query, no change.
 *   * For suffix update/query, use for_prefix(point + 1); 1-index the data.
 */
template <typename T> class binary_indexed_tree {
private:
	std::vector<T> dat;
public:
	binary_indexed_tree() {}
	explicit binary_indexed_tree(size_t N) : dat(N) {}
	binary_indexed_tree(size_t N, const T& t) : dat(N, t) {}

	size_t size() const { return dat.size(); }
	const std::vector<T>& data() const { return dat; }
	std::vector<T>& data() { return dat; }

private:
	template <typename I, typename S = I> struct iterator_range {
	private:
		I begin_;
		S end_;
	public:
		iterator_range() : begin_(), end_() {}
		iterator_range(const I& begin__, const S& end__) : begin_(begin__), end_(end__) {}
		iterator_range(I&& begin__, S&& end__) : begin_(begin__), end_(end__) {}
		I begin() const { return begin_; }
		S end() const { return end_; }
	};

public:
	class const_suffix_iterator {
	private:
		const T* dat;
		int a;
		const_suffix_iterator(const T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const const_suffix_iterator& i, const const_suffix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a < j.a;
		}
		const_suffix_iterator& operator ++ () {
			a |= a+1;
			return *this;
		}
		const T& operator * () const {
			return dat[a];
		}
	};
	using const_suffix_range = iterator_range<const_suffix_iterator>;
	const_suffix_range suffix(int a) const {
		assert(0 <= a && a <= int(dat.size()));
		return const_suffix_range{const_suffix_iterator{dat.data(), a}, const_suffix_iterator{nullptr, int(dat.size())}};
	}

	class suffix_iterator {
	private:
		T* dat;
		int a;
		suffix_iterator(T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const suffix_iterator& i, const suffix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a < j.a;
		}
		suffix_iterator& operator ++ () {
			a |= a+1;
			return *this;
		}
		T& operator * () const {
			return dat[a];
		}
	};
	using suffix_range = iterator_range<suffix_iterator>;
	suffix_range suffix(int a) {
		assert(0 <= a && a <= int(dat.size()));
		return suffix_range{suffix_iterator{dat.data(), a}, suffix_iterator{nullptr, int(dat.size())}};
	}

	class const_prefix_iterator {
	private:
		const T* dat;
		int a;
		const_prefix_iterator(const T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const const_prefix_iterator& i, const const_prefix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a > 0;
		}
		const_prefix_iterator& operator ++ () {
			a &= a-1;
			return *this;
		}
		const T& operator * () const {
			return dat[a-1];
		}
	};
	using const_prefix_range = iterator_range<const_prefix_iterator>;
	const_prefix_range prefix(int a) const {
		return const_prefix_range{const_prefix_iterator{dat.data(), a}, const_prefix_iterator{nullptr, 0}};
	}

	class prefix_iterator {
	private:
		T* dat;
		int a;
		prefix_iterator(T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const prefix_iterator& i, const prefix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a > 0;
		}
		prefix_iterator& operator ++ () {
			a &= a-1;
			return *this;
		}
		T& operator * () const {
			return dat[a-1];
		}
	};
	using prefix_range = iterator_range<prefix_iterator>;
	prefix_range prefix(int a) {
		return prefix_range{prefix_iterator{dat.data(), a}, prefix_iterator{nullptr, 0}};
	}
};

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

	int get_node_split(point pt) const {
		int a = int(pt);
		assert(1 <= a && a < N);
		int l = __builtin_clz(2*a+1) - __builtin_clz(2*N-1);
		int x = (2*a+1) << l;
		assert(S <= x && x < 2*S);
		return (x >= 2 * N ? (x>>1) + N : x) - S;
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

	// Returns the split point of the node, such that 1 <= s <= N.
	int get_node_split(point pt) const {
		int a = int(pt);
		assert(1 <= a && a < N);
		return get_node_bounds(pt.c(0))[1];
	}

	int get_node_size(point pt) const {
		auto [x, y] = get_node_bounds(pt);
		int r = y - x;
		return r > 0 ? r : r + N;
	}
};

} // namespace seg_tree

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

	int N, Q; cin >> N >> Q;
	vector<int> A(N); for (auto& a : A) cin >> a;

	vector<pair<array<int, 2>, int>> queries(Q);
	for (int q = 0; q < Q; q++) {
		auto& r = queries[q].first;
		cin >> r[0] >> r[1]; r[0]--;
		queries[q].second = q;
	}

	vector<int> ans(Q, -1);

	sort(queries.begin(), queries.end());

	vector<vector<int>> A_inds(N);
	binary_indexed_tree<int> cur_pt_cnt(N);
	for (int i = 0; i < N; i++) {
		if (A[i] < N) {
			A_inds[A[i]].push_back(i);
			for (auto& a : cur_pt_cnt.suffix(i)) a++;
		}
	}

	seg_tree::in_order_layout layout(Q);

	struct seg_node {
		int max_val = -1;
		int lazy_val = 0;
		bool is_empty = true;
		array<array<int, 2>, 2> bounds;
	};
	vector<seg_node> seg(2*Q);

	auto update_node = [&](seg_tree::point p) {
		assert(p < Q);
		assert(seg[p].lazy_val == 0);
		if (seg[p.c(0)].is_empty && seg[p.c(1)].is_empty) {
			seg[p].is_empty = true;
			seg[p].max_val = -1;
		} else if (seg[p.c(0)].is_empty) {
			seg[p].is_empty = false;
			seg[p].max_val = seg[p.c(1)].max_val;
			seg[p].bounds = seg[p.c(1)].bounds;
		} else if (seg[p.c(1)].is_empty) {
			seg[p].is_empty = false;
			seg[p].max_val = seg[p.c(0)].max_val;
			seg[p].bounds = seg[p.c(0)].bounds;
		} else {
			seg[p].is_empty = false;
			seg[p].max_val = max(seg[p.c(0)].max_val, seg[p.c(1)].max_val);
			seg[p].bounds[0] = seg[p.c(0)].bounds[0];
			seg[p].bounds[1] = seg[p.c(1)].bounds[1];
		}
	};

	auto downdate_node = [&](seg_tree::point p) {
		assert(p < Q);
		int v = seg[p].lazy_val;
		if (v) {
			for (int z : {0, 1}) {
				if (!seg[p.c(z)].is_empty) {
					seg[p.c(z)].lazy_val += v;
					seg[p.c(z)].max_val += v;
				}
			}
			seg[p].lazy_val = 0;
		}
	};

	vector<pair<int, int>> to_insert(2*Q);
	for (int a = 2*Q-1; a; a--) {
		if (a >= Q) {
			int q = layout.get_leaf_index(seg_tree::point(a));
			to_insert[a] = {queries[q].first[1], q};
		} else {
			to_insert[a] = max(to_insert[2*a], to_insert[2*a+1]);
		}
	}
	auto check_alive = [&](int prv, int nxt) {
		while (true) {
			auto rng = layout.get_range(prv+1, nxt);
			pair<int, int> res(-1, -1);
			rng.for_each([&](auto p, bool) { res = max(res, to_insert[p]); });
			if (res.second == -1) return;
			if (prv != -1 && res.first < queries[prv].first[1]) return;

			int q = res.second;
			auto p = layout.get_point(q);

			to_insert[p] = {-1, -1};
			p.for_parents_up([&](auto x) { to_insert[x] = max(to_insert[x.c(0)], to_insert[x.c(1)]); });

			p.for_parents_down(downdate_node);

			seg[p].is_empty = false;
			seg[p].bounds = {queries[q].first, queries[q].first};
			seg[p].max_val = 0;
			for (auto a : cur_pt_cnt.prefix(queries[q].first[1])) seg[p].max_val += a;
			for (auto a : cur_pt_cnt.prefix(queries[q].first[0])) seg[p].max_val -= a;

			p.for_parents_up(update_node);

			nxt = q;
		}
	};

	for (int k = N; k >= 0; k--) {
		if (k == N) {
			check_alive(-1, Q);
		} else {
			// Delete everything with value k
			for (int i : A_inds[k]) {
				for (auto& a : cur_pt_cnt.suffix(i)) --a;
				// Do the range decrement in the other tree.
				std::y_combinator([&](auto self, seg_tree::point p) -> void {
					if (seg[p].is_empty) return;
					if (i < seg[p].bounds[0][0] || i >= seg[p].bounds[1][1]) return;
					if (seg[p].bounds[1][0] <= i && i < seg[p].bounds[0][1]) {
						seg[p].max_val--;
						seg[p].lazy_val--;
						return;
					}
					self(p.c(0));
					self(p.c(1));
					seg[p].max_val = max(seg[p.c(0)].max_val, seg[p.c(1)].max_val) + seg[p].lazy_val;
				})(seg_tree::point(1));
			}
		}

		// While there are more finished ones, handle them
		while (seg[1].max_val >= k) {
			seg_tree::point cur(1);
			while (cur < layout.N) {
				assert(seg[cur].max_val >= k);
				downdate_node(cur);
				cur = cur.c(seg[cur.c(1)].max_val >= k);
			}
			int q = layout.get_leaf_index(cur);
			ans[queries[q].second] = k;
			seg[cur].is_empty = true;
			seg[cur].max_val = -1;
			cur.for_parents_up(update_node);

			// Now try to add more things
			int prv = -1;
			layout.get_range(0, q).for_each([&](seg_tree::point p, bool) {
				if (prv == -1 && !seg[p].is_empty) {
					while (p < Q) p = p.c(seg[p.c(1)].is_empty ? 0 : 1);
					prv = layout.get_leaf_index(p);
				}
			});
			int nxt = Q;
			layout.get_range(q+1, Q).for_each([&](seg_tree::point p, bool) {
				if (nxt == Q && !seg[p].is_empty) {
					while (p < Q) p = p.c(seg[p.c(0)].is_empty ? 1 : 0);
					nxt = layout.get_leaf_index(p);
				}
			});
			check_alive(prv, nxt);
		}
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}
	return 0;
}
