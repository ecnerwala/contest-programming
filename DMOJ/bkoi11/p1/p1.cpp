#include<bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

template <class T> int sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
struct Point {
	typedef Point P;
	T x, y;
	Point() : x(0), y(0) {}
	Point(T x_, T y_) : x(x_), y(y_) {}
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
int sideOf(P s, P e, P p) { return sgn(s.cross(e, p)); }

template<class P>
int sideOf(const P& s, const P& e, const P& p, double eps) {
	auto a = (e-s).cross(p-s);
	double l = (e-s).dist()*eps;
	return (a > l) - (a < -l);
}

template<class P>
pair<int, P> lineInter(P s1, P e1, P s2, P e2) {
	auto d = (e1 - s1).cross(e2 - s2);
	if (d == 0) // if parallel
		return {-(s1.cross(e1, s2) == 0), P(0, 0)};
	auto p = s2.cross(e1, e2), q = s2.cross(e2, s1);
	return {1, (s1 * p + e1 * q) / d};
}

typedef Point<double> P;
typedef array<P, 2> Line;
#define sp(a) a[0], a[1]
#define ang(a) atan2((a[1] - a[0]).y, (a[1] - a[0]).x)

int angDiff(Line a, Line b) { return sgn(ang(a) - ang(b)); }
bool cmp(Line a, Line b) {
	auto s = angDiff(a, b);
	return s == 0 ? sideOf(sp(b), a[0]) >= 0 : s < 0;
}
vector<P> halfPlaneIntersection(vector<Line> vs) {
	const double EPS = sqrt(2) * 1e-8;
	//sort(all(vs), cmp);
	vector<Line> deq(sz(vs) + 5);
	vector<P> ans(sz(vs) + 5);
	deq[0] = vs[0];
	int ah = 0, at = 0, n = sz(vs);
	rep(i,1,n+1) {
		if (i == n) vs.push_back(deq[ah]);
		if (angDiff(vs[i], vs[i - 1]) == 0) continue;
		while (ah<at && sideOf(sp(vs[i]), ans[at-1], EPS) < 0)
			at--;
		while (i!=n && ah<at && sideOf(sp(vs[i]),ans[ah],EPS)<0)
			ah++;
		auto res = lineInter(sp(vs[i]), sp(deq[at]));
		if (res.first != 1) continue;
		ans[at++] = res.second, deq[at] = vs[i];
	}
	if (at - ah <= 2) return {};
	return {ans.begin() + ah, ans.begin() + at};
}

pair<double, array<P, 2>> hullDiameter(vector<P> S) {
	int n = sz(S), j = n < 2 ? 0 : 1;
	pair<double, array<P, 2>> res({0, {S[0], S[0]}});
	rep(i,0,j)
		for (;; j = (j + 1) % n) {
			res = max(res, {(S[i] - S[j]).dist2(), {S[i], S[j]}});
			if ((S[(j + 1) % n] - S[j]).cross(S[i + 1] - S[i]) >= 0)
				break;
		}
	return {sqrt(res.first), res.second};
}
} // namespace kactl

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	using namespace kactl;
	int N; cin >> N;
	vector<P> pts(N);
	for (auto& p : pts) {
		int x, y; cin >> x >> y;
		p = P(x, y);
	}

	vector<Line> lines(N);
	auto is_good = [&](double R) -> bool {
		for (int i = 0; i < N; i++) {
			P a = pts[i], b = pts[(i+1)%N];
			P diff = (b-a).perp().unit() * R;
			lines[i] = {a + diff, b + diff};
		}
		auto inter = halfPlaneIntersection(lines);
		if (inter.empty()) return false;
		return hullDiameter(inter).first >= 2 * R;
	};

	double mi = 0;
	double ma = hullDiameter(pts).first / 2;
	for (int z = 0; z < 60; z++) {
		double md = (mi + ma) / 2;
		if (is_good(md)) mi = md;
		else ma = md;
	}

	cout << fixed << setprecision(10) << mi << '\n';
	return 0;
}
