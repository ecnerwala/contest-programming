#include <bits/stdc++.h>

void wssert(bool b) {
	if (!b) {
		std::cout << "GIMME WA" << std::endl;
		std::exit(1);
	}
}

#define ll long long
#define sz(x) ((int) (x).size())
#define all(x) (x).begin(), (x).end()
#define vi vector<int>
#define pii pair<int, int>
#define rep(i, a, b) for(int i = (a); i < (b); i++)

#define trav(a, x) for(auto& a : x)

std::ostream& operator << (std::ostream& os, __int128_t v) {
	bool neg = (v < 0);
	if (neg) v = -v;
	std::string r;
	while (v) {
		r += char('0' + (v % 10));
		v /= 10;
	}
	std::reverse(r.begin(), r.end());
	if (r.empty()) r += '0';
	if (neg) os << '-';
	os << r;
	return os;
}

using namespace std;
template<typename T>
using minpq = priority_queue<T, vector<T>, greater<T>>;

using ftype = long double;
using itype = __int128_t;
//typedef long double ftype;

struct pt3 {
    itype x, y, z;
	//int idx = -1;
    pt3(itype x_ = 0, itype y_ = 0, itype z_ = 0) : x(x_), y(y_), z(z_) {}
    pt3 operator-(const pt3 &o) const {
        return pt3(x - o.x, y - o.y, z - o.z);
    }
    pt3 cross(const pt3 &o) const {
        return pt3(y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x);
    }
    itype dot(const pt3 &o) const {
        return x * o.x + y * o.y + z * o.z;
    }
	bool nonzero() const {
		return x != 0 || y != 0 || z != 0;
	}
	/*
    itype abs() const {
        return sqrt(dot(*this));
    }
	*/
};

struct edge;

// The implementation becomes more challenging because we need information of adjacent faces.
// A face will have 3 edges for its adjacent faces
// e1 corresponds to the edge (a,b), e2 to (b,c), and e3 to (c,a)
// A face will store a list of future points that can see it.
// A face will also store "dead" - the index of the point responsible for deleting it (or 1e9 if alive)
struct face {
    int a, b, c;
    pt3 q;
    edge *e1, *e2, *e3;
    vi points;
    int dead = 1e9;
    face(int a_, int b_, int c_, pt3 q_) : a(a_), b(b_), c(c_), q(q_) {
        e1 = e2 = e3 = NULL;
    }
};

// an edge will store the face it leads to and a pointer to the reverse edge
struct edge {
    edge *rev;
    face *f;
};

// This function will glue two faces together
// e1 is a reference to the F1 edge pointer, and e2 is a reference to the F2 edge pointer
void glue(face *F1, face *F2, edge* &e1, edge* &e2) {
    e1 = new edge;
    e2 = new edge;
    e1->rev = e2;
    e2->rev = e1;
    e1->f = F2;
    e2->f = F1;
};

// modify this to your liking
//const ftype EPS = 1e-9;
const itype EPS = 0;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// shuffles the point-set p, making sure the first 4 points are not coplanar.
// if all points are coplanar, an assertion fails
void prepare(vector<pt3> &p) {
    int n = sz(p);
    shuffle(all(p), rng);
    vi ve;
    ve.push_back(0);
    rep(i, 1, n) {
        if(sz(ve) == 1) {
            if((p[ve[0]] - p[i]).nonzero()) ve.push_back(i);
        }else if(sz(ve) == 2) {
            if((p[ve[1]] - p[ve[0]]).cross(p[i] - p[ve[0]]).nonzero()) ve.push_back(i);
        }else if((p[i] - p[ve[0]]).dot((p[ve[1]] - p[ve[0]]).cross(p[ve[2]] - p[ve[0]])) != 0) {
            ve.push_back(i);
            break;
        }
    }
    wssert(sz(ve) == 4);
    vector<pt3> ve2;
    for(int i : ve) ve2.push_back(p[i]);
    reverse(all(ve));
    for(int i : ve) p.erase(p.begin() + i);
    p.insert(p.begin(), all(ve2));
}

