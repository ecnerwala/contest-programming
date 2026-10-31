#include<bits/stdc++.h>

template<typename num = int>
struct hurwitz_quaternion {
	// we store the doubled quaternion
	num s,x,y,z;
	hurwitz_quaternion() : s(0), x(0), y(0), z(0) {}
	hurwitz_quaternion(num v) : s(2*v), x(0), y(0), z(0) {}
	hurwitz_quaternion(num s_, num x_, num y_, num z_) : s(2*s_), x(2*x_), y(2*y_), z(2*z_) {}
	struct doubled_coords_tag {};
	hurwitz_quaternion(doubled_coords_tag, num s_, num x_, num y_, num z_) : s(s_), x(x_), y(y_), z(z_) {
		assert((s & 1) == (x & 1) && (s & 1) == (y & 1) && (s & 1) == (z & 1));
	}
	friend std::ostream& operator << (std::ostream& o, const hurwitz_quaternion& q) {
		o << double(q.s)/2;
		{
			std::ios_base::fmtflags f(o.flags());
			o << std::showpos << double(q.x)/2 << "i" << double(q.y)/2 << "j" << double(q.z)/2 << "k";
			o.flags(f);
		}
		return o;
	}

	explicit operator bool() const {
		return s || x || y || z;
	}

	friend bool operator == (const hurwitz_quaternion& a, const hurwitz_quaternion& b) {
		return std::tie(a.s,a.x,a.y,a.z) == std::tie(b.s,b.x,b.y,b.z);
	}
	friend bool operator != (const hurwitz_quaternion& a, const hurwitz_quaternion& b) { return !(a == b); }

	num real_doubled() const {
		return s;
	}
	std::array<num, 3> imag_doubled() const {
		return {x, y, z};
	}

	friend num norm(const hurwitz_quaternion& q) {
		return (q.s * q.s + q.x * q.x + q.y * q.y + q.z * q.z) >> 2;
	}
	friend hurwitz_quaternion conj(const hurwitz_quaternion& q) {
		return hurwitz_quaternion(doubled_coords_tag{}, q.s, -q.x, -q.y, -q.z);
	}

	friend hurwitz_quaternion operator + (const hurwitz_quaternion& q) {
		return hurwitz_quaternion(doubled_coords_tag{}, +q.s, +q.x, +q.y, +q.z);
	}
	friend hurwitz_quaternion operator - (const hurwitz_quaternion& q) {
		return hurwitz_quaternion(doubled_coords_tag{}, -q.s, -q.x, -q.y, -q.z);
	}

	hurwitz_quaternion& operator += (const hurwitz_quaternion& o) {
		s += o.s;
		x += o.x;
		y += o.y;
		z += o.z;
		return *this;
	}
	friend hurwitz_quaternion operator + (const hurwitz_quaternion& a, const hurwitz_quaternion& b) {
		return hurwitz_quaternion(doubled_coords_tag{}, a.s + b.s, a.x + b.x, a.y + b.y, a.z + b.z);
	}
	hurwitz_quaternion& operator -= (const hurwitz_quaternion& o) {
		s -= o.s;
		x -= o.x;
		y -= o.y;
		z -= o.z;
		return *this;
	}
	friend hurwitz_quaternion operator - (const hurwitz_quaternion& a, const hurwitz_quaternion& b) {
		return hurwitz_quaternion(doubled_coords_tag{}, a.s - b.s, a.x - b.x, a.y - b.y, a.z - b.z);
	}

	friend hurwitz_quaternion operator * (const num& a, const hurwitz_quaternion& q) {
		return hurwitz_quaternion(doubled_coords_tag{}, a*q.s, a*q.x, a*q.y, a*q.z);
	}
	friend hurwitz_quaternion operator * (const hurwitz_quaternion& q, const num& a) {
		return hurwitz_quaternion(doubled_coords_tag{}, q.s*a, q.x*a, q.y*a, q.z*a);
	}
	hurwitz_quaternion& operator *= (const num& a) {
		s *= a;
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}

	friend hurwitz_quaternion operator * (const hurwitz_quaternion& a, const hurwitz_quaternion& b) {
		return hurwitz_quaternion(
			doubled_coords_tag{},
			(a.s * b.s - a.x * b.x - a.y * b.y - a.z * b.z) >> 1,
			(a.s * b.x + a.x * b.s + a.y * b.z - a.z * b.y) >> 1,
			(a.s * b.y + a.y * b.s + a.z * b.x - a.x * b.z) >> 1,
			(a.s * b.z + a.z * b.s + a.x * b.y - a.y * b.x) >> 1
		);
	}
	hurwitz_quaternion& operator *= (const hurwitz_quaternion& o) {
		return *this = *this * o;
	}

