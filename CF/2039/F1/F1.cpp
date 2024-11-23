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

	// dp[L] = 1 + 2 * sum(2 <= v <= L) sum(1 <= a <= L, gcd(a, v) == 1) dp[L / v]
	// dp[L] = 1 + 2 * sum(1 <= g <= L) mu(g) * (L/g) * sum(1 <= v <= L/g) dp[L / g / v] excluding v=g=1
	// dp[L] = 1 + 2 * sum(1 <= g <= L) mu(g) * (L/g) * ans[L/g]
	// delta_dp[L] = 2 * sum(g | L) * mu[g] * ((L/g)ans[L/g] - (L/g-1)ans[L/g-1])
	//const int X = int(1e6) + 10;
	const int X = 10;
	using num = modnum<998244353>;
	std::vector<num> mu(X+1);
	std::vector<num> dp(X+1);
	std::vector<num> delta_dp(X+1);
	std::vector<num> ans(X+1);
	std::vector<num> delta_ans(X+1);
	delta_dp[1] += num(1);
	mu[1] += num(1);
	for (int i = 1; i <= X; i++) {
		{
			num delta_val = num(i) * (delta_ans[i] + ans[i-1] - dp[i-1]) - num(i-1) * ans[i-1];
			delta_dp[i] += 2 * delta_val;
		}
		cerr << i << ' ' << mu[i] << ' ' << delta_dp[i] << ' ' << dp[i] << ' ' << ans[i] << '\n';
		dp[i] = delta_dp[i] + dp[i-1];
		delta_ans[i] += delta_dp[i];
		ans[i] = delta_ans[i] + ans[i-1];
		num delta_val = num(i) * ans[i] - num(i-1) * ans[i-1];
		for (int j = 2; i * j <= X; j++) {
			delta_dp[i*j] += mu[j] * 2 * delta_val;
			delta_ans[i*j] += delta_dp[i];
			mu[i*j] -= mu[i];
		}
		cerr << i << ' ' << mu[i] << ' ' << delta_dp[i] << ' ' << dp[i] << ' ' << ans[i] << '\n';
	}

	int T; cin >> T;
	while (T--) {
		int N; cin >> N; cout << ans[N] << '\n';
	}

	return 0;
}
