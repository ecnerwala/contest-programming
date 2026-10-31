#include<bits/stdc++.h>

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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, M; cin >> N >> M;
		cerr << "Case #" << case_num << " " << N << ' ' << M << '\n';

		using num = modnum<int(1e9)+7>;

		using pt_t = Point<int64_t>;
		struct seg_t {
			pt_t st;
			pt_t en;
			pt_t dir;
			int len;
		};
		int lo = 0;
		std::vector<seg_t> segs; segs.reserve(M+1);

		std::set<std::pair<int64_t, std::pair<int, int>>> xs;
		std::set<std::pair<int64_t, std::pair<int, int>>> ys;
		auto ins = [&](int idx) -> void {
			assert(lo <= idx && idx < int(segs.size()));
			xs.insert({segs[idx].st.x, {idx, 0}});
			xs.insert({segs[idx].en.x, {idx, 1}});
			ys.insert({segs[idx].st.y, {idx, 0}});
			ys.insert({segs[idx].en.y, {idx, 1}});
		};
		auto del = [&](int idx) -> void {
			assert(lo <= idx && idx < int(segs.size()));
			xs.erase({segs[idx].st.x, {idx, 0}});
			xs.erase({segs[idx].en.x, {idx, 1}});
			ys.erase({segs[idx].st.y, {idx, 0}});
			ys.erase({segs[idx].en.y, {idx, 1}});
		};

		pt_t dir = {1, 0};
		segs.push_back(seg_t{{1, 0}, {N, 0}, dir, N});
		ins(0);

		num ans = 0;

		for (int z = 0; z < M; z++) {
			{
				char turn; cin >> turn;
				if (turn == 'L') {
					dir = dir.perp_ccw();
				} else if (turn == 'R') {
					dir = dir.perp_cw();
				} else if (turn == 'S') {
					// dir = dir;
				} else assert(false);
			}
			{
				int new_len; cin >> new_len;
				// if we go N, we're already straight, so no need to keep going
				new_len = std::min(new_len, N);

				pt_t st = segs.back().en + dir;

				int64_t min_area = int64_t(N+1) * int64_t(N+1);
				int cur_len = 0;
				while (cur_len < new_len) {
					assert(lo < int(segs.size()));
					int delta = std::min(segs[lo].len - 1, new_len - cur_len);
					if (delta == 0) {
						assert(segs[lo].len == 1);
						delta = 1;
					}
					del(lo);

					// this includes 1 .. cur_len + 1
					int64_t min_x = std::min(st.x, (st + dir * cur_len).x);
					int64_t max_x = std::max(st.x, (st + dir * cur_len).x);
					int64_t min_y = std::min(st.y, (st + dir * cur_len).y);
					int64_t max_y = std::max(st.y, (st + dir * cur_len).y);
					if (!xs.empty()) {
						min_x = std::min(min_x, xs.begin()->first);
						max_x = std::max(max_x, xs.rbegin()->first);
					}
					if (!ys.empty()) {
						min_y = std::min(min_y, ys.begin()->first);
						max_y = std::max(max_y, ys.rbegin()->first);
					}

					if (delta == segs[lo].len) {
						assert(delta == 1);
						min_area = std::min(min_area, int64_t(max_x - min_x + 1) * int64_t(max_y - min_y + 1));
						cur_len += delta;
						lo++;
						continue;
					}

					min_x = std::min(min_x, segs[lo].en.x);
					max_x = std::max(max_x, segs[lo].en.x);
					min_y = std::min(min_y, segs[lo].en.y);
					max_y = std::max(max_y, segs[lo].en.y);

					struct linear_fn {
						int64_t v0;
						int64_t slope;
					};

					// fn, exclusive end_point pairs
					using piecewise_linear_fn = std::vector<std::pair<linear_fn, int64_t>>;

					auto iter_pair = [&](const piecewise_linear_fn& a, const piecewise_linear_fn& b, auto&& f) {
						assert(b.back().second == a.back().second);
						int64_t st = 0;
						for (auto it = a.begin(), jt = b.begin(); it != a.end() && jt != b.end(); ) {
							int64_t en = std::min(it->second, jt->second);
							assert(st < en);
							f(it->first, jt->first, st, en);
							if (it->second == en) {
								++it;
							}
							if (jt->second == en) {
								++jt;
							}
							st = en;
						}
					};

					auto do_min = [&](const piecewise_linear_fn& a, const piecewise_linear_fn& b) {
						piecewise_linear_fn res;
						iter_pair(a, b, [&](linear_fn x, linear_fn y, int64_t st, int64_t en) {
							assert(st == (res.empty() ? 0 : res.back().second));
							assert(st < en);
							int64_t x0 = x.v0 + st * x.slope;
							int64_t y0 = y.v0 + st * y.slope;
							int64_t x1 = x.v0 + (en-1) * x.slope;
							int64_t y1 = y.v0 + (en-1) * y.slope;
							if (x0 <= y0 && x1 <= y1) {
								res.push_back({x, en});
							} else if (y0 <= x0 && y1 <= x1) {
								res.push_back({y, en});
							} else {
								if (x0 > y0) {
									std::swap(x, y);
									std::swap(x0, y0);
									std::swap(x1, y1);
								}
								assert(x0 < y0 && y1 < x1);
								int64_t mi = st;
								int64_t ma = en-1;
								while (ma - mi > 1) {
									int64_t md = (mi + ma) / 2;
									if (x.v0 + md * x.slope <= y.v0 + md * y.slope) {
										mi = md;
									} else {
										ma = md;
									}
								}
								res.push_back({x, ma});
								res.push_back({y, en});
							}
						});
						return res;
					};
					auto neg = [&](piecewise_linear_fn a) {
						for (auto& [f, en] : a) {
							f.v0 = -f.v0;
							f.slope = -f.slope;
						}
						return a;
					};
					auto do_max = [&](const piecewise_linear_fn& a, const piecewise_linear_fn& b) {
						return neg(do_min(neg(a), neg(b)));
					};
					auto do_dist = [&](const piecewise_linear_fn a, const piecewise_linear_fn b) {
						piecewise_linear_fn res;
						iter_pair(a, b, [&](linear_fn x, linear_fn y, int64_t st, int64_t en) {
							assert(st == (res.empty() ? 0 : res.back().second));
							assert(st < en);
							res.push_back({{y.v0 - x.v0 + 1, y.slope - x.slope}, en});
						});
						return res;
					};

					// now, for i == 0 .. delta - 1, we need to insert st + dir * cur_len + dir * i and segs[lo].st + segs[lo].dir + segs[lo].dir * i
					pt_t a = st + dir * cur_len;
					pt_t a_dir = dir;
					pt_t b = segs[lo].st + segs[lo].dir;
					pt_t b_dir = segs[lo].dir;

					auto pf = [&](int64_t v0, int64_t slope) -> piecewise_linear_fn {
						return {{{v0, slope}, delta}};
					};
					piecewise_linear_fn width = do_dist(
						do_min(do_min(pf(min_x, 0), pf(a.x, a_dir.x)), pf(b.x, b_dir.x)),
						do_max(do_max(pf(max_x, 0), pf(a.x, a_dir.x)), pf(b.x, b_dir.x))
					);
					piecewise_linear_fn height = do_dist(
						do_min(do_min(pf(min_y, 0), pf(a.y, a_dir.y)), pf(b.y, b_dir.y)),
						do_max(do_max(pf(max_y, 0), pf(a.y, a_dir.y)), pf(b.y, b_dir.y))
					);
					iter_pair(width, height, [&](linear_fn a, linear_fn b, int64_t st, int64_t en) -> void {
						assert(st < en);
						// inclusive
						en--;

						{
							int64_t shift = (st + en) / 2;
							a.v0 += a.slope * shift;
							b.v0 += b.slope * shift;
							en -= shift;
							st -= shift;
						}

						auto check = [&](int64_t t) -> void {
							if (st <= t && t <= en) {
								min_area = std::min(min_area, (a.v0 + a.slope * t) * (b.v0 + b.slope * t));
							}
						};
						check(st);
						check(en);
						// see if the vertex is there
						int64_t cons = a.v0 * b.slope + b.v0 * a.slope;
						if (a.slope * b.slope != 0) {
							int64_t vert = -cons / (2 * a.slope * b.slope);
							check(vert - 1);
							check(vert);
							check(vert + 1);
						}
					});

					cur_len += delta;
					segs[lo].st += delta * segs[lo].dir;
					segs[lo].len -= delta;
					assert(segs[lo].len >= 1);
					ins(lo);
				}
				ans += num(min_area);
				segs.push_back({st, st + (new_len - 1) * dir, dir, new_len});
				ins(int(segs.size()) - 1);
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
