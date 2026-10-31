#include<bits/stdc++.h>

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

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
		res.v = minv(v, MOD);
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
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		if (v < 0) v += MOD;
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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<998244353>;

	int N, M, T; cin >> N >> M >> T;

	array<num, 3> dp{0,0,0};
	assert(T == 1 || T == 2);
	dp[T] ++;

	num inv3 = inv(num(N) * num(N-1) * num(N-2)); // 1/N/(N-1)/(N-2)
	num inv2 = inv3 * num(N-2); // 1/N/(N-1)
	num inv1 = inv2 * num(N-1); // 1/N

	for (int op = 0; op < M; op++) {
		int L; cin >> L;
		int R = N-L;

		// dp_left(x) = dp(x)
		array<num, 3> dp_left = dp;

		// dp_right(x) = dp(x+L)
		array<num, 3> dp_right{0,0,0};

		dp_right[0] += dp[0];

		dp_right[1] += dp[1];
		dp_right[0] += dp[1] * L;
		
		dp_right[2] += dp[2];
		dp_right[1] += 2 * dp[2] * L;
		dp_right[0] += dp[2] * num(L) * num(L);

		dp = {0,0,0};

		for (int z = 0; z < 2; z++, swap(dp_left, dp_right), swap(L, R)) {
			dp[0] += dp_left[0] * num(L) * inv1;

			dp[1] += dp_left[1] * num(L) * num(L-1) * inv2;
			dp[0] += dp_left[1] * num(L) * num(R) * inv2;

			dp[2] += dp_left[2] * num(L) * num(L-1) * num(L-2) * inv3;
			dp[1] += dp_left[2] * 3 * num(L) * num(L-1) * num(R) * inv3;
			dp[0] += dp_left[2] * num(L) * num(R) * num(R-L) * inv3;
		}
	}

	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		int c; cin >> c;
		num ans = (dp[2] * c + dp[1]) * c + dp[0];
		cout << ans << '\n';
	}

	return 0;
}
