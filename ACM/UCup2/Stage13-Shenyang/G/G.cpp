#include <bits/stdc++.h>

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
	explicit Point(T x_=0, T y_=0) : x(x_), y(y_) {}
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

namespace delaunay {
typedef Point<ll> P;
typedef struct Quad* Q;
typedef __int128_t lll; // (can be ll if coords are < 2e4)
P arb(LLONG_MAX,LLONG_MAX); // not equal to any other point

struct Quad {
	Q rot = nullptr, o = nullptr; P p = arb; bool mark = false;
	P& F() { return r()->p; }
	Q& r() { return rot->rot; }
	Q prev() { return rot->o->rot; }
	Q next() { return r()->prev(); }
} *H;

bool circ(P p, P a, P b, P c) { // is p in the circumcircle?
	lll p2 = p.dist2(), A = a.dist2()-p2,
	    B = b.dist2()-p2, C = c.dist2()-p2;
	return p.cross(a,b)*C + p.cross(b,c)*A + p.cross(c,a)*B > 0;
}
Q makeEdge(P orig, P dest) {
	Q r = H ? H : new Quad{new Quad{new Quad{new Quad{0}}}};
	H = r->o; r->r()->r() = r;
	rep(i,0,4) r = r->rot, r->p = arb, r->o = i & 1 ? r : r->r();
	r->p = orig; r->F() = dest;
	return r;
}
void splice(Q a, Q b) {
	swap(a->o->rot->o, b->o->rot->o); swap(a->o, b->o);
}
Q connect(Q a, Q b) {
	Q q = makeEdge(a->F(), b->p);
	splice(q, a->next());
	splice(q->r(), b);
	return q;
}

pair<Q,Q> rec(const vector<P>& s) {
	if (sz(s) <= 3) {
		Q a = makeEdge(s[0], s[1]), b = makeEdge(s[1], s.back());
		if (sz(s) == 2) return { a, a->r() };
		splice(a->r(), b);
		auto side = s[0].cross(s[1], s[2]);
		Q c = side ? connect(b, a) : 0;
		return {side < 0 ? c->r() : a, side < 0 ? c : b->r() };
	}

#define H(e) e->F(), e->p
#define valid(e) (e->F().cross(H(base)) > 0)
	Q A, B, ra, rb;
	int half = sz(s) / 2;
	tie(ra, A) = rec({all(s) - half});
	tie(B, rb) = rec({sz(s) - half + all(s)});
	while ((B->p.cross(H(A)) < 0 && (A = A->next())) ||
	       (A->p.cross(H(B)) > 0 && (B = B->r()->o)));
	Q base = connect(B->r(), A);
	if (A->p == ra->p) ra = base->r();
	if (B->p == rb->p) rb = base;

#define DEL(e, init, dir) Q e = init->dir; if (valid(e)) \
		while (circ(e->dir->F(), H(base), e->F())) { \
			Q t = e->dir; \
			splice(e, e->prev()); \
			splice(e->r(), e->r()->prev()); \
			e->o = H; H = e; e = t; \
		}
	for (;;) {
		DEL(LC, base->r(), o);  DEL(RC, base, prev());
		if (!valid(LC) && !valid(RC)) break;
		if (!valid(LC) || (valid(RC) && circ(H(RC), H(LC))))
			base = connect(RC, base->r());
		else
			base = connect(base->r(), LC->r());
	}
	return { ra, rb };
}

vector<P> triangulate(vector<P> pts) {
	sort(all(pts));  assert(unique(all(pts)) == pts.end());
	if (sz(pts) < 2) return {};
	Q e = rec(pts).first;
	vector<Q> q = {e};
	int qi = 0;
	while (e->o->F().cross(e->F(), e->p) < 0) e = e->o;
#define ADD { Q c = e; do { c->mark = 1; pts.push_back(c->p); \
	q.push_back(c->r()); c = c->next(); } while (c != e); }
	ADD; pts.clear();
	while (qi < sz(q)) if (!(e = q[qi++])->mark) ADD;
	return pts;
}
}

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
#define ang(a) (a[1] - a[0]).angle()

