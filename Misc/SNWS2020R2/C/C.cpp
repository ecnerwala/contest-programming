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
using num = modnum<998244353>;

const int MAXN = 1100;
int N;
string S;

num dp[MAXN][2]; // whether it's touched the bottom
num ndp[MAXN][2];

num solve() {
	// 'm' is open and 'w' is close
	memset(dp, 0, sizeof(dp));
	dp[0][true] = 1;
	for (int i = 0; i < N; i++) {
		memset(ndp, 0, sizeof(ndp));
		for (int v = 0; v <= i; v++) {
			for (int z = 0; z < 2; z++) {
				if (S[i] == 'm') {
					// never remove
					ndp[v+1][z] += dp[v][z];
				} else if (S[i] == 'w') {
					// don't remove it
					if (v >= 1) {
						ndp[v-1][z || (v-1 == 0)] += dp[v][z];
					}

					// remove it
					ndp[v][false || v==0] += dp[v][z];
				} else assert(false);
			}
		}
		memcpy(dp, ndp, sizeof(ndp));
	}
	num ans = 0;
	for (int v = 0; v <= N; v++) {
		ans += dp[v][true];
	}
	//cerr << "ans " << ans << '\n';
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> S;
	N = int(S.size());
	num ans = solve();
	reverse(S.begin(), S.end());
	for (char& v : S) {
		v ^= 'm' ^ 'w';
	}
	ans *= solve();
	cout << ans << '\n';

	return 0;
}
