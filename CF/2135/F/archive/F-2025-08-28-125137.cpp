#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = mod_inv_in_range(v, MOD);
		return res;
	}
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

template <typename T> T power(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

template <typename U, typename V> struct pairnum {
	U u;
	V v;

	pairnum() : u(0), v(0) {}
	pairnum(long long val) : u(val), v(val) {}
	pairnum(const U& u_, const V& v_) : u(u_), v(v_) {}

	friend std::ostream& operator << (std::ostream& out, const pairnum& n) { return out << '(' << n.u << ',' << ' ' << n.v << ')'; }
	friend std::istream& operator >> (std::istream& in, pairnum& n) { long long val; in >> val; n = pairnum(val); return in; }

	friend bool operator == (const pairnum& a, const pairnum& b) { return a.u == b.u && a.v == b.v; }
	friend bool operator != (const pairnum& a, const pairnum& b) { return a.u != b.u || a.v != b.v; }

	pairnum inv() const {
		return pairnum(u.inv(), v.inv());
	}
	pairnum neg() const {
		return pairnum(u.neg(), v.neg());
	}
	pairnum operator- () const {
		return pairnum(-u, -v);
	}
	pairnum operator+ () const {
		return pairnum(+u, +v);
	}

	pairnum& operator ++ () {
		++u, ++v;
		return *this;
	}
	pairnum& operator -- () {
		--u, --v;
		return *this;
	}

	pairnum& operator += (const pairnum& o) {
		u += o.u;
		v += o.v;
		return *this;
	}
	pairnum& operator -= (const pairnum& o) {
		u -= o.u;
		v -= o.v;
		return *this;
	}
	pairnum& operator *= (const pairnum& o) {
		u *= o.u;
		v *= o.v;
		return *this;
	}
	pairnum& operator /= (const pairnum& o) {
		u /= o.u;
		v /= o.v;
		return *this;
	}

	friend pairnum operator ++ (pairnum& a, int) { pairnum r = a; ++a; return r; }
	friend pairnum operator -- (pairnum& a, int) { pairnum r = a; --a; return r; }
	friend pairnum operator + (const pairnum& a, const pairnum& b) { return pairnum(a) += b; }
	friend pairnum operator - (const pairnum& a, const pairnum& b) { return pairnum(a) -= b; }
	friend pairnum operator * (const pairnum& a, const pairnum& b) { return pairnum(a) *= b; }
	friend pairnum operator / (const pairnum& a, const pairnum& b) { return pairnum(a) /= b; }
};

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

	template <typename F> void for_each_down(F f) const {
		// strictly greater than 0
		for (int L = floor_log_2(a); L >= 0; L--) {
			f(point(a >> L));
		}
	}

	template <typename F> void for_each_up(F f) const {
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

std::mt19937 mt_gen(48);
std::vector<std::array<int, 2>> gen() {
	int N = 2 * std::uniform_int_distribution<int>(1, 10)(mt_gen) - 1;
	std::vector<std::array<int, 2>> ch(N, {-1, -1});
	int idx = 1;
	std::vector<int> leaves; leaves.reserve(N);
	leaves.push_back(0);
	while (idx < N) {
		int z = std::uniform_int_distribution<int>(0, int(leaves.size()) - 1)(mt_gen);
		assert(ch[leaves[z]][0] == -1);
		assert(ch[leaves[z]][1] == -1);
		ch[leaves[z]] = {idx, idx+1};
		leaves[z] = idx;
		leaves.push_back(idx+1);
		idx += 2;
	}
	assert(idx == N);
	return ch;
}

std::mt19937 mt(48);

std::vector<int> F_slow(std::vector<std::array<int, 2>> ch) {
	// each guy is just a set of other guys
	int N = int(ch.size());
	std::vector<int> node_val(N, -1);
	std::vector<std::vector<int>> chain(N);
	std::vector<std::vector<int>> cmp_res(N, std::vector<int>(N, -2));
	auto cmp_3 = [&](this auto&& self, int v, int u) -> int {
		if (cmp_res[v][u] != -2) return cmp_res[v][u];
		for (int z = 0; true; z++) {
			if (z == int(chain[v].size()) && z == int(chain[u].size())) return 0;
			// u smaller
			else if (z == int(chain[u].size())) return -1;
			// v smaller
			else if (z == int(chain[v].size())) return +1;
			else {
				int d = self(chain[u][z], chain[v][z]);
				if (d != 0) return d;
				continue;
			}
		}
		assert(false);
	};
	[&](this auto&& self, int cur) -> void {
		if (ch[cur][0] != -1) {
			self(ch[cur][0]);
			self(ch[cur][1]);
		}
		std::vector<int> val;
		for (int a = cur; true; ) {
			if (ch[a][0] == -1) break;
			val.push_back(ch[a][1]);
			a = ch[a][0];
		}
		std::sort(val.begin(), val.end(), [&](int u, int v) -> bool {
			return cmp_3(u, v) < 0;
		});
		std::reverse(val.begin(), val.end());
		chain[cur] = val;
	}(0);
	std::vector<int> res(N); std::iota(res.begin(), res.end(), 0);
	std::sort(res.begin(), res.end(), [&](int u, int v) -> bool {
		int d = cmp_3(u, v);
		if (d == 0) return u < v;
		else return d < 0;
	});
	return res;
}

std::vector<int> F(std::vector<std::array<int, 2>> ch) {
	int N = int(ch.size());

	std::vector<std::vector<int>> by_gen(N);
	int G = 1 + [&](this auto&& self, int cur) -> int {
		int gen;
		if (ch[cur][0] == -1) {
			gen = 0;
		} else {
			int gl = self(ch[cur][0]);
			int gr = self(ch[cur][1]);
			gen = std::max(gl, gr+1);
		}
		by_gen[gen].push_back(cur);
		return gen;
	}(0);
	while (!by_gen.empty() && by_gen.back().empty()) by_gen.pop_back();
	assert(int(by_gen.size()) == G);

	using mnum = modnum<998244353>;
	using hsh = pairnum<mnum, mnum>;
	auto init_weights = [&](seg_tree::in_order_layout l, std::vector<hsh>& vec) {
		vec.resize(l.N);
		for (auto& v : vec) {
			v.u = std::uniform_int_distribution<int>(0, mnum::MOD-1)(mt);
			v.v = std::uniform_int_distribution<int>(0, mnum::MOD-1)(mt);
		}
	};
	seg_tree::in_order_layout G_layout(G);
	std::vector<hsh> G_weights;
	init_weights(G_layout, G_weights);
	std::vector<seg_tree::in_order_layout> sub_layouts(G);
	std::vector<std::vector<hsh>> sub_weights(G);

	struct seg_node {
		std::array<int, 2> c{-1, -1};
		hsh v;
	};
	std::vector<seg_node> nodes; nodes.reserve(40 * N + 10);
	// empty node
	const int EMPTY = int(nodes.size());
	nodes.push_back({{0, 0}, 0});

	auto insert_bot = [&](this auto&& self, int tree, int g, int pos, seg_tree::point p) -> int {
		assert(0 <= g && g < G);
		assert(sub_layouts[g].N != 0);
		int res = int(nodes.size());
		nodes.push_back(nodes[tree]);
		if (p >= sub_layouts[g].N) {
			nodes[res].v += hsh(1);
		} else {
			int d = pos >= sub_layouts[g].get_node_split(p) ? 1 : 0;
			int ch_val = self(nodes[res].c[d], g, pos, p.c(d));
			nodes[res].c[d] = ch_val;
			nodes[res].v = nodes[nodes[res].c[0]].v + nodes[nodes[res].c[1]].v * sub_weights[g][p];
		}
		return res;
	};

	auto insert_top = [&](this auto&& self, int tree, int g, int pos, seg_tree::point p) -> int {
		if (p >= G_layout.N) {
			return insert_bot(tree, g, pos, seg_tree::point(1));
		} else {
			int res = int(nodes.size());
			nodes.push_back(nodes[tree]);
			int d = pos >= G_layout.get_node_split(p) ? 1 : 0;
			int ch_val = self(nodes[res].c[d], g, pos, p.c(d));
			nodes[res].c[d] = ch_val;
			nodes[res].v = nodes[nodes[res].c[0]].v + nodes[nodes[res].c[1]].v * G_weights[p];
			return res;
		}
	};

	auto cmp_bot = [&](this auto&& self, int a, int b, int g, seg_tree::point p) -> bool {
		if (a == EMPTY && b == EMPTY) return false;
		else if (a == EMPTY) return true;
		else if (b == EMPTY) return false;

		if (nodes[a].v == nodes[b].v) return false;
		assert(nodes[a].v != nodes[b].v);
		if (p >= sub_layouts[g].N) {
			// these are just the value directly
			return int(nodes[a].v.v) < int(nodes[b].v.v);
		} else {
			int d = nodes[nodes[a].c[1]].v != nodes[nodes[b].c[1]].v ? 1 : 0;
			return self(nodes[a].c[d], nodes[b].c[d], g, p.c(d));
		}
	};

	auto cmp_top = [&](this auto&& self, int a, int b, seg_tree::point p) -> bool {
		if (a == EMPTY && b == EMPTY) return false;
		else if (a == EMPTY) return true;
		else if (b == EMPTY) return false;

		if (nodes[a].v == nodes[b].v) return false;
		if (p >= G_layout.N) {
			int g = G_layout.get_leaf_index(p);
			assert(0 <= g && g < G);
			return cmp_bot(a, b, g, seg_tree::point(1));
		} else {
			int d = nodes[nodes[a].c[1]].v != nodes[nodes[b].c[1]].v ? 1 : 0;
			return self(nodes[a].c[d], nodes[b].c[d], p.c(d));
		}
	};

	std::vector<int> cur_tree(N, -1);
	std::vector<std::pair<int, int>> val(N, {-1, -1});
	std::vector<int> ans; ans.reserve(N);
	for (int g = 0; g < G; g++) {
		std::vector<std::pair<int, int>> gen_vals; gen_vals.reserve(by_gen[g].size());
		for (int cur : by_gen[g]) {
			assert(cur_tree[cur] == -1);
			if (g == 0) {
				cur_tree[cur] = EMPTY;
			} else {
				int t = cur_tree[ch[cur][0]];
				auto [gen, pos] = val[ch[cur][1]];
				assert(gen != -1 && pos != -1);
				assert(gen < g);
				cur_tree[cur] = insert_top(t, gen, pos, seg_tree::point(1));
			}
			gen_vals.push_back({cur_tree[cur], cur});
		}
		std::sort(gen_vals.begin(), gen_vals.end(), [&](auto a, auto b) -> bool {
			if (nodes[a.first].v == nodes[b.first].v) {
				return a.second < b.second;
			}
			return cmp_top(a.first, b.first, seg_tree::point(1));
		});
		int cnt = 0;
		for (int i = 0; i < int(gen_vals.size()); i++) {
			ans.push_back(gen_vals[i].second);
			if (i > 0 && nodes[gen_vals[i-1].first].v == nodes[gen_vals[i].first].v) {
			} else {
				cnt++;
			}
			val[gen_vals[i].second] = {g, cnt-1};
		}
		sub_layouts[g] = seg_tree::in_order_layout(cnt);
		init_weights(sub_layouts[g], sub_weights[g]);
	}

	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int z = 0; z < 10000; z++) {
		auto ch = gen();
		cerr << "test " << z << '\n';
		cerr << ch.size() << '\n';
		for (auto [l, r] : ch) cerr << l << ' ' << r << '\n';
		auto ans_slow = F_slow(ch);
		auto ans = F(ch);
		for (auto v : ans_slow) { cerr << v << ' '; } cerr << '\n';
		for (auto v : ans) { cerr << v << ' '; } cerr << '\n';
		assert(ans == ans_slow);
	}

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<int, 2>> ch(N);
		for (auto& [l, r] : ch) {
			cin >> l >> r; l--, r--;
			assert((l == -1) == (r == -1));
		}
		auto ans = F(ch);
		for (int i = 0; i < N; i++) {
			cout << ans[i]+1 << " \n"[i+1==N];
		}
	}

	return 0;
}
