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

using std::cerr;

int jacobi(int64_t n, int64_t m) {
	int r = 1;
	assert(m & 1);
	assert(m > 0);
	if (n < 0) {
		if (m & 2) r = -r;
		n = -n;
	}
	while (m > 1) {
		assert(n > 0);
		int t = __builtin_ctzll(n);
		n >>= t;
		if ((t & 1) && (((m & 7) == 3) || ((m & 7) == 5))) {
			r = -r;
		}
		// n and m both odd
		if (n < m) {
			if ((n & 2) && (m & 2)) {
				r = -r;
			}
			std::swap(n, m);
		}
		n -= m;
	}
	return r;
}

bool is_qr_slow(int a, int p) {
	if (a == 0) return true;
	int e = (p-1)/2;
	int r = 1;
	while (e) {
		if (e & 1) r = int(int64_t(r) * int64_t(a) % p);
		e >>= 1;
		a = int(int64_t(a) * int64_t(a) % p);
	}
	assert(r == 1 || r == p-1);
	return r == 1;
}

std::array<int, 8> solve_wrap_slow(int P, int T) {
	std::vector<bool> is_qr(P);
	for (int i = 0; i < P; i++) {
		is_qr[i] = is_qr_slow(i, P);
	}
	std::array<int, 8> cnts; cnts.fill(0);
	for (int i = 0; i < P; i++) {
		if (i == 0 || (i+T)%P == 0 || (i+2*T)% P == 0) continue;
		bool a = is_qr[i];
		bool b = is_qr[(i+T)%P];
		bool c = is_qr[(i+2*T)%P];
		cnts[(a << 0) | (b << 1) | (c << 2)]++;
	}
	return cnts;
}

int64_t solve_wrap(int64_t P) {
	assert(P & 1);
	int64_t res = P/4 - 1;
	if ((P & 7) == 1) {
		res --;
	} else if ((P & 7) == 3) {
		res ++;
	} else if ((P & 7) == 5) {
	} else if ((P & 7) == 7) {
	}
	return res;
}

int64_t solve(int64_t P, int T) {
	if ((P-2)/2 < T) {
		T = int((P-2)/2);
	}
	//cerr << "solve " << P << ' ' << T << '\n';
	int64_t ans = solve_wrap(P) * T;

	std::vector<int> jac(4*T+1);
	// do whatever with qr[0]
	for (int i = 1; i <= 2 * T; i++) {
		int v = jacobi(i, P);
		jac[2*T+i] = v;
		jac[2*T-i] = (P & 2) ? -v : v;
	}

	std::vector<int> pref(jac.size()+1);
	std::vector<int> pref2(jac.size()+2);
	for (int i = 0; i < int(jac.size()); i++) {
		pref[i+1] = pref[i] + jac[i];
		pref2[i+2] = pref2[i] + jac[i];
	}

	int64_t extras = 0;
	for (int i = 2*T-(T-1); i < 2*T; i++) {
		// negative
		int t_lo = (2*T + 1) - i;
		int t_hi = T;
		assert(1 <= t_lo && t_lo <= t_hi && t_hi <= T);
		extras += jac[i] * (pref2[i+2*t_hi+2] - pref2[i+2*t_lo]);
		extras += jac[i] * (pref[i+t_hi+1] - pref[i+t_lo]);
		extras += (t_hi + 1 - t_lo);
	}

	for (int j = 2*T + 1; j <= 2 * T + (T-1); j++) {
		int t_lo = j - (2*T - 1);
		int t_hi = T;
		assert(1 <= t_lo && t_lo <= t_hi && t_hi <= T);
		extras += jac[j] * (pref[j+t_hi+1] - pref[j+t_lo]);
	}

	assert(extras % 4 == 0);
	extras /= 4;

	// double because we only counted neg pos pos, not neg neg pos
	ans -= extras * 2;

	//cerr << "ans " << ans << '\n';
	return ans;
}

void test() {
	auto is_prime = [&](int p) -> bool {
		if (p <= 1) return false;
		for (int i = 2; i * i <= p; i++) if (p % i == 0) return false;
		return true;
	};

	for (int m = 1; m <= 7; m += 2) {
		cerr << "mod 8 " << m << '\n';
		for (int p = m; p <= 500; p += 8) {
			if (!is_prime(p)) continue;
			cerr << "prime " << p << '\n';
			for (int i = 1; i < p; i++) {
				int v = jacobi(i-p, p);
				bool r = is_qr_slow(i, p);
				if ((v == 1) != r) {
					cerr << p << ' ' << v << ' ' << r << '\n';
					assert(false);
				}
			}
			{
				auto cnts = solve_wrap_slow(p, 1);
				for (int c : cnts) { cerr << c << ' '; } cerr << '\n';
				cerr << "tot " << p << ' ' << " avg " << p / 4 << ' ' << " val " << cnts[0] + cnts[7] << '\n';
				assert(cnts[0] + cnts[7] == solve_wrap(p));
			}
			{
				int v = 2;
				while (is_qr_slow(v, p)) v++;
				auto cnts = solve_wrap_slow(p, v);
				for (int c : cnts) { cerr << c << ' '; } cerr << '\n';
				cerr << "tot " << p << ' ' << " avg " << p / 4 << ' ' << " val " << cnts[0] + cnts[7] << '\n';
				assert(cnts[0] + cnts[7] == solve_wrap(p));
			}
		}
	}

}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//test();

	int CASES; cin >> CASES;
	while (CASES--) {
		int64_t P; int T; cin >> P >> T;
		cout << solve(P, T) << '\n';
	}

	return 0;
}
