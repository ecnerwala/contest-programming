#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

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
	int idx;
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
	assert(std::is_sorted(pts.begin(), pts.end()));
	//sort(all(pts));
	assert(unique(all(pts)) == pts.end());
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
#undef ADD
}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using pt_t = kactl::P;
	int N; cin >> N;
	std::vector<pt_t> pts(N);
	for (int i = 0; i < N; i++) {
		cin >> pts[i].x >> pts[i].y;
		pts[i].idx = i;
	}
	std::sort(pts.begin(), pts.end());

	std::string ans(N, '0');

	const int64_t INF = 8e18;
	std::vector<int64_t> min_dist(N);
	while (pts.size() >= 2) {
		assert(std::is_sorted(pts.begin(), pts.end()));
		auto tris = kactl::triangulate(pts);
		for (auto& p : pts) {
			min_dist[p.idx] = INF;
		}
		auto do_pair = [&](pt_t i, pt_t j) {
			int64_t v = (i-j).dist2();
			setmin(min_dist[i.idx], v);
			setmin(min_dist[j.idx], v);
		};

		if (tris.empty()) {
			// we're in the linear case
			// lex sort is fine
			for (int z = 0; z+1 < int(pts.size()); z++) {
				do_pair(pts[z+0], pts[z+1]);
			}
		} else {
			assert(int(tris.size()) % 3 == 0);
			for (int z = 0; z < int(tris.size()); z += 3) {
				do_pair(tris[z+0], tris[z+1]);
				do_pair(tris[z+1], tris[z+2]);
				do_pair(tris[z+2], tris[z+0]);
			}
		}

		auto check_pair = [&](pt_t i, pt_t j) {
			int64_t v = (i-j).dist2();
			if (min_dist[i.idx] == v && min_dist[j.idx] == v) {
				ans[i.idx] = ans[j.idx] = '1';
			}
		};

		if (tris.empty()) {
			// we're in the linear case
			// lex sort is fine
			for (int z = 0; z+1 < int(pts.size()); z++) {
				check_pair(pts[z+0], pts[z+1]);
			}
		} else {
			assert(int(tris.size()) % 3 == 0);
			for (int z = 0; z < int(tris.size()); z += 3) {
				check_pair(tris[z+0], tris[z+1]);
				check_pair(tris[z+1], tris[z+2]);
				check_pair(tris[z+2], tris[z+0]);
			}
		}

		{
			int l = 0;
			for (int i = 0; i < int(pts.size()); i++) {
				if (ans[pts[i].idx] == '0') {
					pts[l++] = pts[i];
				}
			}
			pts.resize(l);
		}
	}

	cout << ans << '\n';

	return 0;
}
