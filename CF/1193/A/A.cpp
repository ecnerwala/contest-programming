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
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}

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

const int MAXN = 18;
int N;
num dp[1<<MAXN];
uint32_t adj[MAXN];

uint32_t adjs[1<<MAXN];

num getLegal() {
	adjs[0] = 0;
	for (int i = 0; i < N; i++) {
		for (uint32_t m = 0; m < (1u << i); m++) {
			adjs[m | (1 << i)] = adjs[m] | adj[i];
		}
	}

	dp[0] = 1;
	for (uint32_t m = 0; m < (1u << N); m++) {
		uint32_t nxt = (m == 0) ? ((1u << N)-1) : (adjs[m] ^ (adjs[m] & m));
		for (uint32_t v = nxt; v; v = (v-1) & nxt) {
			if (!(adjs[v] & v)) {
				dp[m | v] += dp[m];
			}
		}
	}
	return dp[(1 << N) - 1];
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	int M; cin >> M;
	for (int e = 0; e < M; e++) {
		int a, b; cin >> a >> b; a--, b--;
		adj[a] |= (1 << b);
		adj[b] |= (1 << a);
	}

	num ans = getLegal();
	cerr << ans << '\n';
	ans *= num(M);
	ans /= num(2);

	cout << ans << '\n';

	return 0;
}
