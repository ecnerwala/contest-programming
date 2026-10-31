#include <bits/stdc++.h>

namespace kactl {

using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef int64_t ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

template <class T> int sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x=0, T y=0) : x(x), y(y) {}
	bool operator<(P p) const { return tie(x,y) < tie(p.x,p.y); }
	bool operator==(P p) const { return tie(x,y)==tie(p.x,p.y); }
	P operator+(P p) const { return P(x+p.x, y+p.y); }
	P operator-(P p) const { return P(x-p.x, y-p.y); }
	P operator*(T d) const { return P(x*d, y*d); }
	P operator/(T d) const { return P(x/d, y/d); }
	T dot(P p) const { return x*p.x + y*p.y; }
	T cross(P p) const { return x*p.y - y*p.x; }
	T cross(P a, P b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	double dist() const { return sqrt((double)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	double angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(double a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
	friend ostream& operator<<(ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};

template<class P>
int lineIntersection(const P& s1, const P& e1, const P& s2,
		const P& e2, P& r) {
	if ((e1-s1).cross(e2-s2)) { //if not parallell
		r = s2-(e2-s2)*(e1-s1).cross(s2-s1)/(e1-s1).cross(e2-s2);
		return 1;
	} else
		return -((e1-s1).cross(s2-s1)==0 || s2==e2);
}

#define eps 1e-8
typedef Point<double> P;

struct Line {
	P P1, P2;
	// Right hand side of the ray P1 -> P2
	explicit Line(P a = P(), P b = P()) : P1(a), P2(b) {};
	P intpo(Line y) {
		P r;
		assert(lineIntersection(P1, P2, y.P1, y.P2, r) == 1);
		return r;
	}
	P dir() {
		return P2 - P1;
	}
	bool contains(P x) {
		return (P2 - P1).cross(x - P1) < eps;
	}
	bool out(P x) {
		return !contains(x);
	}
};

template<class T>
bool mycmp(Point<T> a, Point<T> b) {
	// return atan2(a.y, a.x) < atan2(b.y, b.x);
	if (a.x * b.x < 0)	return a.x < 0;
	if (abs(a.x) < eps) {
		if (abs(b.x) < eps) 	return a.y > 0 && b.y < 0;
		if (b.x < 0)	return a.y > 0;
		if (b.x > 0)	return true;
	}
	if (abs(b.x) < eps) {
		if (a.x < 0)	return b.y < 0;
		if (a.x > 0)	return false;
	}
	return a.cross(b) > 0;
}

bool cmp(Line a, Line b) {
	return mycmp(a.dir(), b.dir());
}

std::optional<std::vector<P>> Intersection(vector <Line> b) {
	sort(b.begin(), b.end(), cmp);
	int n = int(b.size());
	int q = 1, h = 0, i;
	vector <Line> c(b.size() + 10);
	for (i = 0; i < n; i++) {
		while (q < h && b[i].out(c[h].intpo(c[h - 1])))	h--;
		while (q < h && b[i].out(c[q].intpo(c[q + 1])))	q++;
		c[++h] = b[i];
		if (q < h && abs(c[h].dir().cross(c[h - 1].dir())) < eps) {
			if (c[h].dir().dot(c[h - 1].dir()) > 0) {
				h--;
				if (b[i].out(c[h].P1))	c[h] = b[i];
			}else {
				// The area is either 0 or infinite.
				// If you have a bounding box, then the area is definitely 0.
				return std::nullopt;
			}
		}
	}
	while (q < h - 1 && c[q].out(c[h].intpo(c[h - 1])))	h--;
	while (q < h - 1 && c[h].out(c[q].intpo(c[q + 1])))	q++;
	// Intersection is empty. This is sometimes different from the case when
	// the intersection area is 0.
	if (h - q <= 1)	return std::nullopt;
	c[h + 1] = c[q];
	vector <P> s;
	for (i = q; i <= h; i++)	s.push_back(c[i].intpo(c[i + 1]));
	return s;
}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using kactl::P;
	using kactl::Line;
	int N, Q; cin >> N >> Q;
	std::vector<P> points(N);
	std::vector<P> queries(Q);
	for (auto& p : points) { cin >> p.x >> p.y; }
	for (auto& p : queries) { cin >> p.x >> p.y; }

	std::sort(points.begin(), points.end());

	auto center = std::accumulate(points.begin(), points.end(), P(0,0));
	center.x /= N;
	center.y /= N;

	for (auto& p : points) p = p - center;

	std::vector<Line> lines; lines.reserve(2*N);
	for (int i = 0; i < N; i++) {
		bool found = false;
		Line max_dot; double max_dot_v;
		Line min_dot; double min_dot_v;
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			Line l{points[i], points[j]};
			if (!l.contains(P(0,0))) std::swap(l.P1, l.P2);
			double v = (l.P2 - l.P1).dot(points[i]) / (l.P2 - l.P1).dist();
			if (!found) {
				max_dot = min_dot = l;
				max_dot_v = min_dot_v = v;
				found = true;
			} else {
				if (v > max_dot_v) {
					max_dot = l;
					max_dot_v = v;
				}
				if (v < min_dot_v) {
					min_dot = l;
					min_dot_v = v;
				}
			}
		}
		if (found) {
			lines.push_back(max_dot);
			lines.push_back(min_dot);
		}
	}

	for (auto q_x : queries) {
		double ans = 1e99;
		for (P q : {q_x.perp(), q_x.perp() * -1}) {
			for (auto l : lines) {
				P r;
				if (lineIntersection(P(0,0), q, l.P1, l.P2, r) == 1 && q.dot(r) >= 0) {
					ans = min(ans, r.dist());
				}
			}
		}
		ans *= N;
		cout << fixed << setprecision(10) << ans << '\n';
	}
	/*
	auto maybe_hull = kactl::Intersection(lines);
	if (!maybe_hull || maybe_hull->empty()) {
		for (auto _ : queries) {
			cout << 0 << '\n';
		}
		exit(0);
	}
	auto hull = *maybe_hull;
	for (auto p : hull) {
		cerr << p + center << '\n';
	}
	for (auto q_x : queries) {
		double ans = 1e99;
		for (P q : {q_x.perp(), q_x.perp() * -1}) {
			for (int i = 0; i < int(hull.size()); i++) {
				int j = (i+1) % int(hull.size());
				P r;
				if (lineIntersection(P(0,0), q, hull[i], hull[j], r) == 1 && q.dot(r) >= 0) {
					ans = min(ans, r.dist());
				}
			}
		}
		ans *= N;
		cout << fixed << setprecision(10) << ans << '\n';
	}
	*/

	return 0;
}
