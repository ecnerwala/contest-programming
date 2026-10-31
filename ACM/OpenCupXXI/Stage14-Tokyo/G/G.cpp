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

using num = modnum<998244353>;

const num gen = 3;
const num w7 = pow(gen, (num::MOD-1) / 7);

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; int64_t K; cin >> N >> K;
	array<int, 8> p7;
	p7[0] = 1;
	for (int i = 1; i <= 7; i++) p7[i] = p7[i-1] * 7;

	array<num, 7> rts;
	rts[0] = 1;
	for (int i = 1; i < 7; i++) rts[i] = rts[i-1] * w7;

	vector<num> dp(p7[7]);
	for (int i = 0; i < N; i++) {
		int a; cin >> a;
		int a7 = 0;
		for (int z = 0; z < 7; z++) {
			a7 += ((a >> z) & 1) * p7[z];
		}
		dp[a7]++;
	}

	array<array<num, 7>, 7> mat;
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			mat[i][j] = rts[i*j%7];
		}
	}

	for (int b = 0; b < 7; b++) {
		for (int hi = 0; hi < p7[7]; hi += p7[b+1]) {
			for (int lo = 0; lo < p7[b]; lo++) {
				array<num, 7> nxt; nxt.fill(0);
				for (int d = 0; d < 7; d++) {
					num v = dp[hi + lo + d * p7[b]];
					for (int i = 0; i < 7; i++) {
						nxt[i] += v * mat[d][i];
					}
				}
				for (int d = 0; d < 7; d++) {
					dp[hi + lo + d * p7[b]] = nxt[d];
				}
			}
		}
	}

	for (int v = 0; v < p7[7]; v++) {
		dp[v] = pow(dp[v], K);
	}

	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 7; j++) {
			mat[i][j] = inv(mat[i][j]) / num(7);
		}
	}

	for (int b = 0; b < 7; b++) {
		for (int hi = 0; hi < p7[7]; hi += p7[b+1]) {
			for (int lo = 0; lo < p7[b]; lo++) {
				array<num, 7> nxt; nxt.fill(0);
				for (int d = 0; d < 7; d++) {
					num v = dp[hi + lo + d * p7[b]];
					for (int i = 0; i < 7; i++) {
						nxt[i] += v * mat[d][i];
					}
				}
				for (int d = 0; d < 7; d++) {
					dp[hi + lo + d * p7[b]] = nxt[d];
				}
			}
		}
	}

	cout << dp[0] << '\n';

	return 0;
}