	struct div_t {
		hurwitz_quaternion quot, rem;
	};
	friend div_t right_div(const hurwitz_quaternion& a, const hurwitz_quaternion& b) {
		hurwitz_quaternion numer = conj(b) * a;
		num denom = norm(b);

		auto floor_div = [](num u, num v) -> num {
			if ((u^v) >= 0) {
				return u/v;
			} else {
				auto res = std::div(u, v);
				return res.quot - bool(res.rem);
			}
		};
		num s = floor_div(numer.s, denom);
		num x = floor_div(numer.x, denom);
		num y = floor_div(numer.y, denom);
		num z = floor_div(numer.z, denom);

		hurwitz_quaternion q_odd(doubled_coords_tag{}, s | 1, x | 1, y | 1, z | 1);
		hurwitz_quaternion r_odd = a - b * q_odd;
		hurwitz_quaternion q_even(doubled_coords_tag{}, (s+1)&~num(1), (x+1)&~num(1), (y+1)&~num(1), (z+1)&~num(1));
		hurwitz_quaternion r_even = a - b * q_even;
		div_t res = norm(r_odd) < norm(r_even) ? div_t{q_odd, r_odd} : div_t{q_even, r_even};
		assert(norm(res.rem) < norm(b));
		return res;
	}

	friend hurwitz_quaternion right_gcd(hurwitz_quaternion a, hurwitz_quaternion b) {
		while (a) {
			b = right_div(b, a).rem;
			std::swap(a, b);
		}
		return b;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using qt = hurwitz_quaternion<int64_t>;
	int N; cin >> N;
	vector<qt> A(N);
	for (auto& a : A) {
		int64_t x, y, z; cin >> x >> y >> z;
		a = qt(0, x, y, z);
	}

	auto check_cnd = [&](int64_t r, qt cnd) -> bool {
		assert(norm(cnd) == r);
		assert(r % 2 != 0);

		for (auto a : A) {
			a = conj(cnd) * a;
			assert(a.s % r == 0);
			assert(a.x % r == 0);
			assert(a.y % r == 0);
			assert(a.z % r == 0);
			a.s /= r, a.x /= r, a.y /= r, a.z /= r;

			a = a * cnd;
			if (a.s % r == 0 && a.x % r == 0 && a.y % r == 0 && a.z % r == 0) {
				// it's good
			} else {
				return false;
			}
		}
		return true;
	};

	qt G = 0;
	for (auto a : A) G = right_gcd(G, a);

	assert(norm(G) % 4 != 0); // otherwise these aren't primitive

	int64_t gcd_norm = 0;
	for (auto a : A) gcd_norm = std::gcd(gcd_norm, norm(a));

	int64_t r = 1;

	for (int64_t p = 2; p * p <= gcd_norm; p++) {
		if (p * p * p > gcd_norm) {
			p = int64_t(sqrt((long double)gcd_norm));
		}
		int64_t pow_p = 1;
		while (gcd_norm % (p * p) == 0) {
			pow_p *= p;
			gcd_norm /= p * p;
		}
		if (gcd_norm % p == 0) gcd_norm /= p;

		if (pow_p == 1) continue;

		assert(p != 2);
		pow_p = gcd(pow_p, norm(G));

		while (pow_p > 1 && !check_cnd(pow_p, right_gcd(G, pow_p))) {
			pow_p /= p;
		}

		r *= pow_p;
	}

	qt Q = right_gcd(G, r);

	cout << r*r << '\n';
	qt x = Q * qt(0, 1, 0, 0) * conj(Q);
	cout << x.x/2 << ' ' << x.y/2 << ' ' << x.z/2 << '\n';
	qt y = Q * qt(0, 0, 1, 0) * conj(Q);
	cout << y.x/2 << ' ' << y.y/2 << ' ' << y.z/2 << '\n';
	qt z = Q * qt(0, 0, 0, 1) * conj(Q);
	cout << z.x/2 << ' ' << z.y/2 << ' ' << z.z/2 << '\n';

	return 0;
}
