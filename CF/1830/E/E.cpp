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

	auto solve_sim = [&](std::vector<int> A) -> int64_t {
		int N = int(A.size());
		int64_t ans = 0;
		while (true) {
			int best = -1;
			for (int i = 0; i < N; i++) {
				if (A[i] == i) continue;
				if (best == -1 || A[i] > A[best]) best = i;
			}
			if (best == -1) break;
			int best2 = -1;
			for (int j = best+1; j < N; j++) {
				if (best2 == -1 || A[j] < A[best2]) best2 = j;
			}
			assert(best2 != -1);
			std::swap(A[best], A[best2]);
			ans++;
		}
		return ans;
	};

	auto solve_slow = [&](const std::vector<int>& A) -> int64_t {
		int N = int(A.size());
		int64_t ans = 0;
		std::vector<int> Ainv(N);
		for (int i = 0; i < N; i++) Ainv[A[i]] = i;
		/*
		std::set<std::pair<int, int>> st;
		for (int i = 1; i < N; i++) {
			std::vector<int> bigs;
			std::vector<int> smalls;
			for (int a = 0; a < i; a++) {
				if (A[a] >= i) bigs.push_back(A[a]);
			}
			for (int b = i; b < N; b++) {
				if (A[b] < i) smalls.push_back(A[b]);
			}
			std::sort(bigs.begin(), bigs.end(), std::greater<>());
			std::sort(smalls.begin(), smalls.end());
			assert(bigs.size() == smalls.size());
			for (int z = 0; z < int(bigs.size()); z++) {
				st.insert({bigs[z], smalls[z]});
			}
		}
		return st.size();
		*/
		for (int i = 0; i < N; i++) {
			if (A[i] < i) {
				for (int j = i+1; j < N; j++) {
					if (A[i] < A[j] && A[j] <= i) {
						ans++;
					}
				}
			} else if (A[i] > i) {
				ans++;
				for (int j = 0; j < i; j++) {
					if (i < A[j] && A[j] <= A[i]) {
						ans++;
					}
				}
			}
		}
		return ans;
	};

	/*
	for (int N = 1; N <= 8; N++) {
		cerr << "Testing " << N << '\n';
		std::vector<int> A(N);
		std::iota(A.begin(), A.end(), 0);
		do {
			int64_t sim = solve_sim(A);
			int64_t slo = solve_slow(A);
			if (sim != slo) {
				for (int v : A) { cerr << v << ' ' ;} cerr << '\n';
				cerr << sim << ' ' << slo << '\n';
				assert(false);
			}
		} while (std::next_permutation(A.begin(), A.end()));
		cerr << "Done testing " << N << '\n';
	}
	*/

	int N, Q; cin >> N >> Q;
	std::vector<int> P(N);
	for (auto& p : P) { cin >> p; p--; }
	std::vector<std::array<int, 2>> ops(Q);
	for (auto& [x, y] : ops) { cin >> x >> y; x--, y--; }

	seg_tree::in_order_layout layout(N);
	std::vector<std::vector<int>> coords(2*N);
	{
		std::vector<int> Ptemp = P;
		auto ins = [&](int i) -> void {
			layout.get_point(i).for_each([&](auto a) -> void { coords[a].push_back(Ptemp[i]); });
		};
		for (int i = 0; i < N; i++) {
			ins(i);
		}
		for (auto [x, y] : ops) {
			std::swap(Ptemp[x], Ptemp[y]);
			ins(x);
			ins(y);
		}
	}

	std::vector<binary_indexed_tree<int>> seg(2*N);
	for (seg_tree::point a(1); a < 2*N; a++) {
		std::sort(coords[a].begin(), coords[a].end());
		coords[a].resize(std::unique(coords[a].begin(), coords[a].end()) - coords[a].begin());
		seg[a] = binary_indexed_tree<int>(coords[a].size(), 0);
	}

	auto pref_sum = [&](seg_tree::point a, int v) {
		v = int(std::lower_bound(coords[a].begin(), coords[a].end(), v) - coords[a].begin());
		int r = 0;
		for (auto x : seg[a].prefix(v)) {
			r += x;
		}
		return r;
	};
	auto upd = [&](seg_tree::point a, int v, int delta) {
		v = int(std::lower_bound(coords[a].begin(), coords[a].end(), v) - coords[a].begin());
		for (auto& x : seg[a].suffix(v)) {
			x += delta;
		}
	};

	int64_t ans = 0;
	auto get_contrib = [&](int i, int p) -> int64_t {
		int64_t ans = 0;
		if (p < i) {
			auto rng_lo = layout.get_range(p, i);
			rng_lo.for_each([&](auto a) -> void {
				ans += pref_sum(a, p);
			});
			auto rng_hi = layout.get_range(i+1, N);
			rng_hi.for_each([&](auto a) -> void {
				ans += pref_sum(a, i+1) - pref_sum(a, p);
			});
		} else if (p > i) {
			auto rng_lo = layout.get_range(0, i);
			rng_lo.for_each([&](auto a) -> void {
				ans += pref_sum(a, p) - pref_sum(a, i+1);
			});
			auto rng_hi = layout.get_range(i+1, p);
			rng_hi.for_each([&](auto a) -> void {
				ans += pref_sum(a, N) - pref_sum(a, p);
			});
		}
		return ans;
	};
	auto insert_point = [&](int i, int p) {
		// now, query
		ans += get_contrib(i, p);
		layout.get_point(i).for_each([&](auto a) -> void {
			upd(a, p, +1);
		});
		if (p > i) {
			ans++;
		}
	};
	auto delete_point = [&](int i, int p) {
		if (p > i) {
			ans--;
		}
		layout.get_point(i).for_each([&](auto a) -> void {
			upd(a, p, -1);
		});
		ans -= get_contrib(i, p);
	};

	for (int i = 0; i < N; i++) {
		insert_point(i, P[i]);
	}

	for (auto [x, y] : ops) {
		delete_point(x, P[x]);
		delete_point(y, P[y]);
		std::swap(P[x], P[y]);
		insert_point(x, P[x]);
		insert_point(y, P[y]);

		cout << ans << '\n';
	}

	return 0;
}
