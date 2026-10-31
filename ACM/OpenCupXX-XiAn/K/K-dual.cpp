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

const int MAXV = MAXN * 2;

int N, M;
vector<pair<int, int>> gadj[MAXN];
ll W[MAXM];
pair<int, int> U[MAXM];

int V;
vector<pair<int, ll>> adj[MAXV];

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

pair<ll, int> dpDown[MAXV];
pair<ll, int> dpUp[MAXV];
vector<pair<ll, int>> mst;

void dfsDown(int cur, int prv) {
	if (adj[cur].size() == 1) {
		assert(prv != -1);
		// -1 to account for the +1 on the weight
		dpDown[cur] = {-1, cur};
		return;
	}
	dpDown[cur] = {INF, -1};
	for (auto e : adj[cur]) {
		int nxt = e.first;
		if (nxt == prv) continue;

		dfsDown(nxt, cur);
		auto cnd = dpDown[nxt];
		cnd.first += W[e.second];
		if (cnd < dpDown[cur]) {
			dpDown[cur] = cnd;
		}
	}
}

void dfsUp(int cur, int prv) {
	if (adj[cur].size() == 1) {
		assert(prv != -1);
		return;
	}
	pair<ll, int> best(INF, -1), best2(INF, -1);
	for (auto e : adj[cur]) {
		int nxt = e.first;
		pair<ll, int> cnd = (nxt == prv) ? dpUp[cur] : dpDown[nxt];
		cnd.first += W[e.second];
		if (cnd < best) {
			best2 = best;
			best = cnd;
		} else if (cnd < best2) {
			best2 = cnd;
		}
	}

	for (auto e : adj[cur]) {
		int nxt = e.first;
		if (nxt == prv) continue;
		if (dpDown[nxt].second == best.second) {
			dpUp[nxt] = best2;
		} else {
			dpUp[nxt] = best;
		}

		auto v1 = dpUp[nxt], v2 = dpDown[nxt];
		assert(v1.second != v2.second);
		if (v1 > v2) swap(v1, v2);
		assert(v1 < v2);
		v1.first += W[e.second];
		if (v1 > v2) {
			mst.push_back({v1.first + v2.first, e.second});
		}

		dfsUp(nxt, cur);
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

		if (v-u == 1 || v-u == N-1) {
			w++;
		}
		if (w == 0) {
			e--;
			M--;
			continue;
		}

		W[e] = w;
		U[e] = {u, v};
		gadj[u].push_back({v-u, 2 * e});
		gadj[v].push_back({u-v + N, 2 * e + 1});
	}

	dsu dsds(2*M);
	vector<int> ccId(2*M, -1);
	for (int i = 0; i < N; i++) {
		sort(gadj[i].begin(), gadj[i].end());
		assert(int(gadj[i].size()) >= 2);
		assert(gadj[i].front().first == 1);
		assert(gadj[i].back().first == N-1);
		for (int z = 0; z+1 < int(gadj[i].size()); z++) {
			dsds.merge(gadj[i][z].second, gadj[i][z+1].second ^ 1);
		}
		ccId[gadj[i].back().second] = i;
	}

	V = N;
	for (int e = 0; e < 2*M; e++) {
		if (dsds.getPar(e) == e) {
			if (dsds.sz[e] == 1) {
				// do nothing
			} else {
				ccId[e] = V++;
			}
		}
	}

	assert(V == M+1);
	for (int e = 0; e < M; e++) {
		int a = ccId[dsds.getPar(2 * e)];
		int b = ccId[dsds.getPar(2 * e + 1)];
		//cerr << a << ' ' << b << ' ' << W[e] << '\n';
		adj[a].push_back({b, e});
		adj[b].push_back({a, e});
	}

	dfsDown(N, -1);
	dfsUp(N, -1);

	assert(int(mst.size()) == N-1);

	sort(mst.begin(), mst.end());
	//for (auto it : mst) { cerr << it.first << ' ' << U[it.second].first << ' ' << U[it.second].second << '\n'; }
	reverse(mst.begin(), mst.end());

	num ans = 0;
	dsu d2(N);
	for (auto it : mst) {
		ans += num(it.first) * d2.merge(U[it.second].first, U[it.second].second);
	}

	cout << ans << '\n';

	return 0;
}
