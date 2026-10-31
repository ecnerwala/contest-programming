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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<998244353>;

	int N; cin >> N;
	vector<string> G(N); for (auto& r : G) cin >> r;

	mt19937 mt(48);

	vector<vector<num>> dp(N, vector<num>(N, num(0)));

	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			if (G[i][j] == '0') {
				assert(G[j][i] == '0');
				num r = num(mt());
				dp[i][j] = r;
				dp[j][i] = -r;
			}
		}
	}

	for (int i = 0; i < N; i++) {
		int j = 0;
		while (j < N && dp[i][j] == 0) j++;
		if (j == N) continue;

		{
			num f = inv(dp[i][j]);
			for (int v = j; v < N; v++) dp[i][v] *= f;
			assert(dp[i][j] == 1);
		}
		for (int k = 0; k < N; k++) {
			if (k == i) continue;
			num f = dp[k][j];
			for (int v = j; v < N; v++) dp[k][v] -= dp[i][v] * f;
			assert(dp[k][j] == 0);
		}
	}

	string ans(N, '1');
	for (int i = 0; i < N; i++) {
		int j = 0;
		while (j < N && dp[i][j] == 0) j++;
		if (j == N) continue;
		bool is_good = true;
		for (int v = j+1; v < N; v++) {
			if (dp[i][v] != 0) {
				is_good = false;
				break;
			}
		}
		if (is_good) {
			ans[j] = '0';
		}
	}
	cout << ans << '\n';

	return 0;
}
