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

const int MAXN = 1010;
const int MAXM = 1010;
int N, M;
char G[MAXN][MAXM];

num dp[MAXN*MAXM];

int par[MAXN*MAXM];
void reset() { memset(par, -1, sizeof(par)); }
int getPar(int a) { return par[a] < 0 ? a : (par[a] = getPar(par[a])); }
// a becomes parent
bool merge(int a, int b) { a = getPar(a), b = getPar(b); if (a == b) return false; par[b] = a; return true; }

int main() {
	if (fopen("cave.in", "r")) {
		freopen("cave.in", "r", stdin);
		freopen("cave.out", "w", stdout);
	}
	cin >> N >> M; N -= 2, M -= 2;
	if (N <= 0 || M <= 0) {
		cout << 1 << '\n';
		exit(0);
	}

	for (int i = 0; i <= N+1; i++) {
		cin >> G[i];
	}

	num ans = 1;

	reset();
	for (int i = N; i >= 0; i--) {
		for (int j = 1; j <= M; j++) {
			if (G[i][j] == '.' && G[i+1][j] == '.') {
				merge(i*M+j, (i+1)*M+j);
			}
			if (G[i][j-1] == '.' && G[i][j] == '.') {
				merge(i*M+j-1, i*M+j);
			}
		}

		for (int j = 1; j <= M; j++) {
			if (G[i][j] == '.') {
				dp[i*M+j] = 1;
			}
		}

		for (int j = 1; j <= M; j++) {
			if (G[i+1][j] == '.') {
				if (par[(i+1)*M+j] < 0) {
					ans *= dp[(i+1)*M+j];
				} else {
					dp[getPar((i+1)*M+j)] *= dp[(i+1)*M+j];
				}
			}
		}

		for (int j = 1; j <= M; j++) {
			if (G[i][j] == '.') {
				if (par[i*M+j] < 0) {
					dp[i*M+j] ++;
				}
			}
		}
	}

	cout << ans << '\n';

	return 0;
}
