#include <bits/stdc++.h>

const double PI = acos(-1.);
const double TAU = 2 * PI;

template <typename T> struct Point3D {
	using P = Point3D;

	T x, y, z;
	Point3D() : x(0), y(0), z(0) {}
	Point3D(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {}

	template <typename U> explicit Point3D(const Point3D<U>& p) : x(T(p.x)), y(T(p.y)), z(T(p.z)) {}

	friend std::istream& operator >> (std::istream& i, P& p) { return i >> p.x >> p.y >> p.z; }
	friend std::ostream& operator << (std::ostream& o, const P& p) { return o << "(" << p.x << "," << p.y << "," << p.z << ")"; }

	//friend bool operator == (const P& a, const P& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
	//friend bool operator != (const P& a, const P& b) { return a.x != b.x || a.y != b.y || a.z != b.z; }

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
		//assert(a != P());
		//assert(b != P());
		return lexLess(a, P()) == lexLess(b, P());
	}
	friend inline T my_cross(const P& a, const P& b) {
		const P normal = P(1, 1, 1);
		assert(dot(a, normal) == 0);
		assert(dot(b, normal) == 0);
		return dot(cross(a, b), normal);
	}
	friend inline bool is_ccw_eq(const P& a, const P& b) {
		int64_t c = my_cross(a, b);
		if (c > 0) return true;
		else if (c < 0) return false;
		else return parallelSame(a, b);
	}

	friend bool operator < (const P& a, const P& b) {
		const P base_ang = P(0, -1, 1);
		bool ca = is_ccw_eq(base_ang, a);
		bool cb = is_ccw_eq(base_ang, b);
		if (ca != cb) return ca > cb;
		return !is_ccw_eq(b, a);
	}
	friend bool operator == (const P& a, const P& b) {
		return my_cross(a, b) == 0 && parallelSame(a, b);
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	using pt_t = Point3D<int64_t>;
	std::vector<pt_t> pts(N);
	for (int i = 0; i < N; i++) {
		cin >> pts[i];
		int64_t s = pts[i].x + pts[i].y + pts[i].z;
		pts[i] *= 3;
		pts[i].x -= s;
		pts[i].y -= s;
		pts[i].z -= s;
	}

	std::map<pt_t, std::vector<std::pair<int, int>>> ops;
	std::vector<int> base_cnt(N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			{
				pt_t d_st = pts[i] - pts[j];
				pt_t d_en = -d_st;
				if (d_st == d_en) {
					assert(d_st.x == 0 && d_st.y == 0 && d_st.z == 0);
					if (i < j) base_cnt[i]++;
					continue;
				}
				ops[d_st].push_back({i, +1});
				ops[d_en].push_back({i, -1});
				if (d_st < d_en) {
					// do nothing
				} else {
					base_cnt[i]++;
				}
			}
		}
	}
	for (int i = 0; i < N; i++) {
		cerr << base_cnt[i] << '\n';
	}

	for (const auto& [_, changes] : ops) {
	}

	return 0;
}
