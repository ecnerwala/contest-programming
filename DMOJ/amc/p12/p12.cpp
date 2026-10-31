#include <bits/stdc++.h>

template <typename T> int sz(T&& x) { return int(std::size(x)); }

template <class T> int sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x_=0, T y_=0) : x(x_), y(y_) {}
	bool operator<(P p) const { return std::tie(x,y) < std::tie(p.x,p.y); }
	bool operator==(P p) const { return std::tie(x,y)==std::tie(p.x,p.y); }
	P operator+(P p) const { return P(x+p.x, y+p.y); }
	P operator-(P p) const { return P(x-p.x, y-p.y); }
	P operator*(T d) const { return P(x*d, y*d); }
	P operator/(T d) const { return P(x/d, y/d); }
	T dot(P p) const { return x*p.x + y*p.y; }
	T cross(P p) const { return x*p.y - y*p.x; }
	T cross(P a, P b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	double dist() const { return std::sqrt((double)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	double angle() const { return std::atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(double a) const {
		return P(x*std::cos(a)-y*std::sin(a),x*std::sin(a)+y*std::cos(a)); }
	friend std::ostream& operator<<(std::ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};

template<class P>
int sideOf(P s, P e, P p) { return sgn(s.cross(e, p)); }

template<class P>
int sideOf(const P& s, const P& e, const P& p, double eps) {
	auto a = (e-s).cross(p-s);
	double l = (e-s).dist()*eps;
	return (a > l) - (a < -l);
}

#define rep(i, a, b) for (int i = (a); i < (b); ++i)
#define all(x) begin(x), end(x)

typedef Point<double> P;
double rat(P a, P b) { return sgn(b.x) ? a.x/b.x : a.y/b.y; }
double polyUnion(std::vector<std::vector<P>>& poly) {
	double ret = 0;
	rep(i,0,sz(poly)) rep(v,0,sz(poly[i])) {
		P A = poly[i][v], B = poly[i][(v + 1) % sz(poly[i])];
		std::vector<std::pair<double, int>> segs = {{0, 0}, {1, 0}};
		rep(j,0,sz(poly)) if (i != j) {
			rep(u,0,sz(poly[j])) {
				P C = poly[j][u], D = poly[j][(u + 1) % sz(poly[j])];
				int sc = sideOf(A, B, C), sd = sideOf(A, B, D);
				if (sc != sd) {
					double sa = C.cross(D, A), sb = C.cross(D, B);
					if (std::min(sc, sd) < 0)
						segs.emplace_back(sa / (sa - sb), sgn(sc - sd));
				} else if (!sc && !sd && j<i && sgn((B-A).dot(D-C))>0){
					segs.emplace_back(rat(C - A, B - A), 1);
					segs.emplace_back(rat(D - A, B - A), -1);
				}
			}
		}
		std::sort(all(segs));
		for (auto& s : segs) s.first = std::min(std::max(s.first, 0.0), 1.0);
		double sum = 0;
		int cnt = segs[0].second;
		rep(j,1,sz(segs)) {
			if (!cnt) sum += segs[j].first - segs[j - 1].first;
			cnt += segs[j].second;
		}
		ret += A.cross(B) * sum;
	}
	return ret / 2;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::mt19937 mt(48);
	std::uniform_real_distribution dist(-1e-8, +1e-8);
	int N; cin >> N;
	std::vector<std::vector<P>> tris(N, std::vector<P>(3));
	for (auto& a : tris) {
		for (auto& p : a) {
			cin >> p.x >> p.y;
			p.x += dist(mt);
			p.y += dist(mt);
		}
		if (a[0].cross(a[1], a[2]) < 0) std::swap(a[1], a[2]);
	}
	cout << fixed << setprecision(20) << polyUnion(tris) << '\n';

	return 0;
}
