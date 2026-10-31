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

const num inv2 = inv(num(2));

int main() {
	if (fopen("sprinklers2.in", "r")) {
		freopen("sprinklers2.in", "r", stdin);
		freopen("sprinklers2.out", "w", stdout);
	}
	int N; cin >> N;
	int M = N;
	vector<string> G(N);
	for (int i = 0; i < N; i++) {
		cin >> G[i];
		assert(int(G[i].size()) == M);
	}

	num base = 1;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (G[i][j] == '.') base += base;
		}
	}

	// number of corn
	vector<num> dp(M+1);
	for (int j = 0; j <= M; j++) {
		if (j == 0) {
			dp[j] += base;
		} else if (G[0][j-1] == '.') {
			dp[j] += base * inv2;
		}
	}

	vector<num> ndp(M+1);
	for (int i = 1; i < N; i++) {
		fill(ndp.begin(), ndp.end(), 0);
		num prefSum = 0;
		for (int j = 0; j <= M; j++) {
			ndp[j] += dp[j];
			if (j > 0 && G[i][j-1] == '.') {
				ndp[j] += prefSum * inv2;
			}
			if (j < M && G[i-1][j] == '.') {
				prefSum += dp[j] * inv2;
			}
		}
		swap(dp, ndp);
	}

	num ans = 0;
	for (int j = 0; j <= M; j++) {
		if (j == M) {
			ans += dp[j];
		} else if (G[N-1][j] == '.') {
			ans += dp[j] * inv2;
		}
	}
	cout << ans << '\n';

	return 0;
}
