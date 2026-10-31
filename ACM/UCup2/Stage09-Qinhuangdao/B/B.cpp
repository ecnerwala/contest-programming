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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

// return r * (a^b)
template <typename T> T pow_mul(T r, T a, long long b) {
	assert(b >= 0);
	while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

using num = modnum<998244353>;
struct mat {
	std::array<std::array<num, 3>, 3> v;
	friend mat operator * (const mat& a, const mat& b) {
		mat r;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 3; k++) {
					r.v[i][k] += a.v[i][j] * b.v[j][k];
				}
			}
		}
		return r;
	}
	mat& operator *= (const mat& o) {
		return *this = (*this * o);
	}
};

std::string gen_string(int64_t a, int64_t b) {
	std::string res;
	int64_t ia = 0, ib = 0;
	while (ia + ib < a + b) {
		if ((__int128_t)ia * b <= (__int128_t)ib * a) {
			res += '0';
			ia++;
		} else {
			res += '1';
			ib++;
		}
	}
	return res;
}

std::string gen_string_fast(int64_t a, int64_t b) {
	std::string v0 = "0";
	int64_t w0 = b;
	std::string v1 = "1";
	int64_t w1 = a;
	while (true) {
		assert(w0 > 0 && w1 > 0);
		if (w0 == w1) {
			// alternating
			std::string res;
			for (int z = 0; z < w0; z++) {
				res += v0;
				res += v1;
			}
			return res;
		} else if (w0 < w1) {
			v1 += v0;
			w1 -= w0;
		} else if (w0 > w1) {
			v0 += v1;
			w0 -= w1;
		} else assert(false);
	}
}

num yasp(int64_t a, int64_t b) {
	mat v0;
	v0.v[0][0] = 1;
	v0.v[0][1] = 1;
	v0.v[1][1] = 1;
	v0.v[2][1] = 1;
	v0.v[2][2] = 1;
	int64_t w0 = b;
	mat v1;
	v1.v[0][0] = 1;
	v1.v[1][1] = 1;
	v1.v[0][2] = 1;
	v1.v[1][2] = 1;
	v1.v[2][2] = 1;
	int64_t w1 = a;
	while (true) {
		assert(w0 > 0 && w1 > 0);
		if (w0 == w1) {
			// alternating
			mat res;
			res.v[0][0] = 1;
			res.v[1][1] = 1;
			res.v[2][2] = 1;
			res = pow_mul(res, (v0 * v1), w0);
			return res.v[0][0] + res.v[0][1] + res.v[0][2];
		} else if (w0 < w1) {
			int64_t r = (w1 - 1) / w0;
			v1 = pow_mul(v1, v0, r);
			w1 -= w0 * r;
		} else if (w0 > w1) {
			int64_t r = (w0 - 1) / w1;
			v0 = pow_mul(v0, v1, r);
			w0 -= w1 * r;
		} else assert(false);
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int a = 1; a <= 20; a++) {
		for (int b = 1; b <= 20; b++) {
			std::string slo = gen_string(a, b);
			std::string res = gen_string_fast(a, b);
			if (slo != res) {
				//cerr << a << ' ' << b << ' ' << slo << ' ' << res << '\n';
				assert(false);
			}
		}
	}
	//cerr << "tests done" << '\n';

	int T; cin >> T;
	while (T--) {
		int64_t A, B; cin >> A >> B;
		cout << yasp(A, B) << '\n';
	}

	return 0;
}