vector<face*> hull3(vector<pt3> &p) {
    int n = sz(p);
    prepare(p);
    vector<face*> f, new_face(n, NULL);

    // for a point i, conflict[i] is the list of faces it can see.
    // It might contain faces that were deleted, and we should ignore them
    vector<vector<face*>> conflict(n);
    auto add_face = [&](int a, int b, int c) {
        face *F = new face(a, b, c, (p[b] - p[a]).cross(p[c] - p[a]));
        f.push_back(F);
        return F;
    };

    // initialize a triangular disk of the first 3 points.
    // The initial tetrahedron is handled automatically when we insert the 4th point
    face *F1 = add_face(0, 1, 2);
    face *F2 = add_face(0, 2, 1);
    glue(F1, F2, F1->e1, F2->e3);
    glue(F1, F2, F1->e2, F2->e2);
    glue(F1, F2, F1->e3, F2->e1);
    rep(i, 3, n) {
        for(face *F : {F1, F2}) {
            itype Q = (p[i] - p[F->a]).dot(F->q);
            if(Q > EPS) conflict[i].push_back(F);
            // making this second check is an ugly consequence of starting with a degenerate triangular disk.
            // We want to make sure each future point is considered visible to some initial face.
            if(Q >= -EPS) F->points.push_back(i);
        }
    }

    rep(i, 3, n) {
        // mark all visible faces as dead
        for(face *F : conflict[i]) F->dead = min(F->dead, i);

        // If a dead face and alive face are adjacent, we have an exposed edge
        // Vertex v will be a vertex on some exposed edge
        int v = -1;
        for(face *F : conflict[i]) {
            if(F->dead != i) continue;
            int parr[3] = {F->a, F->b, F->c};
            edge* earr[3] = {F->e1, F->e2, F->e3};
            rep(j, 0, 3) {
                int j2 = (j + 1);
                if(j2 >= 3) j2 -= 3;

                if(earr[j]->f->dead > i) {
                    // F is dead and earr[j]->f is alive.
                    // We should add a new face Fn, attach it to earr[j]->f,
                    // combine the point lists of the two faces into Fn,
                    // and store Fn in new_face[parr[j]] so we can glue all the new faces together in a cone.
                    face *Fn = new_face[parr[j]] = add_face(parr[j], parr[j2], i);
                    set_union(all(F->points), all(earr[j]->f->points), back_inserter(Fn->points));
                    Fn->points.erase(stable_partition(all(Fn->points), [&](int k) {
                        return k > i && (p[k] - p[Fn->a]).dot(Fn->q) > EPS;
                    }), Fn->points.end());
                    for(int k : Fn->points) {
                        conflict[k].push_back(Fn);
                    }
                    earr[j]->rev->f = Fn;
                    Fn->e1 = earr[j];
                    v = parr[j];
                }
            }
        }
        // There are no exposed edges
        if(v == -1) continue;

        // Glue all the new cone faces together
        while(new_face[v]->e2 == NULL) {
            int u = new_face[v]->b;
            glue(new_face[v], new_face[u], new_face[v]->e2, new_face[u]->e3);
            v = u;
        }
    }

    // Remove dead faces
    f.erase(remove_if(all(f), [&](face *F) {
        return F->dead < n;
    }), f.end());
    return f;
}

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
	long double dist() const { return std::sqrt((long double)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	//ftype angle() const { return atan2(y, x); }
	//P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	//P rotate(ftype a) const { return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
	friend ostream& operator<<(ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
};

using P3 = pt3;

// typedef Point3D<ftype> P3;

struct PR {
	void ins(int x) { (a == -1 ? a : b) = x; }
	void rem(int x) { (a == x ? a : b) = -1; }
	int cnt() { return (a != -1) + (b != -1); }
	int a, b;
};

struct F { P3 q; int a, b, c; };

vector<F> hull3d(const vector<P3>& A) {
	wssert(sz(A) >= 4);
	vector<vector<PR>> E(sz(A), vector<PR>(sz(A), {-1, -1}));
#define E(x,y) E[f.x][f.y]
	vector<F> FS;
	auto mf = [&](int i, int j, int k, int l) {
		P3 q = (A[j] - A[i]).cross((A[k] - A[i]));
		if (q.dot(A[l]) > q.dot(A[i]))
			q = P3()-q;
		F f{q, i, j, k};
		E(a,b).ins(k); E(a,c).ins(j); E(b,c).ins(i);
		FS.push_back(f);
	};
	rep(i,0,4) rep(j,i+1,4) rep(k,j+1,4)
		mf(i, j, k, 6 - i - j - k);

	rep(i,4,sz(A)) {
		rep(j,0,sz(FS)) {
			F f = FS[j];
			if(f.q.dot(A[i]) > f.q.dot(A[f.a])) {
				E(a,b).rem(f.c);
				E(a,c).rem(f.b);
				E(b,c).rem(f.a);
				swap(FS[j--], FS.back());
				FS.pop_back();
			}
		}
		int nw = sz(FS);
		rep(j,0,nw) {
			F f = FS[j];
#define C(a, b, c) if (E(a,b).cnt() != 2) mf(f.a, f.b, i, f.c);
			C(a, b, c); C(a, c, b); C(b, c, a);
		}
	}
	trav(it, FS) if ((A[it.b] - A[it.a]).cross(
		A[it.c] - A[it.a]).dot(it.q) <= 0) swap(it.c, it.b);
	return FS;
};

template<class P, class F>
void delaunay(vector<P> ps, F trifun, vector<itype> R) {
	vector<P3> p3;
	for(int i = 0; i < (int)ps.size(); i++){
		P p = ps[i];
		p3.emplace_back(p.x, p.y, p.dist2() - R[i] * R[i]);
		//p3.back().idx = i;
	}
	if (sz(ps) == 3) {
		int d = (ps[0].cross(ps[1], ps[2]) < 0);
		int a = 0, c = 1+d, b = 2-d;
		P3 dir = (p3[b]-p3[a]).cross(p3[c]-p3[a]);
		wssert(dir.dot(P3(0,0,1)) < 0);
		trifun(p3[a], p3[c], p3[b], dir);
	}
	if (sz(ps) > 3) for(auto t:hull3(p3)) {
		P3 dir = (p3[t->b]-p3[t->a]).cross(p3[t->c]-p3[t->a]);
		if (dir.dot(P3(0,0,1)) < 0) {
			trifun(p3[t->a], p3[t->c], p3[t->b], dir);
		}
	}
}

// template<class P, class F>
// void delaunay(vector<P>& ps, F trifun, vector<ftype> R) {
// 	if (sz(ps) == 3) { int d = (ps[0].cross(ps[1], ps[2]) < 0);
// 		trifun(0,1+d,2-d); }
// 	vector<P3> p3;
// 	for (P p : ps) p3.emplace_back(p.x, p.y, p.dist2());
// 	if (sz(ps) > 3) for(auto t:hull3d(p3)) if ((p3[t.b]-p3[t.a]).
// 			cross(p3[t.c]-p3[t.a]).dot(P3(0,0,1)) < 0)
// 		trifun(t.a, t.c, t.b);
// }


typedef Point<itype> P;
ftype segDist(P& s, P& e, P& p) {
	if (s==e) return (p-s).dist();
	itype d = (e-s).dist2();
	itype t = std::min<itype>(d,std::max<itype>(0,(p-s).dot(e-s)));
	return ((p-s)*d-(e-s)*t).dist()/ftype(d);
}

struct UF {
    int n;
    vector<int> par;
    UF(int _n) : n(_n) {
        for(int i = 0; i < n; i++) par.push_back(i);
    }
    int find(int a){
        if(a != par[a]) par[a] = find(par[a]);
        return par[a];
    }
    bool join(int a, int b){
        a = find(a);
        b = find(b);
        par[a] = b;
        return (a != b);
    }
};

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N;
	cin >> N;
	vector<Point<itype>> pt(N);
	vector<Point<int64_t>> o_pts(N);
	vector<itype> R(N);

	map<pair<int,int>, int> oidx;
	for(int i = 0; i < N; i++){
		int x, y;
		cin >> x >> y;
		o_pts[i].x = x; o_pts[i].y = y;
		oidx[{x, y}] = i;
		pt[i].x = x; pt[i].y = y;
		int r;
		cin >> r;
		R[i] = r;
	}
	set<pair<int,int> > segs;
	auto to_int = [&](itype f) -> int {
	    return int(f);
	};
	auto to_old = [&](P3 p) -> int {
		pair<int, int> z {to_int(p.x), to_int(p.y)};
		wssert(oidx.count(z));
		return oidx[z];
	};
	set<pair<int,int> > bad_segs;
	set<pair<int,int> > all_segs;

	int all_collinear = true;
	for(int i = 0; i < N; i++){
		if(o_pts[i].cross(o_pts[0], o_pts[1]) != 0) all_collinear = false;
	}

	if(all_collinear){
        vector<int> ord(N);
        for(int i = 0; i < N; i++) ord[i] = i;
        sort(ord.begin(), ord.end(), [&](int x, int y){ return o_pts[x] < o_pts[y]; });
        for(int i = 0; i + 1 < N; i++){
            all_segs.insert({ord[i], ord[i+1]});
        }
	} else {
		delaunay(pt, [&](P3 a, P3 b, P3 c, P3 dir) -> void {
			wssert(dir.z < 0);
			// TODO: maybe negate this?
			Point<itype> dir_numer(dir.x, dir.y);
			itype dir_denom = -2 * dir.z;
			wssert(dir_denom > 0);

			int x = to_old(a);
			int y = to_old(b);
			int z = to_old(c);
			//int x = a.idx;
			//int y = b.idx;
			//int z = c.idx;

			/*
			auto sq = [&](auto v) { return v * v; };
			cerr << "tri " << pt[x] << ' ' << pt[y] << ' ' << pt[z] << ' ' << dir_numer << '/' << dir_denom << '\n';
			cerr << R[to_old(x)] << ' ' << R[to_old(y)] << ' ' << R[to_old(z)] << '\n';
			cerr << (pt[x] * dir_denom - dir_numer).dist2() - sq(R[to_old(x)] * dir_denom) << '\n';
			cerr << (pt[y] * dir_denom - dir_numer).dist2() - sq(R[to_old(y)] * dir_denom) << '\n';
			cerr << (pt[z] * dir_denom - dir_numer).dist2() - sq(R[to_old(z)] * dir_denom) << '\n';
			*/

			vector<int> f = {x, y, z};
			sort(f.begin(), f.end());
			for(int i = 0; i < 3; i++){
				for(int j = i+1; j < 3; j++){
					all_segs.insert({f[i], f[j]});
					int k = 3-i-j;
					auto signum = [&](auto v) -> int { return (v > 0) - (v < 0); };
					if (signum((dir_numer).cross(pt[f[i]] * dir_denom, pt[f[j]] * dir_denom)) 
						!= signum(pt[f[k]].cross(pt[f[i]], pt[f[j]]))
					) {
						//cerr << "bad " << f[i] << ' ' << f[j] << ' ' << f[k] << '\n';
						//cerr << pt[f[i]] << ' ' << pt[f[j]] << ' ' << pt[f[k]] << ' ' << dir_numer << '/' << dir_denom << '\n';
						bad_segs.insert({f[i], f[j]});
					}
				}
			}
		}, R);
        for(auto p : bad_segs) all_segs.erase(p);
	}

	UF uf(N);
	vector<pair<int,int> > edges;
	for(auto [x, y] : all_segs){
		if(uf.join(x, y)) edges.push_back({x, y});
	}
	wssert(int(edges.size()) == N-1);
	cout << "YES" << '\n';
	for(auto [a, b] : edges){
		cout << to_int(pt[a].x) << ' ' << to_int(pt[a].y) << ' ' << to_int(pt[b].x) << ' ' << to_int(pt[b].y) << '\n';
	}
}
