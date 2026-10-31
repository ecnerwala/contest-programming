#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

struct UF {
	vi e;
	UF(int n) : e(n, -1) {}
	bool sameSet(int a, int b) { return find(a) == find(b); }
	int size(int x) { return -e[find(x)]; }
	int find(int x) { return e[x] < 0 ? x : e[x] = find(e[x]); }
	bool join(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) swap(a, b);
		e[a] += e[b]; e[b] = a;
		return true;
	}
};

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

typedef Point<ll> P;
ll closest(vector<P> v) {
	if (sz(v) <= 1) return LLONG_MAX;
	assert(sz(v) > 1);
	set<P> S;
	sort(all(v), [](P a, P b) { return a.y < b.y; });
	pair<ll, pair<P, P>> ret{LLONG_MAX, {P(), P()}};
	int j = 0;
	for (P p : v) {
		P d{1 + (ll)sqrt(ret.first), 0};
		while (v[j].y <= p.y - d.x) S.erase(v[j++]);
		auto lo = S.lower_bound(p - d), hi = S.upper_bound(p + d);
		for (; lo != hi; ++lo)
			ret = min(ret, {(*lo - p).dist2(), {*lo, p}});
		S.insert(p);
	}
	return ret.first;
}

typedef struct Quad* Q;
typedef __int128_t lll; // (can be ll if coords are < 2e4)
P arb(LLONG_MAX,LLONG_MAX); // not equal to any other point

struct Quad {
	Q rot, o; P p = arb; bool mark;
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
	while (H) {
		delete H->rot->rot->rot;
		delete H->rot->rot;
		delete H->rot;
		delete std::exchange(H, H->o);
	}
	return pts;
}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<kactl::P> pts(N);
	for (auto& p : pts) cin >> p.x >> p.y;
	map<kactl::P, int> idx;
	for (int i = 0; i < N; i++) idx[pts[i]] = i;

	vector<pair<int64_t, pair<int, int>>> edges;

	{
		auto triangulation = kactl::triangulate(pts);
		auto check_pts = [&](kactl::P a, kactl::P b) -> void {
			int i = idx[a], j = idx[b];
			edges.push_back({(a-b).dist2(), {i, j}});
		};
		if (!triangulation.empty()) {
			edges.reserve(triangulation.size());
			assert(int(triangulation.size()) % 3 == 0);
			for (int i = 0; i < int(triangulation.size()); i += 3) {
				check_pts(triangulation[i], triangulation[i+1]);
				check_pts(triangulation[i], triangulation[i+2]);
				check_pts(triangulation[i+1], triangulation[i+2]);
			}
		} else {
			edges.reserve(N-1);
			triangulation = pts;
			sort(triangulation.begin(), triangulation.end());
			for (int i = 0; i+1 < N; i++) {
				check_pts(triangulation[i], triangulation[i+1]);
			}
		}
	}

	kactl::UF uf(N);
	kactl::UF color_uf(2*N);
	sort(edges.begin(), edges.end());
	for (auto [_, p] : edges) {
		auto [i, j] = p;
		if (uf.join(i, j)) {
			color_uf.join(2*i, 2*j+1);
			color_uf.join(2*i+1, 2*j);
		}
	}

	vector<kactl::P> A; A.reserve(N);
	vector<kactl::P> B; B.reserve(N);
	for (int i = 0; i < N; i++) {
		if (color_uf.sameSet(2*i, 0)) A.push_back(pts[i]);
		else B.push_back(pts[i]);
	}
	int64_t ans = min(kactl::closest(A), kactl::closest(B));
	cout << ans << '\n';

	return 0;
}
