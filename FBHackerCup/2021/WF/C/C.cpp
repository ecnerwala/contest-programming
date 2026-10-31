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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		cerr << "Case #" << case_num << ": " << "N = " << N << '\n';

		struct agent_t {
			int r, c;
			int st, en;
			int idx;
		};
		std::array<std::array<std::vector<agent_t>, 2>, 2> agents;
		for (auto& r : agents) for (auto& v : r) v.reserve(N);

		for (int i = 0; i < N; i++) {
			int r, c, p; char d; cin >> r >> c >> p >> d;
			if (d == 'E') {
				agents[0][0].push_back({r, c, c, c+p, i});
			} else if (d == 'W') {
				agents[0][1].push_back({-r, -c, -c, -c+p, i});
			} else if (d == 'S') {
				agents[1][0].push_back({c, r, r, r+p, i});
			} else if (d == 'N') {
				agents[1][1].push_back({-c, -r, -r, -r+p, i});
			} else assert(false);
		}

		cerr << agents[0][0].size() << ' ' << agents[0][1].size() << ' ' << agents[1][0].size() << ' ' << agents[1][1].size() << '\n';

		for (auto& d : agents) {
			for (auto& v : d) {
				std::sort(v.begin(), v.end(), [&](const agent_t& a, const agent_t& b) {
					// leftmost has precedence, then latest index
					return std::make_tuple(a.r, a.c, -a.idx) < std::make_tuple(b.r, b.c, -b.idx);
				});
				int L = 0;
				for (int i = 0; i < int(v.size()); i++) {
					assert(L <= i);
					if (L > 0 && v[L-1].r == v[i].r) {
						if (v[L-1].en >= v[i].en) {
							continue;
						}
						v[i].st = std::max(v[i].st, v[L-1].en);
					}
					v[L++] = v[i];
				}
				v.resize(L);
			}

			std::reverse(d[1].begin(), d[1].end());
			for (auto& a : d[1]) {
				a.r = -a.r;
				a.c = -a.c;
				a.st = 1 - a.st;
				a.en = 1 - a.en;
				std::swap(a.st, a.en);
			}

			{
				std::array<std::vector<agent_t>, 2> nd;
				nd[0].reserve(2 * (d[0].size() + d[1].size()));
				nd[1].reserve(2 * (d[0].size() + d[1].size()));

				auto it0 = d[0].begin();
				auto it1 = d[1].begin();
				while (it0 != d[0].end() || it1 != d[1].end()) {
					if (it0 != d[0].end() && (it1 == d[1].end() || std::make_pair(it0->r, it0->en) <= std::make_pair(it1->r, it1->st))) {
						nd[0].push_back(*it0);
						++it0;
						continue;
					}
					if (it1 != d[1].end() && (it0 == d[0].end() || std::make_pair(it1->r, it1->en) <= std::make_pair(it0->r, it0->st))) {
						nd[1].push_back(*it1);
						++it1;
						continue;
					}
					assert(it0 != d[0].end() && it1 != d[1].end());

					assert(it0->r == it1->r);
					int l = std::max(it0->st, it1->st);
					int r = std::min(it0->en, it1->en);
					assert(l < r);
					if (it0->st < l) {
						nd[0].push_back(*it0);
						nd[0].back().en = l;
						it0->st = l;
					} else if (it1->st < l) {
						nd[1].push_back(*it1);
						nd[1].back().en = l;
						it1->st = l;
					}
					assert(it0->st == it1->st);
					assert(it0->st < it0->en);
					assert(it1->st < it1->en);

					int m;
					if (it0->idx < it1->idx) {
						m = (it0->c + it1->c + 2) / 2;
					} else if (it0->idx > it1->idx) {
						m = (it0->c + it1->c + 1) / 2;
					} else assert(false);
					assert(std::make_pair(it1->c - (m-1), it1->idx) > std::make_pair((m-1) - it0->c, it0->idx));
					assert(std::make_pair(it1->c - m, it1->idx) < std::make_pair(m - it0->c, it0->idx));

					m = std::clamp(m, l, r);
					if (l < m) {
						nd[1].push_back(*it1);
						nd[1].back().st = l;
						nd[1].back().en = m;
					}
					if (m < r) {
						nd[0].push_back(*it0);
						nd[0].back().st = m;
						nd[0].back().en = r;
					}

					if (r < it0->en) {
						it0->st = r;
					} else {
						++it0;
					}
					if (r < it1->en) {
						it1->st = r;
					} else {
						++it1;
					}
				}

				d = std::move(nd);
			}

			std::reverse(d[1].begin(), d[1].end());
			for (auto& a : d[1]) {
				a.r = -a.r;
				a.c = -a.c;
				a.st = 1 - a.st;
				a.en = 1 - a.en;
				std::swap(a.st, a.en);
			}

			for (auto& v : d) {
				// let's just merge things if we can
				int L = 0;
				for (int i = 0; i < int(v.size()); i++) {
					assert(L <= i);
					if (L > 0 && v[L-1].idx == v[i].idx && v[L-1].en == v[i].st) {
						assert(v[L-1].r == v[i].r);
						assert(v[L-1].c == v[i].c);
						v[L-1].en = v[i].en;
					} else {
						v[L++] = v[i];
					}
				}
				v.resize(L);
			}
		}

		cerr << agents[0][0].size() << ' ' << agents[0][1].size() << ' ' << agents[1][0].size() << ' ' << agents[1][1].size() << '\n';

		auto lookup = [&](const std::vector<int>& V, int v) -> int {
			return int(std::lower_bound(V.begin(), V.end(), v) - V.begin());
		};

		std::vector<int> ans(N);

		for (const auto& d : agents) {
			for (const auto& v : d) {
				for (const auto& a : v) {
					assert(a.en > a.st);
					ans[a.idx] += a.en - a.st;
				}
			}
		}
		//for (int i = 0; i < N; i++) { cerr << ans[i] << ' '; } cerr << '\n';

		auto solve_cross = [&](std::vector<agent_t> A, std::vector<agent_t> B) {
			if (A.empty() || B.empty()) return;
			for (int z = 0; z+1 < int(A.size()); z++) {
				assert(A[z].r <= A[z+1].r);
			}
			for (int z = 0; z+1 < int(B.size()); z++) {
				assert(B[z].r <= B[z+1].r);
			}

			std::vector<int> Xs; Xs.reserve(A.size());
			for (const auto& a : A) {
				if (Xs.empty() || a.r != Xs.back()) Xs.push_back(a.r);
			}

			int X = int(Xs.size());
			seg_tree::circular_layout Xlayout(X);
			std::vector<std::vector<int>> Ys(2*X);

			for (const auto& b : B) {
				auto Xrng = Xlayout.get_range(lookup(Xs, b.st), lookup(Xs, b.en));
				Xrng.for_each([&](auto x) -> void {
					if (Ys[x].empty() || b.r != Ys[x].back()) Ys[x].push_back(b.r);
				});
			}

			std::vector<std::vector<std::array<int, 2>>> seg(2*X);
			for (int z = 1; z < 2*X; z++) {
				seg[z].resize(2*Ys[z].size());
			}

			std::vector<std::pair<std::pair<int, int>, std::pair<bool, int>>> evts; evts.reserve(A.size() + B.size());
			for (int z = 0; z < int(A.size()); z++) {
				evts.push_back({{A[z].r + A[z].c, -A[z].idx}, {0, z}});
			}
			for (int z = 0; z < int(B.size()); z++) {
				evts.push_back({{B[z].r + B[z].c, -B[z].idx}, {1, z}});
			}
			std::sort(evts.begin(), evts.end());
			for (auto evt : evts) {
				auto [s, z] = evt.second;
				if (s == 0) {
					const auto& a = A[z];
					int res = 0;
					auto Xpt = Xlayout.get_point(lookup(Xs, a.r));
					Xpt.for_each([&](auto x) {
						int Y = int(Ys[x].size());
						if (Y == 0) return;
						seg_tree::circular_layout Ylayout(Y);
						auto Yrng = Ylayout.get_range(lookup(Ys[x], a.st), lookup(Ys[x], a.en));
						Yrng.for_each([&](auto y) -> void {
							res += seg[x][y][0];
							seg[x][y][1] ++;
						});
					});
					ans[a.idx] -= res;
				} else {
					const auto& b = B[z];
					int res = 0;
					auto Xrng = Xlayout.get_range(lookup(Xs, b.st), lookup(Xs, b.en));
					Xrng.for_each([&](auto x) -> void {
						int Y = int(Ys[x].size());
						if (Y == 0) return;
						seg_tree::circular_layout Ylayout(Y);
						auto Ypt = Ylayout.get_point(lookup(Ys[x], b.r));
						Ypt.for_each([&](auto y) -> void {
							res += seg[x][y][1];
							seg[x][y][0] ++;
						});
					});
					ans[b.idx] -= res;
				}
			}
		};

		solve_cross(agents[0][0], agents[1][0]);
		solve_cross(agents[0][1], agents[1][1]);
		for (auto& d : agents) {
			for (auto& v : d) {
				for (auto& a : v) {
					a.r = -a.r;
				}
				std::reverse(v.begin(), v.end());
			}
		}
		solve_cross(agents[0][1], agents[1][0]);
		solve_cross(agents[0][0], agents[1][1]);

		//for (int i = 0; i < N; i++) { cerr << ans[i] << ' '; } cerr << '\n';

		using num = modnum<int(1e9)+7>;
		num tot = 0;
		for (int i = 0; i < N; i++) {
			tot += num(i+1) * num(ans[i]);
		}
		cout << "Case #" << case_num << ": " << tot << '\n';
	}

	return 0;
}
