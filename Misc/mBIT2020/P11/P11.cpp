#include <bits/stdc++.h>

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

	int N, Q; cin >> N >> Q;
	using num = modnum<int(1e9)+7>;
	vector<num> idp(N);
	vector<num> idp_pref(N+1);
	num p2Q1 = pow(inv(num(2)), Q-1);
	for (int i = 0; i < N; i++) {
		if (i < Q-1) {
			idp[i] = 1;
		} else {
			idp[i] = idp_pref[i] - idp_pref[i-(Q-1)] * p2Q1;
		}
		idp_pref[i+1] = (idp[i] + idp_pref[i]) / num(2);
	}
	cout << 1/idp[N-1] << '\n';

	return 0;
}

// If you have seen k in a row, and there are t turns left,
//  dp[*][0] = 1
//  dp[Q-1][t] = 2 * dp[1][t-1]
//  dp[k][t] = max_x( min(dp[k+1][t-1] * (1-x), dp[1][t-1] * (1+x)) )
//  x = (dp[k+1][t-1] - dp[k][t-1]) / (dp[k+1][t-1] + dp[1][t-1])
//  dp[k][t] = 2 * dp[k+1][t-1] * dp[1][t-1] / (dp[k+1][t-1] + dp[1][t-1])
//
//  => 1 / dp[k][t] = 1/2 (1 / dp[k+1][t-1] + 1 / dp[1][t-1])
//
//  idp[Q][t] = 0
//  idp[k][t] = 1/2 (idp[k+1][t-1] + idp[1][t-1])
//  idp[k][t] = 1/2 (1/2(idp[k+2][t-2] + idp[1][t-2]) + idp[1][t-1])
//
//  idp[1][0..Q-2] = 1
//  idp[1][t] = 1/2 idp[1][t-1] + 1/4 idp[1][t-2] + ... + 1/2^(Q-1) * idp[1][t-(Q-1)]
//
//  looking for idp[1][N-1]
//
//  t = 0: 1, 1, 1, 1, 1, 1, 0
//  t = 1: 1, 1, 1, 1, 1, 1/2, 0
//  t = 2: 1, 1, 1, 1, 3/4, 1/2, 0
//  t = 3: 1, 1, 1, 7/8, 3/4, 1/2, 0
//  t = 4: 1, 1, 1, 7/8, 3/4, 1/2, 0
