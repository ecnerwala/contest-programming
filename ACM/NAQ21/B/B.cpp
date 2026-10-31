#include <bits/stdc++.h>

const double PI = std::acos(-1.);
const double TAU = 2 * PI;

template <typename T> struct Point3D {
	using P = Point3D;

	T x, y, z;
	Point3D() : x(0), y(0), z(0) {}
	Point3D(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

	template <typename U> explicit Point3D(const Point3D<U>& p) : x(T(p.x)), y(T(p.y)), z(T(p.z)) {}

	friend std::istream& operator >> (std::istream& i, P& p) { return i >> p.x >> p.y >> p.z; }
	friend std::ostream& operator << (std::ostream& o, const P& p) { return o << "(" << p.x << "," << p.y << "," << p.z << ")"; }

	friend bool operator == (const P& a, const P& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
	friend bool operator != (const P& a, const P& b) { return a.x != b.x || a.y != b.y || a.z != b.z; }

	P& operator += (const P& o) { x += o.x, y += o.y, z += o.z; return *this; }
	P& operator -= (const P& o) { x -= o.x, y -= o.y, z -= o.z; return *this; }
	friend P operator + (const P& a, const P& b) { return P(a) += b; }
	friend P operator - (const P& a, const P& b) { return P(a) -= b; }

	P& operator *= (const T& t) { x *= t, y *= t, z *= t; return *this; }
	P& operator /= (const T& t) { x /= t, y /= t, z /= t; return *this; }
	friend P operator * (const P& p, const T& t) { return P(p) *= t; }
	friend P operator * (const T& t, const P& p) { return P(p) *= t; }
	friend P operator / (const P& a, const T& t) { return P(a) /= t; }

	friend P operator + (const P& a) { return P(+a.x, +a.y, +a.z); }
	friend P operator - (const P& a) { return P(-a.x, -a.y, -a.z); }

	friend T dot(const P& a, const P& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
	friend T norm(const P& a) { return dot(a,a); }
	friend P cross(const P& a, const P& b) { return P(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

	friend T int_norm(const P& p) {
		return gcd(gcd(abs(p.x), abs(p.y)), abs(p.z));
	}
	friend P int_unit(const P& p) {
		T g = int_norm(p);
		return g ? p / g : p;
	}

	friend T abs(const P& a) { return sqrt(std::max(T(0), norm(a))); }
	friend P unit(const P& a) { return a / abs(a); }

	friend T vol(const P& a, const P& b, const P& c, const P& d) { return dot(cross(b-a, c-a), d-a); }

	friend bool lexLess(const P& a, const P& b) { return tie(a.x, a.y, a.z) < tie(b.x, b.y, b.z); }

	friend bool parallelSame(const P& a, const P& b) {
		assert(a != P());
		assert(b != P());
		return lexLess(a, P()) == lexLess(b, P());
	}
};

using pt_t = Point3D<double>;

double seg_pt_dist(pt_t S, pt_t T, pt_t P) {
	if (dot(P-S, T-S) < 0) {
		return abs(P-S);
	}
	if (dot(P-T, S-T) < 0) {
		return abs(P-T);
	}
	return abs(cross(P-T, S-T)) / abs(S-T);
}

double pt_ray_dist(pt_t S, pt_t P, pt_t V) {
	if (dot(S-P, V) < 0) {
		return abs(S-P);
	}
	return abs(cross(S-P, V)) / abs(V);
}

double seg_ray_dist(pt_t S, pt_t T, pt_t P, pt_t V) {
	double ans = std::min({seg_pt_dist(S, T, P), pt_ray_dist(S, P, V), pt_ray_dist(T, P, V)});

	T -= S;
	// (S + T * a - P - V * b) dot itself for 0 <= a <= 1 and b >= 0
	// T dot T a^2
	// V dot V b^2
	// -2 * T dot V ab
	// 2 * T dot (S-P) a
	// 2 * V dot (P-S) b
	// To minimize: partial f / partial a = 0 and partial f / par
	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	pt_t S, T; cin >> S >> T;
	int N; cin >> N;
	bool good = true;
	while (N--) {
		pt_t P, V; cin >> P >> V;
		int M; cin >> M;
		double max_d = 0;
		while (M--) {
			pt_t C; cin >> C;
			max_d = std::max(max_d, abs(P-C));
		}
		if (seg_ray_dist(S, T, P, V) < max_d) {
			good = false;
		}
	}
	cout << (good ? "Go" : "Surrender") << '\n';

	return 0;
}
