#include<bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
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

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

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
		v = int(ll(v) * ll(o.v) % MOD);
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

using num = modnum<int(1e9)+7>;

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

	// Iterate over the range from outside-in.
	//   Calls f(point a, bool is_right)
	template <typename F> void for_each(F f) const {
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

const int INF = int(1e9);

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << '\n';

		int N, Q; cin >> N >> Q;

		std::vector<std::array<int, 3>> level(N);
		std::vector<std::pair<int, std::array<int, 2>>> pts; pts.reserve(3*N);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < 3; j++) {
				cin >> level[i][j];
				pts.push_back({level[i][j], {i,j}});
			}
		}

		std::vector<std::tuple<int, std::array<std::array<int, 2>, 2>, int>> queries(Q);
		for (int q = 0; q < Q; q++) {
			std::array<std::array<int, 2>, 2> p;
			for (int z = 0; z < 2; z++) {
				for (int x = 0; x < 2; x++) {
					cin >> p[z][x]; p[z][x]--;
				}
			}
			int l; cin >> l;
			if (p[0] > p[1]) std::swap(p[0], p[1]);
			queries[q] = {l, p, q};
		}

		using seg_node = std::array<std::array<int, 3>, 3>;
		auto neg_start = [&]() -> seg_node {
			return seg_node{std::array<int, 3>{-1, INF, INF}, {INF, -1, INF}, {INF, INF, -1}};
		};
		auto unvis = [&]() -> seg_node {
			return seg_node{std::array<int, 3>{INF, INF, INF}, {INF, INF, INF}, {INF, INF, INF}};
		};
		auto merge = [&](const seg_node& a, const seg_node& b) -> seg_node {
			seg_node r = unvis();
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					for (int k = 0; k < 3; k++) {
						if (a[i][k] == INF || b[k][j] == INF) continue;
						setmin(r[i][j], a[i][k] + b[k][j] + 1);
					}
				}
			}
			return r;
		};

		auto enable = [&](std::array<int, 3> r, int v) -> seg_node {
			return seg_node{
				std::array<int, 3>{v >= r[0] ? 0 : INF, v >= std::max(r[0], r[1]) ? 1 : INF, v >= std::max({r[0], r[1], r[2]}) ? 2 : INF},
				std::array<int, 3>{v >= std::max(r[0], r[1]) ? 1 : INF, v >= r[1] ? 0 : INF, v >= std::max(r[1], r[2]) ? 1 : INF},
				std::array<int, 3>{v >= std::max({r[0], r[1], r[2]}) ? 2 : INF, v >= std::max(r[1], r[2]) ? 1 : INF, v >= r[2] ? 0 : INF}
			};
		};

		seg_tree::in_order_layout layout(N);
		std::vector<seg_node> seg(2*N, unvis());

		struct link_seg_node {
			bool is_tunnel = false;
			int tunnel_len = 0; // number of vertices
			int r_dist = INF;
			int l_dist = INF;
		};
		auto merge_link = [&](link_seg_node a, link_seg_node b) -> link_seg_node {
			link_seg_node r;

			if (a.is_tunnel && b.is_tunnel) {
				r.is_tunnel = true;
				r.tunnel_len = a.tunnel_len + b.tunnel_len;
			}

			if (b.r_dist < INF) {
				r.r_dist = b.r_dist;
			} else if (b.is_tunnel && a.r_dist < INF) {
				r.r_dist = a.r_dist + b.tunnel_len * 2;
			}

			if (a.l_dist < INF) {
				r.l_dist = a.l_dist;
			} else if (a.is_tunnel && b.l_dist < INF) {
				r.l_dist = b.l_dist + a.tunnel_len * 2;
			}

			return r;
		};

		std::vector<link_seg_node> link_seg(2*N);

		auto enable_link = [&](std::array<int, 3> r, int v) -> link_seg_node {
			if (v >= std::max({r[0], r[1], r[2]})) {
				return link_seg_node{false, 0, 4, 4};
			} else if (v >= std::max(r[0], r[2])) {
				return link_seg_node{true, 1, INF, INF};
			} else {
				return link_seg_node{false, 0, INF, INF};
			}
		};

		auto update_node = [&](seg_tree::point a) -> void {
			seg[a] = merge(seg[2*a], seg[2*a+1]);
			link_seg[a] = merge_link(link_seg[2*a], link_seg[2*a+1]);
		};

		std::sort(pts.begin(), pts.end());
		std::sort(queries.begin(), queries.end());

		num ans = 1;
		auto it = pts.begin();
		for (auto [val, p, q] : queries) {
			for (; it != pts.end() && it->first <= val; ++it) {
				//cerr << "enable " << it->second[0] << ' ' << it->second[1] << '\n';
				int x = it->second[0];
				seg_tree::point a = layout.get_point(x);
				seg[a] = enable(level[x], it->first);
				link_seg[a] = enable_link(level[x], it->first);
				a.for_parents_up(update_node);
			}
			//cerr << p[0][0] << ',' << p[0][1] << " - " << p[1][0] << ',' << p[1][1] << '\n';

			seg_node l = neg_start();
			auto rng = layout.get_range(p[0][0], p[1][0]+1);
			rng.for_each_l_to_r([&](seg_tree::point a) -> void {
				l = merge(l, seg[a]);
			});
			/*
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					cerr << l[i][j] << ' ';
				}
				cerr << '\n';
			}
			*/

			link_seg_node left_link;
			layout.get_range(0, p[0][0]).for_each_l_to_r([&](seg_tree::point a) {
				left_link = merge_link(left_link, link_seg[a]);
			});

			link_seg_node right_link;
			layout.get_range(p[1][0]+1, N).for_each_r_to_l([&](seg_tree::point a) {
				right_link = merge_link(link_seg[a], right_link);
			});

			if (left_link.r_dist < INF && val >= std::max(level[p[0][0]][0], level[p[0][0]][2])) {
				for (int z = 0; z < 3; z++) {
					setmin(l[0][z], l[2][z] + left_link.r_dist);
					setmin(l[2][z], l[0][z] + left_link.r_dist);
				}
			}
			if (right_link.l_dist < INF && val >= std::max(level[p[1][0]][0], level[p[1][0]][2])) {
				for (int z = 0; z < 3; z++) {
					setmin(l[z][0], l[z][2] + right_link.l_dist);
					setmin(l[z][2], l[z][0] + right_link.l_dist);
				}
			}

			int dist = l[p[0][1]][p[1][1]];
			//cerr << dist << '\n';
			if (dist == INF) {
				ans *= num(1);
			} else {
				ans *= num(dist);
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
