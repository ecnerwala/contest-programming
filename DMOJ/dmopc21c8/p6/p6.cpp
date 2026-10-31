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

	int N, Q; cin >> N >> Q;
	seg_tree::in_order_layout layout(N+2);
	const int INF = int(1e9);
	// we have a bunch of linear constraints of the form a[i] + b[j] >= 0 for all i < j
	struct seg_node {
		bool good = true;
		std::array<int, 4> a{INF, INF, INF, INF};
		std::array<int, 4> b{INF, INF, INF, INF};
	};
	std::vector<seg_node> seg(2*layout.N);
	auto update_node = [&](seg_tree::point a) {
		assert(a < layout.N);
		seg[a].good = seg[a[0]].good && seg[a[1]].good;
		for (int z = 0; z < 4; z++) {
			if (seg[a[0]].a[z] + seg[a[1]].b[z] < 0) {
				seg[a].good = false;
			}
			seg[a].a[z] = std::min(seg[a[0]].a[z], seg[a[1]].a[z]);
			seg[a].b[z] = std::min(seg[a[0]].b[z], seg[a[1]].b[z]);
		}
	};

	int cnt_valley = 0;
	std::vector<int> is_valley(N+2);
	auto erase_pair = [&](std::pair<int, int> a, std::pair<int, int> b) {
		assert(a.first < b.first);
		cnt_valley -= ((is_valley[std::min(a.second, b.second)+2]--) == 2);
		if (a.first == -1 && b.first == N) return;

		auto pa = layout.get_point(a.second+2);
		auto pb = layout.get_point(b.second+2);
		if (a.second < b.second) {
			seg[pa].a[0] = INF;
			seg[pa].a[1] = INF;

			seg[pb].b[2] = INF;
			seg[pb].b[3] = INF;
		} else if (a.second > b.second) {
			seg[pa].b[0] = INF;
			seg[pa].b[1] = INF;

			seg[pb].a[2] = INF;
			seg[pb].a[3] = INF;
		} else assert(false);
		pa.for_parents_up(update_node);
		pb.for_parents_up(update_node);
	};
	auto insert_pair = [&](std::pair<int, int> a, std::pair<int, int> b) {
		assert(a.first < b.first);
		cnt_valley += ((++is_valley[std::min(a.second, b.second)+2]) == 2);
		if (a.first == -1 && b.first == N) return;

		auto pa = layout.get_point(a.second+2);
		auto pb = layout.get_point(b.second+2);
		if (a.second < b.second) {
			assert(seg[pa].a[0] == INF);
			seg[pa].a[0] = a.first - a.second;
			assert(seg[pa].a[1] == INF);
			seg[pa].a[1] = -(a.first+1);

			assert(seg[pb].b[2] == INF);
			seg[pb].b[2] = b.first;
			assert(seg[pb].b[3] == INF);
			seg[pb].b[3] = b.second - b.first;
		} else if (a.second > b.second) {
			assert(seg[pa].b[0] == INF);
			seg[pa].b[0] = N - 1 - a.first;
			assert(seg[pa].b[1] == INF);
			seg[pa].b[1] = a.second - (N - 1 - a.first);

			assert(seg[pb].a[2] == INF);
			seg[pb].a[2] = (N - 1 - b.first) - b.second;
			assert(seg[pb].a[3] == INF);
			seg[pb].a[3] = -(N - b.first);
		} else assert(false);
		pa.for_parents_up(update_node);
		pb.for_parents_up(update_node);
	};

	std::vector<int> A(N, -1);
	std::map<int, int> alive;
	std::map<int, int> vals;

	alive[-1] = -1;
	vals[-1] = -1;

	alive[N] = -1;
	//vals[-2] = N;

	insert_pair({-1, -1}, {N, -1});

	auto do_set = [&](int i, int v) -> void {
		assert(A[i] == -1);
		assert(0 <= v && v < N);
		A[i] = v;
		assert(!vals.count(v));
		vals[v] = i;

		auto it = alive.insert({i, v}).first;
		assert(it->first == i && it->second == v);
		auto pt = std::prev(it);
		auto nt = std::next(it);
		erase_pair(*pt, *nt);
		insert_pair(*pt, *it);
		insert_pair(*it, *nt);
	};
	auto do_unset = [&](int i) -> void {
		assert(A[i] != -1);
		int v = A[i];
		A[i] = -1;

		assert(vals.count(v) && vals[v] == i);
		vals.erase(v);

		auto it = alive.find(i);
		assert(it->first == i && it->second == v);
		auto pt = std::prev(it);
		auto nt = std::next(it);
		erase_pair(*pt, *it);
		erase_pair(*it, *nt);
		insert_pair(*pt, *nt);
		alive.erase(it);
	};

	auto real_query = [&]() -> bool {
		return cnt_valley <= 1 && seg[1].good;
	};

	auto query = [&]() -> bool {
		auto it = std::prev(vals.end());
		if (it->first != N-1) {
			assert(it->first < N-1);
			int idx = it->second;
			for (int z = idx-1; z <= idx+1; z += 2) {
				if (0 <= z && z < N && A[z] == -1) {
					do_set(z, N-1);
					bool ans = real_query();
					do_unset(z);
					if (ans) return true;
				}
			}
			return false;
		} else {
			return real_query();
		}
	};

	std::vector<std::pair<int, int>> initial_sets; initial_sets.reserve(N);
	for (int i = 0; i < N; i++) {
		int a; cin >> a; a--;
		if (a != -1) {
			initial_sets.push_back({i, a});
		}
	}
	//std::mt19937 mt(48);
	//std::shuffle(initial_sets.begin(), initial_sets.end(), mt);
	for (auto [i, a] : initial_sets) {
		do_set(i, a);
	}
	cout << (query() ? "YES" : "NO") << '\n';
	for (int q = 0; q < Q; q++) {
		int i, v; cin >> i >> v; i--, v--;
		if (A[i] != -1) do_unset(i);
		if (v != -1) do_set(i, v);
		cout << (query() ? "YES" : "NO") << '\n';
	}

	return 0;
}
