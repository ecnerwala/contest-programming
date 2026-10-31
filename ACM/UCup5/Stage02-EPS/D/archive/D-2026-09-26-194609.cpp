#include <bits/stdc++.h>
#include <cassert>

namespace wala {

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
	friend bool angle_less_from(const Point& base, const Point& s, const Point& t) {
		int r = is_reflex(base, s) - is_reflex(base, t);
		return r ? (r < 0) : (0 < cross(s, t));
	}
	// operator < (s,t) for angles in (base,base+2pi]
	friend bool angle_less_upto(const Point& base, const Point& s, const Point& t) {
		int r = is_reflex(t, base) - is_reflex(s, base);
		return r ? (r < 0) : (0 < cross(s, t));
	}

	friend auto angle_cmp_from(const Point& base) {
		return [base](const Point& s, const Point& t) { return angle_less_from(base, s, t); };
	}
	friend auto angle_cmp_upto(const Point& base) {
		return [base](const Point& s, const Point& t) { return angle_less_upto(base, s, t); };
	}
	friend auto angle_cmp_center_from(const Point& center, const Point& dir) {
		return [center, dir](const Point& s, const Point& t) -> bool { return angle_less_from(dir, s-center, t-center); };
	}
	friend auto angle_cmp_center_upto(const Point& center, const Point& dir) {
		return [center, dir](const Point& s, const Point& t) -> bool { return angle_less_upto(dir, s-center, t-center); };
	}

