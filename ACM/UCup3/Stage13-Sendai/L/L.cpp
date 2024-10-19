#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = mod_inv_in_range(v, MOD);
		return res;
	}
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

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

	friend bool lexLess(const P& a, const P& b) { return std::tie(a.x, a.y, a.z) < std::tie(b.x, b.y, b.z); }

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
		//cerr << i << ' ' << pts[i] << '\n';
	}

	std::vector<std::pair<pt_t, std::pair<int, int>>> ops; ops.reserve(N * (N-1));
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
				ops.push_back({d_st, {i, +1}});
				ops.push_back({d_en, {i, -1}});
				if (d_st < d_en) {
					// do nothing
				} else {
					base_cnt[i]++;
				}
			}
		}
	}
	std::sort(ops.begin(), ops.end());
	//for (int i = 0; i < N; i++) { cerr << base_cnt[i] << '\n'; }
	std::vector<pt_t> cnds(N+1);
	for (int i = 0; i < N; i++) {
		cnds[base_cnt[i]+1] = pts[i];
	}
	for (int i = 0; i < N; i++) {
		cnds[i+1] += cnds[i];
	}

	auto get_val = [](pt_t p) {
		auto sq = [](int64_t a) -> uint64_t {
			a = std::abs(a);
			return uint64_t(a) * uint64_t(a);
		};
		return sq(p.x) + sq(p.y) + sq(p.z);
	};
	std::vector<uint64_t> best_pts(N+1);
	for (int i = 0; i <= N; i++) {
		best_pts[i] = get_val(cnds[i]);
	}
	std::vector<int> did_change; did_change.reserve(N * (N-1));
	for (int it = 0; it < int(ops.size()); ) {
		pt_t base = ops[it].first;
		did_change.clear();
		for (; it < int(ops.size()) && ops[it].first == base; ++it) {
			auto [i, delta] = ops[it].second;
			if (delta == +1) {
				base_cnt[i]++;
				if (0 <= base_cnt[i] && base_cnt[i] <= N) {
					cnds[base_cnt[i]] -= pts[i];
					did_change.push_back(base_cnt[i]);
				}
			} else if (delta == -1) {
				if (0 <= base_cnt[i] && base_cnt[i] <= N) {
					cnds[base_cnt[i]] += pts[i];
					did_change.push_back(base_cnt[i]);
				}
				base_cnt[i]--;
			} else assert(false);
		}
		//for (int i = 0; i < N; i++) { cerr << base_cnt[i] << ' '; } cerr << '\n';
		for (auto v : did_change) {
			//cerr << "candidate " << v << ' ' << cnds[v] << ' ' << get_val(cnds[v]) << '\n';
			best_pts[v] = std::max(best_pts[v], get_val(cnds[v]));
		}
	}
	using num = modnum<998244353>;
	for (int i = 1; i <= N; i++) {
		cout << num(int(best_pts[i] % uint64_t(num::MOD))) / (num(i) * num(i) * num(27)) << '\n';
	}

	return 0;
}
