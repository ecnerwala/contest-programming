#include<bits/stdc++.h>
using namespace std;

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

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

	Point& operator += (const Point& p) { x += p.x, y += p.y; }
	Point& operator -= (const Point& p) { x -= p.x, y -= p.y; }
	Point& operator *= (const T& t) { x *= t, y *= t; }
	Point& operator /= (const T& t) { x /= t, y /= t; }

	friend Point operator + (const Point& a, const Point& b) { return Point(a.x+b.x, a.y+b.y); }
	friend Point operator - (const Point& a, const Point& b) { return Point(a.x-b.x, a.y-b.y); }
	friend Point operator * (const Point& a, const T& t) { return Point(a.x*t, a.y*t); }
	friend Point operator * (const T& t ,const Point& a) { return Point(t*a.x, t*a.y); }
	friend Point operator / (const Point& a, const T& t) { return Point(a.x/t, a.y/t); }

	friend T norm(const Point& a) { return a.x * a.x + a.y * a.y; }
	template <typename U=T> friend U abs(const Point& p) { return sqrt(U(norm(p))); }
	friend T unit(const Point& a) { if (a == Point()) return a; return a / abs(a); }

	friend T int_norm(const Point& a) { return __gcd(a.x,a.y); }
	friend T int_unit(const Point& a) { if (a == Point()) return a; return a / int_norm(a); }

	friend T cross(const Point& a, const Point& b) { return a.x * b.y - a.y * b.x; }
	friend T dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }
	friend T area(const Point& a, const Point& b, const Point& c) { return cross(b-a, c-a); }

	// Returns conj(a) * b
	friend Point rotation(const Point& a, const Point& b) { return Point(dot(a, b), cross(a, b)); }

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

	// is x in [s,t] taken ccw? 1/0/-1 for in/border/out
	friend int angle_between(const Point& s, const Point& t, const Point& x) {
		if (same_dir(x, s) || same_dir(x, t)) return 0;
		return angle_less(s, x, t) ? 1 : -1;
	}
};

template <class To, class From>
typename std::enable_if<
	(sizeof(To) == sizeof(From)) &&
	std::is_trivially_copyable<From>::value &&
	std::is_trivial<To>::value,
	// this implementation requires that To is trivially default constructible
	To>::type
// constexpr support needs compiler magic
bit_cast(const From &src) noexcept
{
	To dst;
	std::memcpy(&dst, &src, sizeof(To));
	return dst;
}

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);

	using Pt = Point<double>;

	int N; double V; cin >> N >> V;
	vector<pair<Pt, Pt>> S(N);
	for (int i = 0; i < N; i++) {
		cin >> S[i].first >> S[i].second;
	}

	auto getTime = [&](Pt d, Pt v) -> double {
		double mi = 0, ma = 1e15;
		while (bit_cast<int64_t>(ma) - bit_cast<int64_t>(mi) > 1) {
			double md = bit_cast<double>(bit_cast<int64_t>(mi) + (bit_cast<int64_t>(ma) - bit_cast<int64_t>(mi)) / 2);
			if (abs(d + md * v) <= md * V) {
				ma = md;
			} else {
				mi = md;
			}
		}
		return ma;
	};
	auto getTimeStart = [&](double t0, pair<Pt, Pt> r0, pair<Pt, Pt> r1) -> double {
		return t0 + getTime(r1.first - r0.first + (r1.second - r0.second) * t0, r1.second);
	};

	vector<vector<double>> dp(1 << N, vector<double>(N, 1e18));
	for (int i = 0; i < N; i++) {
		dp[1<<i][i] = getTime(S[i].first, S[i].second);
	}
	for (int m = 1; m < (1<<N); m++) {
		for (int i = 0; i < N; i++) {
			if (!(m & (1<<i))) continue;
			for (int j = 0; j < N; j++) {
				if (m & (1<<j)) continue;
				setmin(dp[m|(1<<j)][j], getTimeStart(dp[m][i], S[i], S[j]));
			}
		}
	}
	double ans = *min_element(dp[(1<<N)-1].begin(), dp[(1<<N)-1].end());
	cout << fixed << setprecision(20) << ans << '\n';

	return 0;
}
