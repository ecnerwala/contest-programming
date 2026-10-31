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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	struct vert_t {
		int par;
		int v_color;
		int e_color;
		int64_t weight;
	};
	std::vector<vert_t> verts(N);
	for (int i = 0; i < N; i++) {
		cin >> verts[i].v_color;
	}
	verts[0].par = -1;
	for (int i = 1; i < N; i++) {
		cin >> verts[i].par;
		verts[i].par--;
	}
	verts[0].e_color = -1;
	for (int i = 1; i < N; i++) {
		cin >> verts[i].e_color;
	}
	verts[0].weight = 0;
	for (int i = 1; i < N; i++) {
		cin >> verts[i].weight;
	}

	std::vector<bool> is_good_ch(N);
	std::vector<std::vector<int>> good_ch(N);
	std::vector<int> good_sz(N, 1);
	for (int i = N-1; i >= 1; i--) {
		int p = verts[i].par;
		assert(0 <= p && p < i);
		is_good_ch[i] = (verts[i].e_color == verts[p].e_color);
		if (is_good_ch[i]) {
			good_ch[p].push_back(i);
			good_sz[p] += good_sz[i];
		}
	}
	std::vector<bool> is_heavy(N);
	for (int i = 0; i < N; i++) {
		if (good_ch[i].empty()) continue;
		int j = *std::max_element(good_ch[i].begin(), good_ch[i].end(), [&](auto a, auto b) { return good_sz[a] < good_sz[b]; });
		is_heavy[j] = true;
	}
	std::vector<int> heavy_root(N, -1);
	std::vector<int> heavy_sz(N);
	std::vector<int> heavy_ch(N, -1);
	std::vector<int> heavy_idx(N, -1);
	for (int i = 0; i < N; i++) {
		int p = verts[i].par;
		if (!is_heavy[i]) {
			heavy_root[i] = i;
			heavy_idx[i] = 0;
		} else {
			heavy_ch[p] = i;
			heavy_root[i] = heavy_root[p];
			heavy_idx[i] = heavy_idx[p] + 1;
		}
		int rt = heavy_root[i];
		heavy_sz[rt] += 1;
	}

	auto maybe_biggest = [&](const std::set<std::pair<int64_t, int>>& a) -> int64_t {
		if (a.empty()) return 0;
		auto it = a.rbegin();
		return it->first;
	};
	auto maybe_biggest_2 = [&](const std::set<std::pair<int64_t, int>>& a) -> int64_t {
		auto it = a.rbegin();
		int64_t ans = 0;
		for (int z = 0; z < 2 && it != a.rend(); z++, ++it) {
			ans += it->first;
		}
		return ans;
	};

	std::vector<std::map<int, std::map<int, std::set<std::pair<int64_t, int>>>>> ch_v_e(N);
	std::vector<std::map<int, std::set<std::pair<int64_t, int>>>> best_ch2(N);

	std::set<std::pair<int64_t, int>> heavy_bests;

	struct seg_t {
		int c_pref, c_suff;
		int64_t w_pref, w_suff, w_mid, w_through;
	};
	/*
	auto dump = [&](const seg_t& a) -> void {
		cerr << a.c_pref << '-' << a.w_pref << '-' << a.w_through << "(" << a.w_mid << ")" << '-' << a.w_suff << '-' << a.c_suff << '\n';
	};
	*/
	auto merge = [&](const seg_t& a, const seg_t& b) -> seg_t {
		//cerr << "merge" << '\n';
		//dump(a);
		//dump(b);
		seg_t r;
		r.c_pref = a.c_pref;
		r.w_pref = a.w_pref;
		if (a.w_through >= 0 && a.c_suff == b.c_pref) {
			setmax(r.w_pref, a.w_through + b.w_pref);
		}
		r.c_suff = b.c_suff;
		r.w_suff = b.w_suff;
		if (b.w_through >= 0 && b.c_pref == a.c_suff) {
			setmax(r.w_suff, b.w_through + a.w_suff);
		}
		r.w_mid = std::max(a.w_mid, b.w_mid);
		if (a.c_suff == b.c_pref) {
			setmax(r.w_mid, a.w_suff + b.w_pref);
		}
		if (a.w_through >= 0 && b.w_through >= 0 && a.c_suff == b.c_pref) {
			r.w_through = a.w_through + b.w_through;
		} else {
			r.w_through = -1;
		}
		//dump(r);
		return r;
	};
	std::vector<std::vector<seg_t>> segs(N);
	for (int i = 0; i < N; i++) {
		if (is_heavy[i]) continue;
		segs[i].resize(2*heavy_sz[i]);
	}

	// Now, let's go initialize everything
	for (int i = N-1; i >= 0; i--) {
		int rt = heavy_root[i];
		seg_tree::in_order_layout layout(heavy_sz[rt]);
		auto& seg = segs[rt];
		{
			seg_tree::point a = layout.get_point(heavy_idx[i]);
			seg[a].c_pref = seg[a].c_suff = verts[i].v_color;
			seg[a].w_through = verts[i].weight;
			seg[a].w_mid = maybe_biggest(best_ch2[i][verts[i].v_color]);
			seg[a].w_suff = maybe_biggest(ch_v_e[i][verts[i].v_color][verts[i].e_color]);
			seg[a].w_pref = seg[a].w_through + seg[a].w_suff;
		}
		//cerr << "set " << i << '\n';

		if (rt == i) {
			for (seg_tree::point a(layout.N - 1); a > 0; a--) {
				seg[a] = merge(seg[a.c(0)], seg[a.c(1)]);
			}

			//cerr << "root " << rt << ' ' << heavy_sz[rt] << '\n';
			//cerr << seg[1].w_mid << ' ' << seg[1].w_pref << '\n';
			heavy_bests.insert({seg[1].w_mid, i});

			if (verts[rt].par != -1) {
				auto& s2 = best_ch2[verts[rt].par][verts[rt].v_color];
				auto& s = ch_v_e[verts[rt].par][verts[rt].v_color][verts[rt].e_color];
				s2.erase({maybe_biggest_2(s), verts[rt].e_color});
				s.insert({seg[1].w_pref, rt});
				s2.insert({maybe_biggest_2(s), verts[rt].e_color});
			}
		}
	}

	auto get_ans = [&]() -> int64_t {
		auto it = heavy_bests.rbegin();
		assert(it != heavy_bests.rend());
		//cerr << "ans " << it->first << ' ' << it->second << '\n';
		return it->first;
	};

	auto do_update = [&](int v, int c) -> void {
		//cerr << '\n';
		//cerr << "do update " << v << ' ' << verts[v].v_color << "->" << c << '\n';
		// go update each guy one at a time
		for (int i = v; true; ) {
			int rt = heavy_root[i];
			seg_tree::in_order_layout layout(heavy_sz[rt]);
			auto& seg = segs[rt];
			{
				heavy_bests.erase({seg[1].w_mid, rt});

				if (verts[rt].par != -1) {
					auto& s2 = best_ch2[verts[rt].par][verts[rt].v_color];
					auto& s = ch_v_e[verts[rt].par][verts[rt].v_color][verts[rt].e_color];
					s2.erase({maybe_biggest_2(s), verts[rt].e_color});
					s.erase({seg[1].w_pref, rt});
					s2.insert({maybe_biggest_2(s), verts[rt].e_color});
				}
			}

			if (i == v) {
				verts[i].v_color = c;
			}

			{
				seg_tree::point a = layout.get_point(heavy_idx[i]);
				seg[a].c_pref = seg[a].c_suff = verts[i].v_color;
				seg[a].w_through = verts[i].weight;
				seg[a].w_mid = maybe_biggest(best_ch2[i][verts[i].v_color]);
				seg[a].w_suff = maybe_biggest(ch_v_e[i][verts[i].v_color][verts[i].e_color]);
				seg[a].w_pref = seg[a].w_through + seg[a].w_suff;
				a.for_parents_up([&](auto p) -> void {
					seg[p] = merge(seg[p.c(0)], seg[p.c(1)]);
				});
			}

			//cerr << "updating " << i << '\n';
			//cerr << "root " << rt << ' ' << heavy_sz[rt] << '\n';
			//cerr << seg[1].c_pref << ' ' << seg[1].c_suff << '\n';
			//cerr << seg[1].w_through << '\n';
			//cerr << seg[1].w_mid << ' ' << seg[1].w_pref << '\n';

			{
				heavy_bests.insert({seg[1].w_mid, rt});

				if (verts[rt].par != -1) {
					auto& s2 = best_ch2[verts[rt].par][verts[rt].v_color];
					auto& s = ch_v_e[verts[rt].par][verts[rt].v_color][verts[rt].e_color];
					s2.erase({maybe_biggest_2(s), verts[rt].e_color});
					s.insert({seg[1].w_pref, rt});
					s2.insert({maybe_biggest_2(s), verts[rt].e_color});
				}
			}

			if (verts[i].e_color != verts[v].e_color) break;
			i = verts[rt].par;
			if (i == -1) break;
		}
	};

	cout << get_ans() << '\n';
	for (int q = 0; q < Q; q++) {
		int x, c; cin >> x >> c; x--;
		do_update(x, c);
		cout << get_ans() << '\n';
	}

	return 0;
}
