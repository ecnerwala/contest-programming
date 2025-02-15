#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

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

template <typename num> struct cplx {
	num re, im;

	cplx& operator += (cplx o) { re += o.re, im += o.im; return *this; }
	cplx& operator -= (cplx o) { re -= o.re, im -= o.im; return *this; }
	friend cplx operator + (cplx a, cplx b) { return cplx{a.re + b.re, a.im + b.im}; }
	friend cplx operator - (cplx a, cplx b) { return cplx{a.re - b.re, a.im - b.im}; }
	friend cplx operator + (cplx a) { return cplx{+a.re, +a.im}; }
	friend cplx operator - (cplx a) { return cplx{-a.re, -a.im}; }

	friend cplx operator * (cplx a, cplx b) { return cplx{a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re}; }

	friend cplx mul_i(cplx a) { return cplx{-a.im, a.re}; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::string S; cin >> S;
	std::vector<int> pow6(N+1);
	pow6[0] = 1;
	for (int i = 1; i <= N; i++) pow6[i] = pow6[i-1] * 6;

	assert(int(S.size()) == pow6[N]);
	for (int i = 0; i < pow6[N]; i++) {
		if (S[pow6[N] - 1 - i] == '0') S[i] = '0';
	}

	const int W = 7;
	std::vector<int> powW(N+1);
	powW[0] = 1;
	for (int i = 1; i <= N; i++) powW[i] = powW[i-1] * W;

	using num = modnum<998244353>;
	std::vector<num> vec(powW[N]);

	for (int i = 0; i < pow6[N]; i++) {
		if (S[i] == '1') {
			int ni = 0;
			for (int a = 0, cur = i; a < N; a++) {
				ni += powW[a] * (cur % 6);
				cur /= 6;
			}
			vec[ni] = 1;
		}
	}

	for (int a = 0; a < N; a++) {
		for (int hi = 0; hi < powW[N]; hi += powW[a+1]) {
			for (int lo = 0; lo < powW[a]; lo++) {
				auto cpi = vec[lo + hi + powW[a] * 0];
				auto cpj = vec[lo + hi + powW[a] * 1];
				auto cpk = vec[lo + hi + powW[a] * 2];
				auto cnk = vec[lo + hi + powW[a] * 3];
				auto cnj = vec[lo + hi + powW[a] * 4];
				auto cni = vec[lo + hi + powW[a] * 5];

				auto ti = cpi + cni;
				auto tj = cpj + cnj;
				auto tk = cpk + cnk;

				vec[lo + hi + powW[a] * 0] = ti + tj + tk;
				vec[lo + hi + powW[a] * 1] = -ti - tj + tk;
				vec[lo + hi + powW[a] * 2] = -ti + tj - tk;
				vec[lo + hi + powW[a] * 3] = +ti - tj - tk;
				vec[lo + hi + powW[a] * 4] = cpi - cni;
				vec[lo + hi + powW[a] * 5] = cpj - cnj;
				vec[lo + hi + powW[a] * 6] = cpk - cnk;
			}
		}
	}

	for (int a = 0; a < powW[N]; a++) {
		cerr << a << ' ' << vec[a] << '\n';
	}

	// we now have to iterate over pow8
	num ans = 0;
	std::y_combinator([&](auto self, int cur, int ca, int cb, int cc, num w) -> void {
		if (cur == N) {
			cerr << ca << ' ' << cb << ' ' << cc << ' ' << w << ' ' << vec[ca] * vec[cb] * vec[cc] << '\n';
			num r = vec[ca] * vec[cb] * vec[cc];
			ans += r * w;
			return;
		}
		self(cur + 1, ca * W + 0, cb * W + 0, cc * W + 0, w);
		self(cur + 1, ca * W + 1, cb * W + 1, cc * W + 1, w);
		self(cur + 1, ca * W + 2, cb * W + 2, cc * W + 2, w);
		self(cur + 1, ca * W + 3, cb * W + 3, cc * W + 3, w);

		self(cur + 1, ca * W + 4, cb * W + 5, cc * W + 6, 4 * w);
		self(cur + 1, ca * W + 5, cb * W + 6, cc * W + 4, 4 * w);
		self(cur + 1, ca * W + 6, cb * W + 4, cc * W + 5, 4 * w);

		self(cur + 1, ca * W + 6, cb * W + 5, cc * W + 4, -4 * w);
		self(cur + 1, ca * W + 5, cb * W + 4, cc * W + 6, -4 * w);
		self(cur + 1, ca * W + 4, cb * W + 6, cc * W + 5, -4 * w);
	})(0, 0, 0, 0, num(1));
	for (int z = 0; z < N; z++) ans /= num(8);
	cout << ans << '\n';

	return 0;
}