	// is p in [s,t] taken ccw? 1/0/-1 for in/border/out
	friend int angle_between(const Point& s, const Point& t, const Point& p) {
		if (same_dir(p, s) || same_dir(p, t)) return 0;
		return angle_less_from(s, p, t) ? 1 : -1;
	}
};

} // namespace wala

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, W; int64_t U, V; std::cin >> N >> W >> U >> V;
		using dbl = long double;
		using pt_t = wala::Point<int, int64_t>;
		using pd = wala::Point<dbl>;
		std::vector<pt_t> car(N);
		for (auto& p : car) std::cin >> p;
		int Q; std::cin >> Q;
		std::vector<std::pair<int, int>> S(Q);
		for (int q = 0; q < Q; q++) { std::cin >> S[q].first; S[q].second = q; }
		std::sort(S.begin(), S.end());

		if (U < 0) {
			U = -U;
			for (auto& p : car) p.x = -p.x;
			std::reverse(car.begin(), car.end());
			for (auto& s : S) s.first *= -1;
			std::reverse(S.begin(), S.end());
		}
		assert(U >= 0);
		assert(V > 0);

		auto sq = [](int64_t a) -> __int128_t { return __int128_t(a) * __int128_t(a); };
		// distance in V units
		auto get_time = [&](pt_t P) -> std::optional<dbl> {
			if (U >= V && P.x >= 0) {
				return std::nullopt;
			}
			dbl res;
			if (U == V) {
				assert(P.x < 0);
				res = dbl(int64_t(P.x) * int64_t(P.x) + int64_t(P.y) * int64_t(P.y)) / dbl(-P.x) * 0.5;
			} else {
				__int128_t dx = sq(V * P.y) + sq(V * P.x) - sq(U * P.y);
				if (dx < 0) {
					assert(U > V);
					return std::nullopt;
				}

				// This should always be the right direction
				res = (dbl(P.x * U) + std::sqrt(dbl(dx))) / dbl(V * V - U * U) * dbl(V);
			}
			//assert(res >= 0);
			/*
			pd tgt = pd(P) + pd(res, 0);
			std::cerr << abs(tgt) << '/' << res << ' ' << V << '/' << U << '\n';
			std::cerr << abs(tgt) / res << ' ' << dbl(V) / dbl(U) << '\n';
			*/
			return res;
		};

		{
			auto mi = std::min_element(car.begin(), car.end(), [&](auto a, auto b) -> bool {
				return std::make_pair(a.y, a.x) < std::make_pair(b.y, b.x);
			});
			std::rotate(car.begin(), mi, car.end());
		}
		std::vector<pt_t> upper; upper.reserve(N);
		std::vector<pt_t> lower; lower.reserve(N);
		{
			auto ma = std::max_element(car.begin(), car.end(), [&](auto a, auto b) -> bool {
				return std::make_pair(a.y, a.x) < std::make_pair(b.y, b.x);
			});
			for (auto p : std::span(car.begin(), ma+1)) {
				while (upper.size() >= 2 && cross3(upper.end()[-2], upper.end()[-1], p) <= 0) upper.pop_back();
				upper.push_back(p);
			}
			std::reverse(car.begin() + 1, car.end());
			ma = std::max_element(car.begin(), car.end(), [&](auto a, auto b) -> bool {
				return std::make_pair(a.y, a.x) < std::make_pair(b.y, b.x);
			});
			for (auto p : std::span(car.begin(), ma+1)) {
				while (lower.size() >= 2 && cross3(lower.end()[-2], lower.end()[-1], p) >= 0) lower.pop_back();
				lower.push_back(p);
			}
			std::reverse(car.begin() + 1, car.end());
			if (upper.size() >= 2 && upper[0].y == upper[1].y) upper.erase(upper.begin());
			if (lower.size() >= 2 && lower.end()[-1].y == lower.end()[-2].y) lower.pop_back();
		}

		// Prune away the time when we can go vertical
		pt_t safe_dir(-int(U), int(V));
		while (upper.size() >= 2 && cross(safe_dir, upper.end()[-1] - upper.end()[-2]) >= 0) upper.pop_back();
		while (lower.size() >= 2 && cross(safe_dir, lower.end()[-1] - lower.end()[-2]) <= 0) lower.pop_back();

		//for (auto& p : upper) { std::cerr << p << '\n'; } std::cerr << '\n';
		//for (auto& p : lower) { std::cerr << p << '\n'; } std::cerr << '\n';

		auto setmin = [](auto& a, auto b) { if (b < a) a = b; };
		const dbl INF = 1e99;
		std::vector<dbl> ans(Q, 2 * INF);
		{
			auto it = S.rbegin();
			for (; it != S.rend() && cross(safe_dir, upper.back() - pt_t(it->first, 0)) >= 0; ++it) {
				setmin(ans[it->second], W);
			}
			dbl t_cur = W - upper.back().y;
			for (int i = int(upper.size()) - 1; i >= 0; i--) {
				for (; it != S.rend() && (i == 0 || cross(upper[i] - upper[i-1], upper[i] - pt_t(it->first, 0)) >= 0); ++it) {
					auto t = get_time(upper[i] - pt_t(it->first, 0));
					if (!t) break;
					setmin(ans[it->second], t_cur + *t);
				}
				if (i > 0) {
					auto t = get_time(upper[i] - upper[i-1]);
					if (!t) break;
					t_cur += *t;
				}
			}
		}
		{
			auto it = S.begin();
			for (; it != S.end() && cross(safe_dir, lower.back() - pt_t(it->first, 0)) <= 0; ++it) {
				setmin(ans[it->second], W);
			}
			dbl t_cur = W - lower.back().y;
			for (int i = int(lower.size()) - 1; i >= 0; i--) {
				for (; it != S.end() && (i == 0 || cross(lower[i] - lower[i-1], lower[i] - pt_t(it->first, 0)) <= 0); ++it) {
					auto t = get_time(lower[i] - pt_t(it->first, 0));
					assert(t);
					setmin(ans[it->second], t_cur + *t);
				}
				if (i > 0) {
					auto t = get_time(lower[i] - lower[i-1]);
					assert(t);
					t_cur += *t;
				}
			}
			assert(it == S.end());
		}
		std::cout << std::fixed << std::setprecision(20);
		for (int q = 0; q < Q; q++) {
			std::cout << ans[q] / dbl(V) << '\n';
		}
	}

	return 0;
}
