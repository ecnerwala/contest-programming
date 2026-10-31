#include<bits/stdc++.h>
#include "geodebug.h"

#define popipopi main

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

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

typedef Point<int64_t> P;
typedef struct Quad* Q;
typedef __int128_t lll; // (can be ll if coords are < 2e4)
const P arb(LLONG_MAX,LLONG_MAX); // not equal to any other point

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
#undef DEL
#undef valid
#undef H
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
#undef ADD
}

} // namespace kactl

int popipopi() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//GD_INIT("E.html");
	//GD_LOG_TO_STDERR(false);
	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		//GD_LAYER();
		using kactl::P;
		using kactl::triangulate;

		P R; cin >> R.x >> R.y;
		P A; cin >> A.x >> A.y;
		P B; cin >> B.x >> B.y;

		int N; cin >> N;
		cerr << "Case #" << case_num << ": " << "N = " << N << '\n';

		// argmin_p (x - p_x)^2 + (y - p_y)^2
		// = argmin_p x^2 + y^2 - 2p_x * x - 2p_y * y + p_x^2 + p_y^2
		// = argmin_p - 2p_x * x - 2p_y * y + p_x^2 + p_y^2
		// = argmin_p [x, y, 1] dot [-2p_x, -2p_y, p_x^2 + p_y^2]

		std::vector<P> pts(5*N);
		for (int i = 0; i < N; i++) {
			cin >> pts[i].x >> pts[i].y;
		}
		/*
		cerr << "R = " << R << '\n';
		cerr << "A = " << A << '\n';
		cerr << "B = " << B << '\n';
		for (int i = 0; i < N; i++) {
			cerr << pts[i] << '\n';
		}
		*/

		for (int i = 0; i < N; i++) {
			pts[1*N+i] = P(2*0 - pts[i].x, pts[i].y);
			pts[2*N+i] = P(2*R.x - pts[i].x, pts[i].y);
			pts[3*N+i] = P(pts[i].x, 2*0 - pts[i].y);
			pts[4*N+i] = P(pts[i].x, 2*R.y - pts[i].y);
		}


		std::sort(pts.begin(), pts.end());
		//GD_RECT(0, 0, R.x, R.y);
		//GD_POINT(A.x, A.y, "red");
		//GD_POINT(B.x, B.y, "red");
		for (P p : pts) {
			//GD_POINT(p.x, p.y);
		}
		// corners are 0, 1, N+2, and N+3
		auto lookup = [&](P a) -> int {
			return int(std::lower_bound(pts.begin(), pts.end(), a) - pts.begin());
		};
		auto is_original = [&](P a) -> bool {
			return 0 < a.x && a.x < R.x && 0 < a.y && a.y < R.y;
		};

		auto triangles = kactl::triangulate(pts);

		assert(int(triangles.size()) % 3 == 0);
		int V = int(triangles.size()) / 3;
		cerr << V << '\n';

		std::vector<std::pair<std::array<int, 2>, std::pair<int, double>>> half_edges;
		half_edges.reserve(V*3);

		for (int i = 0; i < int(triangles.size()) / 3; i ++) {
			auto add_edge = [&](int a, int b, int c) -> void {
				if (!is_original(pts[a]) && !is_original(pts[b])) {
					return;
				}
				P pa = pts[a];
				P pb = pts[b];
				//GD_LINE((pa.x + pb.x) * 0.5, (pa.y + pb.y) * 0.5, (pa.x + pb.x) * 0.5 + (pa.y - pb.y), (pa.y + pb.y) * 0.5 + (pb.x - pa.x), "yellow");
				double d;
				if ((pts[a] - pts[c]).dot(pts[b] - pts[c]) >= 0) {
					d = (pts[a] - pts[b]).dist() / 2;
				} else {
					// d = circumradius
					assert(pts[a].cross(pts[b], pts[c]) >= 0);
					d = (pts[a] - pts[b]).dist() * (pts[b] - pts[c]).dist() * (pts[c] - pts[a]).dist() / double(pts[a].cross(pts[b], pts[c])) / 2;
				}
				half_edges.push_back({{std::min(a, b), std::max(a, b)}, {i, d}});
			};

			P pa = triangles[3*i+0];
			P pb = triangles[3*i+1];
			P pc = triangles[3*i+2];

			//GD_TRIANGLE(pa.x, pa.y, pb.x, pb.y, pc.x, pc.y);

			int a = lookup(pa);
			int b = lookup(pb);
			int c = lookup(pc);

			//cerr << i << ' ' << pa << ' ' << pb << ' ' << pc << '\n';

			add_edge(a, b, c);
			add_edge(b, c, a);
			add_edge(c, a, b);
		}

		assert(int(half_edges.size()) % 2 == 0);

		std::sort(half_edges.begin(), half_edges.end());
		std::vector<std::tuple<double, std::array<int, 2>, std::array<int, 2>>> edges; edges.reserve(half_edges.size() / 2);
		for (int z = 0; z < int(half_edges.size()); z += 2) {
			assert(half_edges[z].first == half_edges[z+1].first);
			auto [i, di] = half_edges[z].second;
			auto [j, dj] = half_edges[z+1].second;
			double d = std::max(di, dj);
			//std::cerr << i << '-' << j << ' ' << d << '\n';
			edges.push_back({d, {i, j}, half_edges[z].first});
		}

		// the square of 2 times the distance
		double ans = R.dist();

		auto get_start = [&](P q) -> int {
			assert(is_original(q));

			P closest = pts[0];
			for (P p : pts) {
				if ((p - q).dist2() < (closest - q).dist2()) closest = p;
			}
			assert(is_original(closest));

			setmin(ans, (closest - q).dist());
			//GD_POINT(closest.x, closest.y, "blue");

			for (int i = 0; i < int(triangles.size()) / 3; i ++) {
				for (int z = 0; z < 3; z++) {
					P pa = triangles[3*i+(z+0)%3];
					P pb = triangles[3*i+(z+1)%3];
					P pc = triangles[3*i+(z+2)%3];

					assert(pa.cross(pb, pc) > 0);
					if (pa == closest && pa.cross(pb, q) >= 0 && pa.cross(q, pc) >= 0) {
						//GD_TRIANGLE(pa.x, pa.y, pb.x, pb.y, pc.x, pc.y, "red");
						return i;
					}
				}
			}
			assert(false);
		};

		int iA = get_start(A);
		int iB = get_start(B);

		cerr << iA << ' ' << iB << ' ' << ans << '\n';

		std::vector<int> par(V, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a);
			b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};

		//GD_PAUSE();
		std::sort(edges.begin(), edges.end(), std::greater<>());
		for (int z = 0; get_par(iA) != get_par(iB); z++) {
			auto [d, p, e] = edges[z];
			setmin(ans, d);
			auto [i, j] = p;
			merge(i, j);
			//GD_SEGMENT(pts[e[0]].x, pts[e[0]].y, pts[e[1]].x, pts[e[1]].y, "green") << d;
		}

		cout << "Case #" << case_num << ": ";
		cout << fixed << setprecision(8) << ans << '\n';
	}

	return 0;
}
