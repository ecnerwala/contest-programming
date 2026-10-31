#include<bits/stdc++.h>
using namespace std;

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

using num = modnum<int(1e9)+7>;

const int MAXN = 1.1e5;
const int MAXK = 12;
int N, K;
int endpoint[MAXN*2];

num choose[MAXK][MAXK];
num dp[MAXN*2][MAXK+1];

int main() {
	if (fopen("help.in", "r")) {
		freopen("help.in", "r", stdin);
		freopen("help.out", "w", stdout);
	}
	cin >> N >> K;
	for (int i = 0; i < N; i++) {
		int l, r; cin >> l >> r;
		assert(l < r);
		endpoint[l] = r;
		endpoint[r] = l;
	}

	for (int i = 0; i <= K; i++) {
		choose[i][0] = choose[i][i] = 1;
		for (int j = 1; j < i; j++) {
			choose[i][j] = choose[i-1][j-1] + choose[i-1][j];
		}
	}

	dp[0][0] = 1;

	for (int i = 1; i <= 2*N; i++) {
		if (endpoint[i] < i) {
			// allow ourselves to take this
			for (int k2 = K; k2 >= 0; k2--) {
				for (int k1 = 0; k1 < k2; k1++) {
					dp[i][k2] += dp[i][k1] * choose[k2][k1];
				}
			}
		} else {
			int j = endpoint[i];
			for (int k = 0; k <= K; k++) {
				assert(dp[j][k] == 0);
				for (int v = 0; v < j; v++) {
					dp[j][k] += dp[v][k];
				}
				for (int v = j+1; v <= 2*N; v++) {
					dp[v][k] *= 2;
				}
			}
		}
	}

	num ans = 0;
	for (int i = 0; i <= 2*N; i++) {
		ans += dp[i][K];
	}
	cout << ans << '\n';

	return 0;
}
