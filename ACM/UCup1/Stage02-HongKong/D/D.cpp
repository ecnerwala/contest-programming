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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;

	std::vector<std::vector<std::pair<int, int>>> in_edges(N);
	for (int e = 0; e < M; e++) {
		int u, v, c; cin >> u >> v >> c; u--, v--;
		in_edges[v].push_back({u, c});
	}
	int Q; cin >> Q;
	std::vector<int> ans(Q);
	std::vector<std::vector<std::pair<Point<int>, int>>> queries(N);
	for (int q = 0; q < Q; q++) {
		int a, b, x; cin >> a >> b >> x; x--;
		queries[x].push_back({{a, b}, q});
	}

	using hull_t = std::vector<Point<int>>;
	std::vector<hull_t> dp(N);

	for (int i = 0; i < N; i++) {
		if (i >= 1001) dp[i-1001].clear();

		if (i == 0) {
			dp[0].push_back({0, 0});
		} else {
			int tot_sz = 0;
			for (auto [j, c] : in_edges[i]) {
				tot_sz += int(dp[j].size());
			}
			dp[i].reserve(tot_sz);
			for (auto [j, c] : in_edges[i]) {
				for (auto p : dp[j]) {
					dp[i].push_back(p + (c == 0 ? Point(1, 0) : Point(0, 1)));
				}
			}
			std::sort(dp[i].begin(), dp[i].end(), [&](auto a, auto b) { return (a.x != b.x) ? (a.x < b.x) : (a.y < b.y); });
			int L = 0;
			for (int z = 0; z < int(dp[i].size()); z++) {
				auto p = dp[i][z];
				while (true) {
					if (L == 0) {
						dp[i][L++] = p;
						break;
					}
					assert(L >= 1);
					if (p.x == dp[i][L-1].x) {
						assert(p.y >= dp[i][L-1].y);
						break;
					}
					if (p.y >= dp[i][L-1].y) {
						break;
					}
					if (L == 1) {
						dp[i][L++] = p;
						break;
					}
					Point a = p - dp[i][L-1];
					Point b = dp[i][L-1] - dp[i][L-2];
					if (int64_t(a.x) * int64_t(b.y) - int64_t(a.y) * int64_t(b.x) >= 0) {
						L--;
						continue;
					} else {
						dp[i][L++] = p;
						break;
					}
					assert(false);
				}
			}
			dp[i].resize(L);
		}

		// answer queries
		for (auto [p, q] : queries[i]) {
			int best = int(2e9);
			for (auto v : dp[i]) {
				best = std::min(best, dot(v, p));
			}
			ans[q] = best;
		}
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}


	return 0;
}
