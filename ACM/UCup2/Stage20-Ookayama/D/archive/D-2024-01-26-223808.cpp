#include <bits/stdc++.h>

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
	explicit Point3D(T x=0, T y=0, T z=0) : x(x), y(y), z(z) {}
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
	T dist2() const { return x*x + y*y + z*z; }
	double dist() const { return sqrt((double)dist2()); }
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

struct PR {
	void ins(int x) { (a == -1 ? a : b) = x; }
	void rem(int x) { (a == x ? a : b) = -1; }
	int cnt() { return (a != -1) + (b != -1); }
	int a, b;
};

struct F { P3 q; int a, b, c; };

vector<F> hull3d(const vector<P3>& A) {
	assert(sz(A) >= 4);
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

	std::mt19937_64 mt(48);
	int T; cin >> T;
	while (T--) {
		int N, R; cin >> N >> R;
		cerr << "CASE " << N << " " << R << endl;
		using pt_t = kactl::P3;
		std::vector<pt_t> points; points.reserve(N+1); points.resize(N);
		std::uniform_real_distribution<double> dist(-1e-8, 1e-8);
		for (auto& p : points) {
			double x, y, z; cin >> x >> y >> z;

			double a = std::sqrt(x * x + y * y + z * z - R * R);
			x /= a; y /= a; z /= a;

			// preterb randomly
			x += dist(mt); y += dist(mt); z += dist(mt);

			p = pt_t(x, y, z);
		}
		points.push_back(pt_t(0, 0, 0));
		for (int i = 0; i <= N; i++) {
			cerr << i << ' ' << points[i].x << ' ' << points[i].y << ' ' << points[i].z << '\n';
		}
		auto faces = kactl::hull3d(points);
		int F = int(faces.size());
		std::vector<bool> is_good(F);
		std::vector<std::pair<std::array<int, 2>, int>> half_edges; half_edges.reserve(3*F);
		int ans = 0;
		for (int f = 0; f < F; f++) {
			auto [q, a, b, c] = faces[f];
			// Now, check if the face is outside
			cerr << "face " << a << ' ' << b << ' ' << c << ' ' << q.dot(points[a]) << ' ' << q.dist() << '\n';
			is_good[f] = (q.dot(points[a]) < q.dist());
			ans += is_good[f];
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
		std::sort(half_edges.begin(), half_edges.end());
		for (int i = 0; i < int(half_edges.size()); i += 2) {
			assert(half_edges[i].first == half_edges[i+1].first);
			auto [a, b] = half_edges[i].first;
			int f1 = half_edges[i].second;
			int f2 = half_edges[i+1].second;
			if (is_good[f1] && is_good[f2]) {
				cerr << "adjacent " << f1 << ' ' << f2 << ' ' << points[a].cross(points[b]).dist() << ' ' << (points[a] - points[b]).dist() << '\n';
				if (points[a].cross(points[b]).dist() < (points[a] - points[b]).dist2()) {
					cerr << "merging " << f1 << ' ' << f2 << '\n';
					ans -= merge(f1, f2);
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
