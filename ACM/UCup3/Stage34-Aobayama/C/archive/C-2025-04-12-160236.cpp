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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	using num = modnum<998244353>;

	const int B = 30;
	const int L = 7;

	std::vector<num> p2(B+1);
	p2[0] = 0;
	for (int i = 0; i < B; i++) p2[i+1] = p2[i] + p2[i];

	std::vector<num> precomp_den(1 << (B - L));
	precomp_den[0] = 1;
	for (int b = B-1; b >= L; b--) {
		num c = p2[b];
		for (int i = 0; i < (1 << (B - 1 - b)); i++) {
			precomp_den[(2 * i + 1) << (b - L)] += c * precomp_den[(2 * i) << (b - L + 1)];
		}
	}
	std::vector<num> numer; numer.push_back(1);
	for (int b = L-1; b > 0; b--) {
		// first, square precomp_numer 
		{
			std::vector<num> n_numer(2 * numer.size() - 1);
			for (int i = 0; i < int(numer.size()); i++) {
				n_numer[2*i] = numer[i];
			}
			numer = n_numer;
		}
		// now multiply by (1 - (2^b)^{2^(L-b)} x^{2^L}) / (1 - 2^b x^{2^b})
		{
			numer.resize(int(numer.size()) + (1 << (L-b)), num(0));
			num c = pow(p2[b], 1 << (L-b));
			for (int i = int(numer.size()) - 1 - (1 << (L-b)); i >= 0; i--) {
				numer[i] -= c * numer[i - (1 << (L-b))];
			}
			// divide den by it too
			for (int i = 0; i+1 < (1 << (B-L)); i++) {
				precomp_den[i+1] += c * precomp_den[i];
			}
		}
		{
			// now divide by 1 - 2^b x^{2^b}
			num c = p2[b];
			for (int i = 0; i+1 < int(numer.size()); i++) {
				numer[i+1] += c * numer[i];
			}
			assert(numer.back() == 0);
			numer.pop_back();
		}
	}

	int T; cin >> T;
	uint32_t a, b; cin >> a >> b;
	int64_t tot_ans = 0;
	for (int c = 0; c < T; c++) {
		int N = int((uint32_t(c) * a + b) & ((1 << B) - 1));
		num ans = 0;
		for (int i = N & ((1 << L) - 1); i < int(numer.size()) && i <= N; i += (1 << L)) {
			ans += numer[i] * precomp_den[(N - i) >> L];
		}
		tot_ans += int(ans) ^ c;
	}
	cout << tot_ans << '\n';

	return 0;
}
