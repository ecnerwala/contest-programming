#include <bits/stdc++.h>

void wssert(bool b) {
	if (!b) { std::cout << "GARBAGE" << '\n'; exit(0); }
}

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

template<class T> struct Point3D {
	typedef Point3D P;
	typedef const P& R;
	T x, y, z;
	explicit Point3D(T x_=0, T y_=0, T z_=0) : x(x_), y(y_), z(z_) {}
	bool operator<(R p) const {
		return tie(x, y, z) < tie(p.x, p.y, p.z); }
	bool operator==(R p) const {
		return tie(x, y, z) == tie(p.x, p.y, p.z); }
	P operator+(R p) const { return P(x+p.x, y+p.y, z+p.z); }
	P operator-(R p) const { return P(x-p.x, y-p.y, z-p.z); }
	P operator*(T d) const { return P(x*d, y*d, z*d); }
	P operator/(T d) const { return P(x/d, y/d, z/d); }
	T dot(R p) const { return x*p.x + y*p.y + z*p.z; }
	P cross(R p) const {
		return P(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
	}
	friend P cross3(P a, P b, P c) { return (b-a).cross(c-a); }
	T dist2() const { return x*x + y*y + z*z; }
	double dist() const { return sqrt((double)dist2()); }
	double abs() const { return sqrt((double)dist2()); }
	//Azimuthal angle (longitude) to x-axis in interval [-pi, pi]
	double phi() const { return atan2(y, x); } 
	//Zenith angle (latitude) to the z-axis in interval [0, pi]
	double theta() const { return atan2(sqrt(x*x+y*y),z); }
	P unit() const { return *this/(T)dist(); } //makes dist()=1
	//returns unit vector normal to *this and p
	P normal(P p) const { return cross(p).unit(); }
	//returns point rotated 'angle' radians ccw around axis
	P rotate(double angle, P axis) const {
		double s = sin(angle), c = cos(angle); P u = axis.unit();
		return u*dot(u)*(1-c) + (*this)*c - cross(u)*s;
	}
};


typedef Point3D<double> P3;

using pt3 = P3;
using ftype = double;

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
    face(int a, int b, int c, pt3 q) : a(a), b(b), c(c), q(q) {
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
const ftype EPS = 1e-11;

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
            if((p[ve[0]] - p[i]).abs() > EPS) ve.push_back(i);
        }else if(sz(ve) == 2) {
            if((p[ve[1]] - p[ve[0]]).cross(p[i] - p[ve[0]]).abs() > EPS) ve.push_back(i);
        }else if(abs((p[i] - p[ve[0]]).dot((p[ve[1]] - p[ve[0]]).cross(p[ve[2]] - p[ve[0]]))) > EPS) {
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
	assert(sz(p) >= 4);
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
            ftype Q = (p[i] - p[F->a]).dot(F->q);
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
			q = q * -1;
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
	for (F& it : FS) if ((A[it.b] - A[it.a]).cross(
		A[it.c] - A[it.a]).dot(it.q) <= 0) swap(it.c, it.b);
	return FS;
};

}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	cerr << fixed << setprecision(10);

	std::mt19937_64 mt(48);
	int T; cin >> T;
	while (T--) {
		int N, R; cin >> N >> R;
		//cerr << "CASE " << N << " " << R << endl;
		using pt_t = kactl::P3;
		std::vector<pt_t> points; points.reserve(N+1); points.resize(N);
		std::uniform_real_distribution<double> dist(-1e-7, 1e-7);
		for (auto& p : points) {
			double x, y, z; cin >> x >> y >> z;

			// preterb randomly
			double l = std::sqrt(x * x + y * y + z * z);
			x += dist(mt) * l; y += dist(mt) * l; z += dist(mt) * l;

			double a = std::sqrt(x * x + y * y + z * z - R * R);
			x /= a; y /= a; z /= a;

			p = pt_t(x, y, z);
		}
		points.push_back(pt_t(0, 0, 0));
		for (int i = 0; i <= N; i++) {
			//cerr << "{" << points[i].x << ", " << points[i].y << ", " << points[i].z << "},\n";
			//cerr << i << ' ' << points[i].x << ' ' << points[i].y << ' ' << points[i].z << "  " << points[i].dist() << '\n';
		}
		if (N <= 2) {
			cout << 1 << '\n';
			continue;
		}
		auto faces = kactl::hull3(points);
		int F = int(faces.size());
		std::vector<bool> is_good(F);
		std::vector<std::pair<std::array<int, 2>, int>> half_edges; half_edges.reserve(3*F);
		for (int f = 0; f < F; f++) {
			//auto [q, a, b, c] = faces[f];
			int a = faces[f]->a;
			int b = faces[f]->b;
			int c = faces[f]->c;
			// Now, check if the face is outside
			//is_good[f] = (q.dot(points[a]) < q.dist());
			//cerr << "face " << f << ' ' << a << '-' << b << '-' << c << ' ' << q.dot(points[a]) << ' ' << q.dist() << ' ' << (is_good[f] ? "good" : "bad") << '\n';
			//ans += is_good[f];
			half_edges.push_back({{std::min(a, b), std::max(a, b)}, f});
			half_edges.push_back({{std::min(b, c), std::max(b, c)}, f});
			half_edges.push_back({{std::min(c, a), std::max(c, a)}, f});
		}
		std::vector<int> par(F, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a), b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};
		int num_merges = 0;
		std::sort(half_edges.begin(), half_edges.end());
		wssert(int(half_edges.size()) % 2 == 0);
		for (int i = 0; i < int(half_edges.size()); i += 2) {
			wssert(half_edges[i].first == half_edges[i+1].first);
			auto [a, b] = half_edges[i].first;
			int f1 = half_edges[i].second;
			int f2 = half_edges[i+1].second;
			if (points[a].dist2() < 1 || points[b].dist2() < 1 || ((points[a] - points[b]).dot(points[b]) < 0 && (points[b] - points[a]).dot(points[a]) < 0 && points[a].cross(points[b]).dist2() < (points[a] - points[b]).dist2())) {
				is_good[f1] = true;
				is_good[f2] = true;
				//cerr << "merging " << f1 << ' ' << f2 << '\n';
				num_merges += merge(f1, f2);
			}
		}
		int num_verts = 0;
		for (int f = 0; f < F; f++) {
			if (!is_good[f]) {
				int a = faces[f]->a;
				int b = faces[f]->b;
				int c = faces[f]->c;
				auto q = cross3(points[a], points[b], points[c]);
				// check if this guy is a singleton
				if (q.dot(points[a]) < q.dist() && q.dot(cross3(points[a], points[b], pt_t())) >= 0 && q.dot(cross3(points[b], points[c], pt_t())) >= 0 && q.dot(cross3(points[c], points[a], pt_t())) >= 0) {
					is_good[f] = true;
				}
			}
			num_verts += is_good[f];
		}
		int ans = num_verts - num_merges;
		cout << ans << '\n';
	}

	return 0;
}
