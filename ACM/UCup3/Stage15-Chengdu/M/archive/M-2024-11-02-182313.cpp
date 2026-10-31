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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASE_NUM; cin >> CASE_NUM;
	while (CASE_NUM--) {
		using ld = long double;
		using pt_t = Point<ld>;
		pt_t X_L; ld Z_L; pt_t V_L; cin >> X_L >> Z_L >> V_L;
		std::array<std::vector<pt_t>, 2> P;
		pt_t V_side_1(0, 0);
		for (auto side = 0; side < 2; side++) {
			auto& P_i = P[side];
			int N; ld Z; cin >> N >> Z;
			P_i.resize(N);
			for (auto& p : P_i) {
				pt_t a; cin >> a;
				p = a + (a - X_L) * Z / (Z_L - Z);
			}
			pt_t side_vel = -V_L * Z / (Z_L - Z);
			if (side == 0) V_side_1 -= side_vel;
			else V_side_1 += side_vel;
		}

		int Q; cin >> Q;
		std::vector<std::array<int, 2>> queries(Q);
		for (auto& [t0, t1] : queries) cin >> t0 >> t1;
		// P[1] moves with velocity V_side_1
		pt_t V_dir = V_side_1.perp_cw().unit();
		for (auto& P_i : P) {
			for (auto& p : P_i) p = p.unrotate(V_dir);
		}
		ld S_side_1 = V_side_1.dist();
		V_side_1 = V_side_1.unrotate(V_dir);
		cerr << V_side_1 << ' ' << S_side_1 << '\n';

		int T = 1024;
		seg_tree::in_order_layout layout(T);
		std::vector<ld> tot_area_at(T+1);
		std::vector<ld> tot_integ_at(T+1);
		int t_min = 0, t_max = T;
		struct seg_node {
			ld l, m, r;
		};
		std::vector<seg_node> seg(2*T);

		// P[1] moves in the positive y direction with speed S_side_1
		std::array<std::vector<pt_t>, 4> hull;
		for (int side = 0; side < 2; side++) {
			auto& P_i = P[side];
			auto& lower = hull[side * 2 + 0];
			auto& upper = hull[side * 2 + 1];
			auto lex_cmp = [](pt_t a, pt_t b) -> bool { return std::pair<ld, ld>(a) < std::pair<ld, ld>(b); };
			int i_min = int(std::min_element(P_i.begin(), P_i.end(), lex_cmp) - P_i.begin());
			int i_max = int(std::max_element(P_i.begin(), P_i.end(), lex_cmp) - P_i.begin());
			assert(i_min != i_max);
			if (i_min < i_max) {
				lower.insert(lower.end(), P_i.begin() + i_min, P_i.begin() + i_max + 1);
				upper.insert(upper.end(), P_i.begin() + i_max, P_i.end());
				upper.insert(upper.end(), P_i.begin(), P_i.begin() + i_min + 1);
			} else {
				lower.insert(lower.end(), P_i.begin() + i_min, P_i.end());
				lower.insert(lower.end(), P_i.begin(), P_i.begin() + i_max + 1);
				upper.insert(upper.end(), P_i.begin() + i_max, P_i.begin() + i_min + 1);
			}
			std::reverse(upper.begin(), upper.end());
		}

		std::array<int, 4> idx{0, 0, 0, 0};
		while (true) {
			std::pair<ld, int> best_st;
			std::pair<ld, int> best_en;
			for (int z = 0; z < 4; z++) {
				auto p1 = hull[z][idx[z]];
				auto p2 = hull[z][idx[z]+1];
				std::pair<ld, int> cnd_st = {p1.x, z};
				std::pair<ld, int> cnd_en = {p2.x, z};
				if (!z) { best_st = cnd_st, best_en = cnd_en; }
				else {
					best_st = std::max(best_st, cnd_st);
					best_en = std::min(best_en, cnd_en);
				}
			}
			ld x_st = best_st.first;
			ld x_en = best_en.first;
			if (x_st < x_en) {
				std::array<ld, 4> y_sts;
				std::array<ld, 4> y_ens;
				for (int z = 0; z < 4; z++) {
					auto p1 = hull[z][idx[z]];
					auto p2 = hull[z][idx[z]+1];
					ld x1 = p1.x;
					ld x2 = p2.x;
					auto get_y = [&](ld x) -> ld {
						ld dy = p2.y - p1.y;
						ld dx1 = (x - x1);
						ld dx2 = (x2 - x);
						if (dx1 + dx2 <= ld(0)) return p1.y;
						if (dx1 < dx2) {
							return p1.y + std::clamp<ld>(dx1 / (dx1 + dx2), 0, 1) * dy;
						} else {
							return p2.y - std::clamp<ld>(dx2 / (dx1 + dx2), 0, 1) * dy;
						}
					};
					y_sts[z] = get_y(x_st);
					y_ens[z] = get_y(x_en);
				}
				auto lineInter = [&](pt_t s1, pt_t e1, pt_t s2, pt_t e2) -> pair<int, pt_t> {
					auto d = cross(e1 - s1, e2 - s2);
					if (d == 0) return {-cross3(s1, e1, s2) == 0, pt_t(0, 0)};
					auto p = cross3(s2, e1, e2), q = cross3(s2, e2, s1);
					return {1, (s1 * p + e1 * q) / d};
				};
				auto cut = [&](vector<pt_t> poly, pt_t s, pt_t e) -> vector<pt_t> {
					vector<pt_t> res;
					for(int i = 0; i < (int)poly.size(); i++){
						pt_t cur = poly[i], prev = i ? poly[i-1] : poly.back();
						bool side = cross3(s, e, cur) < 0;
						if (side != (cross3(s, e, prev) < 0)){
							res.push_back(lineInter(s, e, cur, prev).second);
						}
						if (side){
							res.push_back(cur);
						}
					}
					return res;
				};
				auto area_at = [&](ld timestamp) -> ld {
					vector<pt_t> trapezoid0({
						pt_t{x_st, y_sts[0]},
						pt_t{x_en, y_ens[0]},
						pt_t{x_en, y_ens[1]},
						pt_t{x_st, y_sts[1]}
					});
					ld delta_y = S_side_1 * timestamp;
					vector<pt_t> trapezoid1({
						pt_t{x_st, y_sts[2] + delta_y},
						pt_t{x_en, y_ens[2] + delta_y},
						pt_t{x_en, y_ens[3] + delta_y},
						pt_t{x_st, y_sts[3] + delta_y}
					});
					std::vector<pt_t> final_poly = trapezoid0;
					final_poly = cut(final_poly, trapezoid1[1], trapezoid1[0]);
					final_poly = cut(final_poly, trapezoid1[3], trapezoid1[2]);
					ld res = 0;
					for (int i = 1; i+1 < int(final_poly.size()); i++) {
						res += cross3(final_poly[0], final_poly[i], final_poly[i+1]);
					}
					return res;
				};
				auto integ_at = [&](ld t0, ld t1) -> ld {
					ld a0 = area_at(t0);
					ld amid = area_at(t0 + (t1 - t0) / 2);
					ld a1 = area_at(t1);
					return (a0 + amid * 4 + a1) / 6 * (t1 - t0);
				};
				{
					// now, compute the areas for each t range
					std::vector<ld> t_evts;
					t_evts.push_back(t_min);
					for (int zl : {0, 1}) {
						for (int zr : {2, 3}) {
							{
								ld t_cnd = (y_sts[zl] - y_sts[zr]) / S_side_1;
								if (t_min < t_cnd && t_cnd < t_max) t_evts.push_back(t_cnd);
							}
							{
								ld t_cnd = (y_ens[zl] - y_ens[zr]) / S_side_1;
								if (t_min < t_cnd && t_cnd < t_max) t_evts.push_back(t_cnd);
							}
						}
					}
					std::sort(t_evts.begin() + 1, t_evts.end());
					t_evts.push_back(t_max);
					int t_st_floor = t_min;
					tot_area_at[t_min] += area_at(t_min);
					for (int z = 0; z+1 < int(t_evts.size()); z++) {
						ld t_st = t_evts[z];
						ld t_en = t_evts[z+1];
						int t_en_floor = z+2 == int(t_evts.size()) ? t_max : int(t_en);
						t_en_floor = std::clamp(t_en_floor, t_min, t_max);
						if (t_en_floor > t_st_floor) {
							tot_integ_at[t_st_floor] += integ_at(t_st, t_st_floor + 1);
							if (t_en_floor - t_st_floor > 1) {
								// strictly in between
								auto rng = layout.get_range(t_st_floor + 1, t_en_floor);
								rng.for_each([&](seg_tree::point a) -> void {
									auto [t0, t1] = layout.get_node_bounds(a);
									seg[a].l += area_at(t0);
									seg[a].m += area_at(ld(t0 + t1) / 2);
									seg[a].r += area_at(t1);
								});
							}
							tot_area_at[t_en_floor] += area_at(t_en_floor);
							tot_integ_at[t_en_floor] += integ_at(t_en_floor, t_en);
							t_st_floor = t_en_floor;
						} else {
							tot_integ_at[t_st_floor] += integ_at(t_st, t_en);
						}
					}
					assert(t_st_floor == t_max);
				}
			}

			{
				int z = best_en.second;
				idx[z]++;
				if (idx[z] + 1 >= int(hull[z].size())) break;
			}
		}
		for (seg_tree::point a(1); a < T; a++) {
			ld l = seg[a].l, m = seg[a].m, r = seg[a].r;
			{
				seg[a.c(0)].l += l;
				seg[a.c(0)].m += (l * 3 + m * 5 + (m - r)) / 8;
				seg[a.c(0)].r += m;
			}
			{
				seg[a.c(1)].r += r;
				seg[a.c(1)].m += (r * 3 + m * 5 + (m - l)) / 8;
				seg[a.c(1)].l += m;
			}
		}
		for (int i = 0; i < T; i++) {
			seg_tree::point a = layout.get_point(i);
			tot_area_at[i] += seg[a].l;
			tot_integ_at[i] += (seg[a].l + seg[a].m * 4 + seg[a].r) / 6;
		}
		std::vector<ld> pref_integ(T+1);
		pref_integ[0] = 0;
		for (int i = 0; i < T; i++) {
			pref_integ[i+1] = pref_integ[i] + tot_integ_at[i];
		}
		for (auto [t0, t1] : queries) {
			if (t0 == t1) {
				cout << tot_area_at[t0] << '\n';
			} else {
				cout << fixed << setprecision(20) << '\n';
				cout << (pref_integ[t1] - pref_integ[t0]) / ld(t1 - t0) << '\n';
			}
		}
	}

	return 0;
}
