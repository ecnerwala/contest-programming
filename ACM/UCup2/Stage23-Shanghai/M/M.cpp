#include <bits/stdc++.h>

namespace seg_tree {

// Floor of log_2(a); index of highest 1-bit
inline int floor_log_2(int a) {
	return a ? (8 * sizeof(a)) - 1 - __builtin_clz(a) : -1;
}

inline int ceil_log_2(int a) {
	return a ? floor_log_2(2*a-1) : -1;
}

inline int next_pow_2(int a) {
	return 1 << ceil_log_2(a);
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
		for (int L = floor_log_2(a); L > 0; L--) {
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
		int anc_depth = floor_log_2((a-1) ^ b);
		int anc_msk = (1 << anc_depth) - 1;
		for (int v = (-a) & anc_msk; v; v &= v-1) {
			int i = __builtin_ctz(v);
			f(point(((a-1) >> i) + 1));
		}
		for (int v = b & anc_msk; v; ) {
			int i = floor_log_2(v);
			f(point((b >> i) - 1));
			v ^= (1 << i);
		}
	}

	// Iterate over the range from right to left.
	//    Calls f(point)
	template <typename F> void for_each_r_to_l(F f) const {
		int anc_depth = floor_log_2((a-1) ^ b);
		int anc_msk = (1 << anc_depth) - 1;
		for (int v = b & anc_msk; v; v &= v-1) {
			int i = __builtin_ctz(v);
			f(point((b >> i) - 1));
		}
		for (int v = (-a) & anc_msk; v; ) {
			int i = floor_log_2(v);
			f(point(((a-1) >> i) + 1));
			v ^= (1 << i);
		}
	}

	template <typename F> void for_parents_down(F f) const {
		int x = a, y = b;
		if ((x ^ y) > x) { x <<= 1, std::swap(x, y); }
		int dx = __builtin_ctz(x);
		int dy = __builtin_ctz(y);
		int anc_depth = floor_log_2((x-1) ^ y);
		for (int i = floor_log_2(x); i > dx; i--) {
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
		int anc_depth = floor_log_2((x-1) ^ y);
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

using std::cerr;
std::vector<int64_t> solve(int M, int64_t L, std::vector<std::array<int, 2>> routes, std::vector<int64_t> queries) {
	std::vector<int64_t> coords; coords.reserve(2*M);
	for (auto [s, t] : routes) {
		coords.push_back(s);
		coords.push_back(t);
	}
	std::sort(coords.begin(), coords.end());
	coords.resize(int(std::unique(coords.begin(), coords.end()) - coords.begin()));
	int N = int(coords.size());
	for (auto& [s, t] : routes) {
		s = int(std::lower_bound(coords.begin(), coords.end(), s) - coords.begin());
		t = int(std::lower_bound(coords.begin(), coords.end(), t) - coords.begin());
	}

	std::vector<int> num_edges(N);
	for (auto [s, t] : routes) {
		assert(s != t);
		num_edges[(t+1)%N]++;
		num_edges[(s+1)%N]--;
	}
	for (int i = 0; i+1 < N; i++) num_edges[i+1] += num_edges[i];
	{
		int v = *std::min_element(num_edges.begin(), num_edges.end());
		for (auto& a : num_edges) a -= v;
	}

	{ // First, find the baseline
		std::vector<int> par(N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a);
			b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};
		int num_comps = N;
		for (auto [s, t] : routes) {
			num_comps -= merge(s, t);
		}
		for (int i = 0; i < N; i++) {
			if (num_edges[i] > 0) {
				num_comps -= merge(i, (i+N-1) % N);
			}
		}
		if (num_comps == 1) {
			// create some -1's, but that's fine
			for (auto& a : num_edges) a--;
		}
	}

	int64_t ans = 0;
	for (auto [s, t] : routes) {
		ans += (coords[t] - coords[s]) + (s > t ? L : 0);
	}
	for (int i = 0; i < N; i++) {
		ans += num_edges[i] * ((i == 0) ? (L + coords[0] - coords[N-1]) : (coords[i] - coords[i-1]));
	}
	assert(ans % L == 0);

	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};
	int num_comps = N;
	for (auto [s, t] : routes) {
		num_comps -= merge(s, t);
	}
	for (int i = 0; i < N; i++) {
		if (num_edges[i] > 0) {
			num_comps -= merge(i, (i+N-1) % N);
		}
	}

	seg_tree::in_order_layout layout(N);
	std::vector<int64_t> seg(2*N, -L);
	auto set_end = [&](int lo, int hi, int64_t val) {
		layout.get_range(lo, hi).for_each([&](seg_tree::point a) {
			seg[a] = std::max(seg[a], val);
		});
	};
	for (int i = 0; i < N; i++) {
		assert(num_edges[i] >= -1);
		if (num_edges[i] == -1) {
			// < i requires going up to coords[i]
			// i or bigger requires going up to coords[i] + L
			set_end(0, i, coords[i]);
			set_end(i, N, coords[i] + L);
		}
	}
	// Now, we need to find the components
	std::vector<std::vector<int>> comps(N);
	for (int i = 0; i < N; i++) {
		comps[get_par(i)].push_back(i);
	}
	for (const auto& comp : comps) {
		if (comp.empty()) continue;
		// Otherwise, you must reach a point of the component
		for (int z = 0; z < int(comp.size()); z++) {
			int i = comp[z];
			if (num_edges[i] > 0) continue;
			// strictly after prv and i or earlier must reach i
			if (z == 0) {
				int prv = comp.back();
				set_end(prv+1, N, coords[i] + L);
				set_end(0, i+1, coords[i]);
			} else {
				int prv = comp[z-1];
				set_end(prv+1, i+1, coords[i]);
			}
		}
	}
	for (seg_tree::point a(1); a < layout.N; a++) {
		for (int z = 0; z < 2; z++) {
			seg[a.c(z)] = std::max(seg[a.c(z)], seg[a]);
		}
	}

	std::vector<int64_t> answers; answers.reserve(queries.size());
	for (int64_t x : queries) {
		auto it = std::lower_bound(coords.begin(), coords.end(), x);
		if (it == coords.end()) {
			x -= L;
			it = coords.begin();
		}
		int i = int(it - coords.begin());
		// find the first thing we have to increment
		int64_t y = seg[layout.get_point(i)];
		assert(y >= x);
		assert(y >= coords[i]);
		if (num_edges[i] == -1) assert(y == coords[i] + L);
		else assert(y < coords[i] + L);

		int64_t real_ans = ans + (y - x);
		answers.push_back(real_ans);
	}
	return answers;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	constexpr bool TESTING = true;
	std::mt19937_64 mt(48);

	int T;
	if (TESTING) T = 10000;
	else cin >> T;
	while (T--) {
		int M; int64_t L;
		if (TESTING) {
			cerr << "Start test" << '\n';
			M = std::uniform_int_distribution<int>(1, 200)(mt);
			L = std::uniform_int_distribution<int>(2, 200)(mt);
			//cerr << "M = " << M << ", L = " << L << '\n';
		} else {
			cin >> M >> L;
		}
		std::vector<std::array<int, 2>> routes(M);
		for (auto& [s, t] : routes) {
			if (TESTING) {
				s = std::uniform_int_distribution<int>(0, L-1)(mt);
				t = std::uniform_int_distribution<int>(0, L-1)(mt);
				if (t == s) t = (s+1)%L;
				assert(s != t);
				//cerr << s << ' ' << t << '\n';
			} else {
				cin >> s >> t;
			}
		}

		int Q;
		if (TESTING) {
			Q = 100;
			//cerr << "Q = " << Q << '\n';
		} else {
			cin >> Q;
		}
		std::vector<int64_t> queries(Q);
		for (auto& x : queries) {
			if (TESTING) {
				x = std::uniform_int_distribution<int64_t>(0, L-1)(mt);
				//cerr << x << '\n';
			} else {
				cin >> x;
			}
		}

		auto answers = solve(M, L, routes, queries);
		if (TESTING) {
			auto shift = std::uniform_int_distribution<int>(1, L-1)(mt);
			auto routes_rotated = routes;
			for (auto& [s, t] : routes_rotated) {
				s = (s + shift) % L;
				t = (t + shift) % L;
			}
			auto queries_rotated = queries;
			for (auto& x : queries_rotated) {
				x = (x + shift) % L;
			}
			auto answers_rotated = solve(M, L, routes_rotated, queries_rotated);
			assert(answers == answers_rotated);
		}

		if (!TESTING) {
			for (auto& v : answers) cout << v << '\n';
		}
	}

	return 0;
}
