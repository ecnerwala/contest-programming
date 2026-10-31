#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
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

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

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
		v = int(ll(v) * ll(o.v) % MOD);
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

	// a sequence is nonzero if it consists of the following nonzero parts:
	//
	// an odd number of 2's, a 1, any number of 2's and 1's
	//   OR
	// an even number of 2's, no 1's, and then something >= 3
	//
	// any number of (>= 1 3's)/(>= 1 2's) sequences (equiv, either empty, or a 3, any number of 3/2's, and a 2)
	// a even number of 3's
	// any number of (>= 1 4's)/(>= 1 3's) sequences
	// a even number of 4's
	// ...

	int N, M; cin >> N >> M;

	using num = modnum<998244353>;
	std::vector<num> fact(N+1);
	fact[0] = 1;
	for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);
	std::vector<num> ifact(N+1);
	ifact[N] = inv(fact[N]);
	for (int i = N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	// dp value for just (3's/2's) and the 3's
	std::vector<num> one_val(N+1);
	one_val[0] = 1;
	if (N >= 2) {
		one_val[2] = 1;
		for (int i = 3; i <= N; i++) {
			one_val[i] = one_val[i-1] + one_val[i-1];
		}
	}
	for (int i = 2; i <= N; i++) {
		one_val[i] += one_val[i-2];
	}

	std::vector<num> dp(N+1);
	dp[0] = 1;
	for (int z = 0; z < max(M-3+1, 0); z++) {
		std::vector<num> ndp(N+1);
		for (int a = 0; a <= N; a++) {
			for (int b = 0; b <= N-a; b++) {
				ndp[a+b] += dp[a] * one_val[b];
			}
		}
		dp = std::move(ndp);
	}

	std::vector<num> dp_with_1(N+1);
	dp_with_1[0] = 1;
	for (int i = 1; i <= N; i++) {
		for (int j = 0; j <= N-i; j++) {
			dp_with_1[i+j] += fact[i-1+j] * ifact[i-1] * ifact[j] * dp[i];
		}
	}
	
	std::vector<num> dp_with_2(N+1);
	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= N-i; j += 2) {
			if (M < 2 && j > 0) {
				break;
			}
			dp_with_2[i+j] += dp_with_1[i];
			if (j >= 2) {
				num v = dp_with_1[i];
				for (int k = 0; k <= N-i-j; k++, v *= 2) {
					dp_with_2[i+j+k] += v;
				}
			}
		}
	}

	num ans = 0;
	for (int i = 0; i <= N; i++) {
		ans += dp_with_2[i] * fact[N] * ifact[N-i] * ifact[i];
	}

	cout << pow(num(M+1), N) - ans << '\n';

	return 0;
}
