#include<bits/stdc++.h>
using namespace std;

template <int MOD> struct modnum {
	static_assert(MOD > 0, "MOD must be positive");

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

using num = modnum<int(1e9)+7>;

const int MAXN = 6000;
int N;
vector<int> adj[MAXN];
int sz[MAXN];
num dp[MAXN][MAXN];

num tmp[MAXN];

void dfs(int cur, int prv = -1) {
	sz[cur] = 1;
	dp[cur][1] ++;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfs(nxt, cur);
		for (int i = sz[cur] + sz[nxt]; i > 0; i -= 2) {
			tmp[i] = 0;
		}
		for (int i = sz[cur]; i > 0; i -= 2) {
			for (int j = sz[nxt]; j >= 0; j -= 2) {
				tmp[i+j] += dp[cur][i] * dp[nxt][j];
			}
		}
		sz[cur] += sz[nxt];
		for (int i = sz[cur]; i > 0; i -= 2) {
			dp[cur][i] = tmp[i];
		}
	}
	if (sz[cur] % 2 == 0) {
		num fac = -1;
		dp[cur][0] = 0;
		for (int i = 2; i <= sz[cur]; i += 2) {
			fac *= num(i-1);
			dp[cur][0] += fac * dp[cur][i];
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for (int i = 0; i < N-1; i++) {
		int x, y; cin >> x >> y;
		x--, y--;
		adj[x].push_back(y);
		adj[y].push_back(x);
	}
	dfs(0);
	assert(N % 2 == 0);
	assert(sz[0] == N);
	cout << -dp[0][0] <<'\n';

	return 0;
}
