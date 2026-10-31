#include <bits/stdc++.h>
#include <cassert>

template <typename T, typename AreaT=T> struct Point {
public:
	T x, y;
	Point() : x(0), y(0) {}
	Point(T x_, T y_) : x(x_), y(y_) {}
	template <typename U, typename V> explicit Point(const Point<U, V>& p) : x(p.x), y(p.y) {}
	Point(const std::pair<T, T>& p) : x(p.first), y(p.second) {}
	Point(const std::complex<T>& p) : x(real(p)), y(imag(p)) {}
	explicit operator std::pair<T, T> () const { return std::pair<T, T>(x, y); }
	explicit operator std::complex<T> () const { return std::complex<T>(x, y); }
	auto as_pair() const { return std::pair<T, T>(*this); }
	auto as_complex() const { return std::complex<T>(*this); }

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

	AreaT dist2() const { return AreaT(x) * AreaT(x) + AreaT(y) * AreaT(y); }
	auto dist() const { return std::sqrt(dist2()); }
	Point unit() const { return *this / this->dist(); }
	auto angle() const { return std::atan2(y, x); }

	T int_norm() const { return std::gcd(x,y); }
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

	friend AreaT dot(const Point& a, const Point& b) { return AreaT(a.x) * AreaT(b.x) + AreaT(a.y) * AreaT(b.y); }
	friend AreaT cross(const Point& a, const Point& b) { return AreaT(a.x) * AreaT(b.y) - AreaT(a.y) * AreaT(b.x); }
	friend AreaT cross3(const Point& a, const Point& b, const Point& c) { return cross(b-a, c-a); }

	// Complex numbers and rotation
	friend Point conj(const Point& a) { return Point(a.x, -a.y); }

	// Returns conj(a) * b
	friend Point dot_cross(const Point& a, const Point& b) { return Point(dot(a, b), cross(a, b)); }
	friend Point cmul(const Point& a, const Point& b) { return dot_cross(conj(a), b); }
	friend Point cdiv(const Point& a, const Point& b) { return dot_cross(b, a) / b.dist2(); }

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
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, Q; std::cin >> N >> Q;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;

		using pt_t = Point<int64_t, int64_t>;
		std::vector<pt_t> pref(N+1);
		for (int i = 0; i < N; i++) {
			pref[i+1] = pref[i] + pt_t{1, A[i]};
		}

		auto upper_hull = [&](const std::vector<pt_t>& pts) {
			std::vector<pt_t> res; res.reserve(pts.size());
			for (auto p : pts) {
				while (res.size() >= 2 && cross3(res.end()[-2], res.end()[-1], p) >= 0) res.pop_back();
				res.push_back(p);
			}
			return res;
		};

		std::vector<pt_t> maxs(N);
		// initialize to anything
		for (int i = 0; i < N; i++) maxs[i] = pref[i+1];
		auto try_max = [&](pt_t a) -> void {
			assert(1 <= a.x && a.x <= N);
			maxs[a.x-1].y = std::max(maxs[a.x-1].y, a.y);
		};

		[&](this auto&& self, int lo, int hi) -> void {
			assert(hi - lo >= 1);
			if (hi - lo == 1) return;
			int md = lo + (hi-lo)/2;
			self(lo, md);
			self(md, hi);
			std::vector<pt_t> los(pref.begin() + lo, pref.begin() + md);
			std::reverse(los.begin(), los.end()); for (auto& p : los) { p = -p; }
			std::vector<pt_t> his(pref.begin() + md, pref.begin() + hi);
			los = upper_hull(los);
			his = upper_hull(his);
			try_max(los[0] + his[0]);
			for (int i = 0, j = 0; i+1 < int(los.size()) || j+1 < int(his.size()); ) {
				bool do_i = i+1 < int(los.size()) && (j+1 == int(his.size()) || cross(los[i+1] - los[i], his[j+1] - his[j]) < 0);
				if (do_i) i++;
				else j++;

				try_max(los[i] + his[j]);
			}
		}(0, N+1);

		maxs = upper_hull(maxs);

		std::vector<std::pair<int64_t, int>> queries(Q);
		for (int q = 0; q < Q; q++) {
			std::cin >> queries[q].first;
			queries[q].second = q;
		}
		std::sort(queries.begin(), queries.end());
		std::vector<int64_t> ans(Q);
		int i = 0;
		for (auto [v, q_id] : queries) {
			pt_t cnd{v, 1};
			while (i+1 < int(maxs.size()) && dot(maxs[i+1] - maxs[i], cnd) >= 0) i++;
			ans[q_id] = dot(maxs[i], cnd);
		}

		for (int q = 0; q < Q; q++) std::cout << ans[q] << " \n"[q+1==Q];
	}

	return 0;
}
