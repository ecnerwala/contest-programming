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
using ll = long long;
const ll INF = 1e18;

const int MAXN = 2.1e5;
const int MAXM = 4.1e5;

int N, M;
vector<pair<int, int>> gadj[MAXN];
ll W[MAXM];
pair<int, int> U[MAXM];

pair<ll, int> cnds[MAXM];

int pred[2*MAXM];
int succ[2*MAXM];

struct dsu {
	vector<int> par, sz;
	dsu(int n) : par(n, -1), sz(n, 1) {}
	int getPar(int a) { return par[a] == -1 ? a : (par[a] = getPar(par[a])); }
	num merge(int a, int b) {
		a = getPar(a), b = getPar(b);
		if (a == b) return num(0);
		if (sz[a] < sz[b]) swap(a,b);
		par[b] = a;
		sz[a] += sz[b];
		return num(sz[a] - sz[b]) * num(sz[b]);
	}
};

ll dp[MAXM*2];

void dfsDown(int e) {
	if (pred[e^1] == -1) {
		dp[e] = 0;
		return;
	}
	dp[e] = INF;
	for (int cur = succ[e^1]; cur != (e^1); cur = succ[cur]) {
		dfsDown(cur);
		dp[e] = min(dp[e], dp[cur] + W[cur/2]);
	}
}

void dfsUp(int e) {
	if (pred[e^1] == -1) {
		return;
	}
	ll best = dp[e^1] + W[e/2], best2 = INF;
	for (int cur = succ[e^1]; cur != (e^1); cur = succ[cur]) {
		ll cnd = dp[cur] + W[cur/2];
		if (cnd < best) {
			best2 = best;
			best = cnd;
		} else if (cnd < best2) {
			best2 = cnd;
		}
	}
	for (int cur = succ[e^1]; cur != (e^1); cur = succ[cur]) {
		ll cnd = dp[cur] + W[cur/2];
		dp[cur^1] = (cnd == best) ? best2 : best;
		dfsUp(cur);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	assert(N >= 3);

	for (int e = 0; e < M; e++) {
		int u, v; ll w; cin >> u >> v >> w; u--, v--;
		assert(u != v);
		if (u > v) swap(u, v);
		assert(u < v);

		W[e] = w;
		U[e] = {u, v};
		gadj[u].push_back({v-u, 2 * e});
		gadj[v].push_back({u-v + N, 2 * e + 1});
	}

	for (int i = 0; i < N; i++) {
		sort(gadj[i].begin(), gadj[i].end());
		assert(int(gadj[i].size()) >= 2);
		assert(gadj[i].front().first == 1);
		assert(gadj[i].back().first == N-1);
		pred[gadj[i].front().second ^ 1] = -1;
		for (int z = 0; z+1 < int(gadj[i].size()); z++) {
			int x = gadj[i][z].second, y = gadj[i][z+1].second ^ 1;
			succ[x] = y; pred[y] = x;
		}
		succ[gadj[i].back().second] = -1;
	}

	dfsDown(0);
	dfsDown(1);
	dfsUp(0);
	dfsUp(1);

	for (int e = 0; e < M; e++) {
		cnds[e] = {dp[2 * e] + dp[2 * e + 1] + W[e], e};
	}
	sort(cnds, cnds + M);
	reverse(cnds, cnds + M);

	num ans = 0;
	dsu d2(N);
	for (int z = 0; z < M; z++) {
		int e = cnds[z].second;
		ans += num(cnds[z].first) * d2.merge(U[e].first, U[e].second);
	}

	cout << ans << '\n';

	return 0;
}
