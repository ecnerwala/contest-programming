#include <bits/stdc++.h>

template <typename T> struct Point {
public:
	T x, y;
	Point() : x(0), y(0) {}
	Point(T x_, T y_) : x(x_), y(y_) {}
	template <typename U> explicit Point(const Point<U>& p) : x(p.x), y(p.y) {}
	Point(const std::pair<T, T>& p) : x(p.first), y(p.second) {}
	Point(const std::complex<T>& p) : x(real(p)), y(imag(p)) {}
	explicit operator std::pair<T, T> () const { return std::pair<T, T>(x, y); }
	explicit operator std::complex<T> () const { return std::complex<T>(x, y); }

	friend std::ostream& operator << (std::ostream& o, const Point& p) { return o << '(' << p.x << ',' << p.y << ')'; }
	friend std::istream& operator >> (std::istream& i, Point& p) { return i >> p.x >> p.y; }
	friend bool operator == (const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
	friend bool operator != (const Point& a, const Point& b) { return !(a==b); }

	Point operator + () const { return Point(+x, +y); }
	Point operator - () const { return Point(-x, -y); }

	Point& operator += (const Point& p) { x += p.x, y += p.y; return *this; }
	Point& operator -= (const Point& p) { x -= p.x, y -= p.y; return *this; }
	Point& operator *= (const T& t) { x *= t, y *= t; return *this; }
	Point& operator /= (const T& t) { x /= t, y /= t; return *this; }

	friend Point operator + (const Point& a, const Point& b) { return Point(a.x+b.x, a.y+b.y); }
	friend Point operator - (const Point& a, const Point& b) { return Point(a.x-b.x, a.y-b.y); }
	friend Point operator * (const Point& a, const T& t) { return Point(a.x*t, a.y*t); }
	friend Point operator * (const T& t ,const Point& a) { return Point(t*a.x, t*a.y); }
	friend Point operator / (const Point& a, const T& t) { return Point(a.x/t, a.y/t); }

	T dist2() const { return x * x + y * y; }
	auto dist() const { return std::sqrt(dist2()); }
	Point unit() const { return *this / this->dist(); }
	auto angle() const { return std::atan2(y, x); }

	T int_norm() const { return __gcd(x,y); }
	Point int_unit() const { if (!x && !y) return *this; return *this / this->int_norm(); }

	// Convenient free-functions, mostly for generic interop
	friend auto norm(const Point& a) { return a.dist2(); }
	friend auto abs(const Point& a) { return a.dist(); }
	friend auto unit(const Point& a) { return a.unit(); }
	friend auto arg(const Point& a) { return a.angle(); }
	friend auto int_norm(const Point& a) { return a.int_norm(); }
	friend auto int_unit(const Point& a) { return a.int_unit(); }

	Point perp_cw() const { return Point(y, -x); }
	Point perp_ccw() const { return Point(-y, x); }

	friend T dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }
	friend T cross(const Point& a, const Point& b) { return a.x * b.y - a.y * b.x; }
	friend T cross3(const Point& a, const Point& b, const Point& c) { return cross(b-a, c-a); }

	// Complex numbers and rotation
	friend Point conj(const Point& a) { return Point(a.x, -a.y); }

	// Returns conj(a) * b
	friend Point dot_cross(const Point& a, const Point& b) { return Point(dot(a, b), cross(a, b)); }
	friend Point cmul(const Point& a, const Point& b) { return dot_cross(conj(a), b); }
	friend Point cdiv(const Point& a, const Point& b) { return dot_cross(b, a) / b.norm(); }

	// Must be a unit vector; otherwise multiplies the result by abs(u)
	Point rotate(const Point& u) const { return dot_cross(conj(u), *this); }
	Point unrotate(const Point& u) const { return dot_cross(u, *this); }

	friend bool lex_less(const Point& a, const Point& b) {
		return std::tie(a.x, a.y) < std::tie(b.x, b.y);
	}

	friend bool same_dir(const Point& a, const Point& b) { return cross(a,b) == 0 && dot(a,b) > 0; }

	// check if 180 <= s..t < 360
	friend bool is_reflex(const Point& a, const Point& b) { auto c = cross(a,b); return c ? (c < 0) : (dot(a, b) < 0); }

	// operator < (s,t) for angles in [base,base+2pi)
	friend bool angle_less(const Point& base, const Point& s, const Point& t) {
		int r = is_reflex(base, s) - is_reflex(base, t);
		return r ? (r < 0) : (0 < cross(s, t));
	}

	friend auto angle_cmp(const Point& base) {
		return [base](const Point& s, const Point& t) { return angle_less(base, s, t); };
	}
	friend auto angle_cmp_center(const Point& center, const Point& dir) {
		return [center, dir](const Point& s, const Point& t) -> bool { return angle_less(dir, s-center, t-center); };
	}

	// is p in [s,t] taken ccw? 1/0/-1 for in/border/out
	friend int angle_between(const Point& s, const Point& t, const Point& p) {
		if (same_dir(p, s) || same_dir(p, t)) return 0;
		return angle_less(s, p, t) ? 1 : -1;
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

	int N, M; cin >> N >> M;
	struct range_t {
		std::array<int, 2> bounds;
		int st, en;
	};
	std::vector<range_t> ranges; ranges.reserve(N+M);
	for (int i = 0; i < N; i++) {
		int l, r; cin >> l >> r;
		// insert before query 0, delete before query M
		ranges.push_back({{l, r}, 0, M});
	}
	std::vector<int> cur_range(N);
	std::iota(cur_range.begin(), cur_range.end(), 0);
	for (int j = 0; j < M; j++) {
		int x, l, r; cin >> x >> l >> r; x--;
		ranges[cur_range[x]].en = j;
		cur_range[x] = int(ranges.size());
		ranges.push_back({{l, r}, j, M});
	}
	for (auto& r : ranges) {
		// make L an upper bound on losses
		r.bounds[0] = 2*(N-1) - r.bounds[0];
	}

	std::vector<int64_t> ans(M);
	for (int z = 0; z < 2; z++) {
		std::sort(ranges.begin(), ranges.end(), [&](const auto& a, const auto& b) -> bool {
			return a.bounds[z] < b.bounds[z];
		});
		std::vector<std::tuple<int, bool, int>> evts; evts.reserve(ranges.size() * 2);
		for (int r = 0; r < int(ranges.size()); r++) {
			evts.push_back({ ranges[r].st, 0, r });
			evts.push_back({ ranges[r].en, 1, r });
		}
		std::sort(evts.begin(), evts.end());

		seg_tree::in_order_layout layout(N+M);
		using pt_t = Point<int64_t>;
		struct seg_t {
			int cnt = 0;
			int64_t tot = 0;
			bool is_real = false;
			std::array<pt_t, 2> tan;
		};
		std::vector<seg_t> seg(layout.N * 2);
		auto update_node = [&](seg_tree::point a) -> void {
			if (a >= layout.N) {
				assert(seg[a].cnt == 0 || seg[a].cnt == 1);
				int r = layout.get_leaf_index(a);
				if (seg[a].cnt == 1) {
					seg[a].is_real = true;
					seg[a].tot = -ranges[r].bounds[z];
					seg[a].tan[0] = seg[a].tan[1] = {1, -ranges[r].bounds[z]};
				} else {
					seg[a].is_real = false;
					seg[a].tot = 0;
				}
				return;
			}

			seg[a].cnt = seg[a.c(0)].cnt + seg[a.c(1)].cnt;
			seg[a].tot = seg[a.c(0)].tot + seg[a.c(1)].tot + 2 * int64_t(seg[a.c(0)].cnt) * int64_t(seg[a.c(1)].cnt);
			if (seg[a.c(0)].cnt == 0 || seg[a.c(1)].cnt == 0) {
				// do nothing, pass through
				seg[a].is_real = false;
				return;
			}
			seg_tree::point pl = a.c(0), pr = a.c(1);
			pt_t offl = {0, 0};
			pt_t offr = {seg[pl].cnt, seg[pl].tot};
			auto m = [&](pt_t a, pt_t b) -> pt_t {
				return pt_t{a.x + b.x, a.y + b.y + 2 * int64_t(a.x) * int64_t(b.x)};
			};
			while (true) {
				assert(seg[pl].cnt > 0 && seg[pr].cnt > 0);
				if (!seg[pl].is_real) {
					assert(seg[pl.c(0)].cnt == 0 || seg[pl.c(1)].cnt == 0);
					pl = seg[pl.c(0)].cnt ? pl.c(0) : pl.c(1);
					continue;
				}
				if (!seg[pr].is_real) {
					assert(seg[pr.c(0)].cnt == 0 || seg[pr.c(1)].cnt == 0);
					pr = seg[pr.c(0)].cnt ? pr.c(0) : pr.c(1);
					continue;
				}

				assert(seg[pl].is_real && seg[pr].is_real);
				if (seg[pl].cnt == 1 && seg[pr].cnt == 1) {
					seg[a].tan = {m(offl, seg[pl].tan[0]), m(offr, seg[pr].tan[0])};
				}

				pt_t vl0 = m(offl, seg[pl].tan[0]);
				pt_t vl1 = m(offl, seg[pl].tan[1]);
				pt_t vr0 = m(offr, seg[pr].tan[0]);
				pt_t vr1 = m(offr, seg[pr].tan[1]);
				assert(vl0.x < vl1.x);
				assert(vl1.x < vr0.x);
				assert(vr0.x < vr1.x);
				if (seg[pr].cnt > 1 && cross3(vl1, vr0, vr1) >= 0) {
					offr = m(offr, {seg[pr.c(0)].cnt, seg[pr.c(0)].tot});
					pr = pr.c(1);
					continue;
				} else if (seg[pl].cnt == 1) {
					assert(seg[pr].cnt > 1);
					// go the other way
					pr = pr.c(0);
					continue;
				}

				if (seg[pl].cnt > 1 && cross3(vl0, vl1, vr0) >= 0) {
					pl = pl.c(0);
					continue;
				} else if (seg[pr].cnt == 1) {
					assert(seg[pl].cnt > 1);
					offl = m(offl, {seg[pl.c(0)].cnt, seg[pl.c(0)].tot});
					pl = pl.c(1);
					continue;
				}

				assert(seg[pl].cnt > 1 && seg[pr].cnt > 1);
				// Now, both sides are convex, so we need to decide which half based on the split
				int64_t split_x = seg[a.c(0)].cnt;
				assert(vl1.x <= split_x);
				assert(vr0.x >= split_x);
				// Now, kill whichever side is bigger when extrapolated to split_x
				// vl1.y + (split_x - vl1.x) * (vl1.y - vl0.y) / (vl1.x - vl0.x)
				// ((vl0.y * vl1.x - vl1.y * vl0.x) + split_x * (vl1.y - vl0.y)) / (vl1.x - vl0.x)
				int64_t numl = vl0.y * vl1.x - vl1.y * vl0.x + split_x * (vl1.y - vl0.y);
				int64_t denl = vl1.x - vl0.x;
				int64_t numr = vr0.y * vr1.x - vr1.y * vr0.x + split_x * (vr1.y - vr0.y);
				int64_t denr = vr1.x - vr0.x;
				assert(denl > 0 && denr > 0);
				if (__int128_t(numl) * denr >= __int128_t(numr) * denl) {
					// the left is higher, so kill the left half
					offl = m(offl, {seg[pl.c(0)].cnt, seg[pl.c(0)].tot});
					pl = pl.c(1);
				} else {
					pr = pr.c(0);
				}
				continue;
			}
		};

		auto it = evts.begin();
		for (int q = 0; q < M; q++) {
			for (; it != evts.end() && std::get<0>(*it) <= q; ++it) {
				assert(std::get<0>(*it) == q);
				bool rem = std::get<1>(*it);
				int r = std::get<2>(*it);
				auto a = layout.get_point(r);
				if (rem) {
					assert(seg[a].cnt == 1);
					seg[a].cnt = 0;
				} else {
					assert(seg[a].cnt == 0);
					seg[a].cnt = 1;
				}
				update_node(a);
				a.for_parents_up(update_node);
			}
		}
	}
	for (int m = 0; m < M; m++) {
		cout << ans[m] << '\n';
	}

	return 0;
}
