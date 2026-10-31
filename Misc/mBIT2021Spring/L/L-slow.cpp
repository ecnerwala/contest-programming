#include <bits/stdc++.h>

template <typename T>
using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int signum(double a) {
	return (a > 0) - (a < 0);
}

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

using pt_t = Point<double>;
struct circ_t {
	pt_t p;
	double r;
	double d;
};

const double PI2 = atan2(1, 0);
const double PI = acos(-1);
const double TAU = 2 * PI;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	N += 2;
	vector<circ_t> circles(N);
	for (int i = 0; i < N; i++) {
		auto& c = circles[i];
		if (i < 2) {
			cin >> c.p;
			c.r = 0, c.d = 0;
		} else {
			cin >> c.p >> c.r >> c.d;
		}
	}

	// sa == sb is external, else internal
	auto get_tangent = [](circ_t a, circ_t b, int sa, int sb) -> array<double, 2> {
		assert(sa == -1 || sa == 1);
		assert(sb == -1 || sb == 1);
		double d = abs(b.p - a.p);
		double theta = arg(b.p - a.p) + asin((b.r * sb - a.r * sa) / d);
		if (theta <= -PI) theta += TAU;
		if (theta > PI) theta -= TAU;

		double ta = theta + PI2 * sa;
		if (ta <= -PI) ta += TAU;
		if (ta > PI) ta -= TAU;

		double tb = theta + PI2 * sb;
		if (tb <= -PI) tb += TAU;
		if (tb > PI) tb -= TAU;

		return {ta, tb};
	};
	auto get_pt = [](circ_t a, double theta) -> pt_t {
		return a.p + pt_t(std::polar(a.r, theta));
	};

	int V = 2;
	vector<vector<pair<double, int>>> angles(N);
	vector<vector<pair<int, double>>> adj(V); adj.reserve(8*N*N);

	auto get_vtx = [&](int i, double d) -> int {
		if (i < 2) return i;
		int resp = V++;
		adj.emplace_back();
		angles[i].push_back({d, resp});
		return resp;
	};

	for (int i = 0; i < N; i++) {
		for (int si : {-1, 1}) {
			for (int j = i+1; j < N; j++) {
			auto ci = circles[i];
			auto cj = circles[j];
				if (i < 2 && si == 1) continue;
				for (int sj : {-1, 1}) {
					if (j < 2 && sj == 1) continue;

					auto [di, dj] = get_tangent(ci, cj, si, sj);

					auto vi = get_vtx(i, di);
					auto vj = get_vtx(j, dj);

					pt_t pi = get_pt(ci, di);
					pt_t pj = get_pt(cj, dj);
					pt_t dir = pj - pi;
					double dir_len = abs(dir);
					double xi = dot(dir, pi);
					double xj = dot(dir, pj);
					auto intersects = [&](circ_t c) -> bool {
						if (abs(c.p - pi) < c.r) return true;
						if (abs(c.p - pj) < c.r) return true;
						double xc = dot(dir, c.p);
						if (signum(xc - xi) * signum(xc - xj) >= 0) return false;
						return abs(cross(dir, c.p - pi)) / dir_len < c.r;
					};
					double cost = 0;
					for (int k = 2; k < N; k++) {
						if (k == i || k == j) continue;
						if (intersects(circles[k])) cost += circles[k].d;
					}

					cerr << i << ',' << vi << ' ' << j << ',' << vj << ' ' << cost << ' ' << abs(dir) << '\n';
					cost += abs(dir);
					adj[vi].push_back({vj, cost});
					adj[vj].push_back({vi, cost});
				}
			}
		}
	}

	for (int k = 2; k < N; k++) {
		sort(angles[k].begin(), angles[k].end());
		for (int i = 0; i < int(angles[k].size()); i++) {
			auto [da, va] = angles[k][i];
			auto [db, vb] = angles[k][(i+1) % int(angles[k].size())];
			if (i + 1 == int(angles[k].size())) db += TAU;
			double d = min(db - da, TAU - (db - da)) * circles[k].r;
			adj[va].push_back({vb, d});
			adj[vb].push_back({va, d});
		}
	}

	const double INF = 1e99;
	vector<double> dist(V, INF);
	vector<bool> vis(V);
	min_pq<pair<double, int>> pq;
	pq.push({dist[0] = 0., 0});

	while (true) {
		auto [d, cur] = pq.top(); pq.pop();
		if (vis[cur]) continue;
		vis[cur] = true;
		if (cur == 1) break;
		for (auto [nxt, l] : adj[cur]) {
			l += d;
			if (l < dist[nxt]) {
				pq.push({dist[nxt] = l, nxt});
			}
		}
	}

	cout << fixed << setprecision(10) << dist[1] << '\n';

	return 0;
}