int angDiff(Line a, Line b) { return sgn(ang(a) - ang(b)); }
bool cmp(Line a, Line b) {
	int s = angDiff(a, b);
	return (s ? s : sideOf(sp(a), b[0])) < 0;
}
vector<P> halfPlaneIntersection(vector<Line> vs) {
	const double EPS = sqrt(2) * 1e-8;
	sort(all(vs), cmp);
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

}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using pt_t = kactl::P;
	pt_t R_LO, R_HI;
	cin >> R_LO.x >> R_LO.y >> R_HI.x >> R_HI.y;
	int N; cin >> N;
	std::vector<pt_t> P(N);
	for (auto& p : P) cin >> p.x >> p.y;

	pt_t A0 = R_LO, A1 = pt_t(R_HI.x, R_LO.y), A2 = R_HI, A3 = pt_t(R_LO.x, R_HI.y);
	std::vector<kactl::Line> rect_planes({
		{A0 * 2, A1 * 2},
		{A1 * 2, A2 * 2},
		{A2 * 2, A3 * 2},
		{A3 * 2, A0 * 2},
	});

	double tot_area = 2 * (R_HI.x - R_LO.x) * 2 * (R_HI.y - R_LO.y);
	cerr << "Tot area = " << tot_area << '\n';

	auto get_area_val = [&](std::vector<kactl::P> region, pt_t p) -> double {
		cerr << "Querying area " << p << '\n';
		for (auto a : region) { cerr << a << ' '; } cerr << '\n';
		// take the integral over the region of int p.norm - 2 * (p dot v) dv
		double res = 0;
		for (int z = 0; z < 2; z++) {
			cerr << "z = " << z << '\n';
			double area = 0;
			for (int i = 0; i < int(region.size()); i++) {
				pt_t a = region[i];
				pt_t b = region[(i+1) % int(region.size())];
				res += (p.x * p.x) * (a.y + b.y) / 2 * (b.x - a.x);
				area += (a.y + b.y) / 2 * (b.x - a.x);
				// integral of x * ((a.y * b.x - b.y * a.x + (b.y - a.y) * x) / (b.x - a.x))
				// integral of x * (a.y * b.x - b.y * a.x) / (b.x - a.x) + integral of x^2 * (b.y - a.y) / (b.x - a.x)
				double int_x = (a.y * b.x - b.y * a.x) * (b.x - a.x) / 2 + (b.x - a.x) * (b.x - a.x) / 3 * (b.y - a.y);
				res -= 2 * p.x * int_x;
			}
			cerr << "area = " << -area << '\n';
			cerr << -area * p.x * p.x << '\n';
			std::reverse(region.begin(), region.end());
			std::swap(p.x, p.y);
			for (auto& a : region) { std::swap(a.x, a.y); }
		}
		cerr << "res = " << -res << '\n';
		cerr << '\n';
		return -res;
	};

	double ans = 0;
	for (int i = 0; i < N; i++) {
		std::vector<kactl::Line> close_planes;
		close_planes.reserve(N+4);
		close_planes.insert(close_planes.end(), rect_planes.begin(), rect_planes.end());

		std::vector<kactl::Line> far_planes;
		far_planes.reserve(N+4);
		far_planes.insert(far_planes.end(), rect_planes.begin(), rect_planes.end());

		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			pt_t mid = P[i] + P[j];
			pt_t perp = (P[j] - P[i]).perp();
			close_planes.push_back({mid - perp, mid + perp});
			far_planes.push_back({mid + perp, mid - perp});
		}
		ans += get_area_val(kactl::halfPlaneIntersection(far_planes), P[i]);
		ans -= get_area_val(kactl::halfPlaneIntersection(close_planes), P[i]);
	}

	if constexpr (false) {
		std::vector<pt_t> P_extra; P_extra.reserve(N+4);
		P_extra.insert(P_extra.end(), P.begin(), P.end());
		P_extra.push_back(pt_t{-int(4e4), -int(4e4)});
		P_extra.push_back(pt_t{-int(4e4), +int(4e4)});
		P_extra.push_back(pt_t{+int(4e4), -int(4e4)});
		P_extra.push_back(pt_t{+int(4e4), +int(4e4)});

		std::map<pt_t, int> pt_id;
		for (int i = 0; i < int(P_extra.size()); i++) {
			pt_id[P_extra[i]] = i;
		}
	}

	ans /= tot_area;
	ans *= M_PI;
	cout << fixed << setprecision(18) << ans << '\n';

	return 0;
}
