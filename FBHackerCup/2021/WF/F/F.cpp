#include<bits/stdc++.h>

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

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		cerr << "Case #" << case_num << ": N = " << N << "\n";
		using range_t = std::array<int, 2>;
		using rect_t = std::array<range_t, 2>;
		std::array<std::vector<rect_t>, 2> R;
		R[0].resize(N);
		R[1].resize(N);
		for (int i = 0; i < N; i++) {
			int x, y, w, h; cin >> x >> y >> w >> h;
			char d; cin >> d;
			R[0][i] = {range_t{x, x+w}, range_t{y, y+h}};
			if (d == 'D') {
				y -= h;
			} else if (d == 'U') {
				y += h;
			} else if (d == 'L') {
				x -= w;
			} else if (d == 'R') {
				x += w;
			} else assert(false);
			R[1][i] = {range_t{x, x+w}, range_t{y, y+h}};
		}
		const int X = 2*N;
		for (int z = 0; z < 2; z++) {
			std::vector<int> coords; coords.reserve(2*N);
			for (const auto& a : R) {
				for (const auto& r : a) {
					coords.push_back(r[z][0]);
				}
			}
			assert(int(coords.size()) == X);
			std::sort(coords.begin(), coords.end());
			for (auto& a : R) {
				for (auto& r : a) {
					for (auto& v : r[z]) {
						v = int(std::lower_bound(coords.begin(), coords.end(), v) - coords.begin());
					}
					assert(0 <= r[z][0] && r[z][0] < r[z][1] && r[z][1] <= X);
				}
			}
		}

		// First, check self-intersections

		bool is_good = true;

		int V = N;
		std::vector<std::vector<int>> adj(V);
		std::vector<int> indeg(V);
		adj.reserve(1e7);

		// z is the point side
		for (int zp = 0; zp < 2; zp++) {
			for (int zr = 0; zr < 2; zr++) {
				seg_tree::circular_layout xlayout(X);

				auto add_edge = [&](int from, int to) -> void {
					if (zp == zr) {
						assert(from != -1);
						if (from != to) {
							cerr << "Flip conflict " << zp << ' ' << from << ' ' << to << '\n';
							assert(zp == 1 && zr == 1);
							is_good = false;
						}
					} else {
						if (zp) std::swap(from, to);
						adj[from].push_back(to);
						indeg[to]++;
					}
				};

				auto make_merged = [&](int a, int b) -> int {
					if (zp == zr) {
						assert(a == -1 || b == -1 || a != b);
						return -1;
					} else {
						assert(a != b);
						int idx = V++;
						adj.emplace_back();
						indeg.push_back(0);
						add_edge(idx, a);
						add_edge(idx, b);
						return idx;
					}
				};

				for (int outer_range = 0; outer_range < 2; outer_range++) {
					std::vector<std::vector<std::pair<int, int>>> seg_set(2*X);
					std::vector<std::pair<int, int>> sorted(N);
					for (int i = 0; i < N; i++) {
						auto r = R[zp][i];
						sorted[i] = {r[1][0], i};
					}
					std::sort(sorted.begin(), sorted.end());

					for (auto it : sorted) {
						int i = it.second;
						auto r = R[zp][i];
						if (outer_range) {
							xlayout.get_range(r[0]).for_each([&](seg_tree::point a, bool) {
								seg_set[a].push_back({r[1][0], i});
							});
						} else {
							xlayout.get_point(r[0][0]).for_each([&](seg_tree::point a) {
								seg_set[a].push_back({r[1][0], i});
							});
						}
					}
					std::vector<std::vector<int>> segs(2*X);
					for (seg_tree::point a(2*X-1); a > 0; a--) {
						int Y = int(seg_set[a].size());
						if (Y == 0) continue;

						seg_tree::circular_layout ylayout(Y);
						segs[a].resize(2*Y);

						assert(is_sorted(seg_set[a].begin(), seg_set[a].end()));
						//std::sort(seg_set[a].begin(), seg_set[a].end());

						for (int i = 0; i < Y; i++) {
							segs[a][ylayout.get_point(i)] = seg_set[a][i].second;
						}
						for (seg_tree::point b(Y-1); b > 0; b--) {
							segs[a][b] = make_merged(segs[a][b.c(0)], segs[a][b.c(1)]);
						}
					}

					for (int i = 0; i < N; i++) {
						auto r = R[zr][i];
						auto process_seg = [&](seg_tree::point a) -> void {
							int Y = int(seg_set[a].size());
							if (Y == 0) return;
							seg_tree::circular_layout ylayout(Y);

							int lo = int(std::lower_bound(seg_set[a].begin(), seg_set[a].end(), std::pair<int, int>{r[1][0], -1}) - seg_set[a].begin());
							int hi = int(std::lower_bound(seg_set[a].begin(), seg_set[a].end(), std::pair<int, int>{r[1][1], -1}) - seg_set[a].begin());
							auto rng = ylayout.get_range(lo, hi);
							rng.for_each([&](seg_tree::point b, bool) -> void {
								add_edge(i, segs[a][b]);
							});
						};
						if (outer_range) {
							xlayout.get_point(r[0][0]).for_each(process_seg);
							if (!is_good) goto done;
						} else {
							xlayout.get_range(r[0]).for_each_l_to_r(process_seg);
							if (!is_good) goto done;
						}
					}
				}
			}
		}

		if (is_good) {
			std::vector<int> q; q.reserve(V);
			for (int i = 0; i < V; i++) {
				if (indeg[i] == 0) q.push_back(i);
			}
			for (int z = 0; z < int(q.size()); z++) {
				int cur = q[z];
				if (cur < N) {
					//cerr << "take " << cur << '\n';
				}
				for (int nxt : adj[cur]) {
					--indeg[nxt];
					if (indeg[nxt] == 0) q.push_back(nxt);
				}
			}

			if (int(q.size()) < V) is_good = false;
		}

done:
		cout << "Case #" << case_num << ": " << (is_good ? "YES" : "NO") << '\n' << flush;
	}

	return 0;
}
