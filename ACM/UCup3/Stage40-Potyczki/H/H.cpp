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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<int(1e9)+7>;
	const int MAXN = int(1e6) + 20;
	std::vector<num> fact(MAXN+1);
	fact[0] = 1;
	for (int i = 1; i <= MAXN; i++) fact[i] = fact[i-1] * num(i);
	std::vector<num> ifact(MAXN+1);
	ifact[MAXN] = inv(fact[MAXN]);
	for (int i = MAXN; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	auto choose = [&](int n, int r) -> num {
		if (0 <= r && r <= n) return fact[n] * ifact[r] * ifact[n-r];
		else return 0;
	};

	int T; cin >> T;
	while (T--) {
		int N, A, B; cin >> N >> A >> B;
		assert(A != B);
		if (A > B) std::swap(A, B);
		assert(1 <= A && A < B && B <= N);

		auto solve_pair = [&](int ca, int cb) -> num {
			assert(ca >= 1 && cb >= 1);
			num v = 1;
			v *= num(ca);
			v *= num(cb);
			ca--, cb--;
			int num_forbidden = B - A - 1;
			v *= choose(N - 2 - cb, num_forbidden) * fact[N - 2 - num_forbidden] * fact[num_forbidden];
			return v;
		};
		auto solve_val = [&](int l) -> num {
			assert(l >= 1);
			if (l == 1) return 0;
			return solve_pair(l / 2, (l+1) / 2) + solve_pair((l+1) / 2, l / 2);
		};

		num ans = 0;
		for (int b = 0; (N >> b) >= 1; b++) {
			int v_floor = (N >> b);
			int v_ceil = v_floor + 1;
			int num_ceil = N & ((1 << b) - 1);
			int num_floor = (1 << b) - num_ceil;
			assert(num_floor);
			ans += num(num_floor) * solve_val(v_floor);
			if (num_ceil) ans += num(num_ceil) * solve_val(v_ceil);
		}
		cout << ans << '\n';
	}

	return 0;
}
