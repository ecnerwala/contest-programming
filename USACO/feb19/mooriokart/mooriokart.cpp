#include<bits/stdc++.h>
using namespace std;

using ll = long long;

ll modinv(ll a, ll m) {
	a %= m;
	return a == 1 ? 1 : (m - modinv(m, a) * m / a);
}

template <int MOD_> struct modnum {
	static const int MOD = MOD_;
private:
	int v;
public:
	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int () const { return v; }
	friend std::ostream& operator << (std::ostream& o, const modnum& m) { return o << m.v; }
	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum operator ~ () const {
		assert(v);
		modnum res;
		res.v = int(modinv(v, MOD));
		return res;
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
		return *this *= ~o;
	}

	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

using num = modnum<int(1e9)+7>;

const int MAXN = 1600;
const int MAXY = 2600;
int N, M;
int X, Y;

vector<pair<int, int>> adj[MAXN];
bool vis[MAXN];
struct state {
	num v[MAXY]; // number of paths a given length
	num totLong; // total of all long paths

	friend ostream& operator << (ostream& o, const state& s) {
		o << "[";
		for (int i = 0; i <= Y; i++) { o << s.v[i] << ", "; }
		o << "; " << s.totLong << "]";
		return o;
	}

	state& operator += (const state& o) {
		for (int i = 0; i <= Y; i++) {
			v[i] += o.v[i];
		}
		totLong += o.totLong;
		return *this;
	}
	state& operator -= (const state& o) {
		for (int i = 0; i <= Y; i++) {
			v[i] -= o.v[i];
		}
		totLong -= o.totLong;
		return *this;
	}
	state addEdge(int d) const {
		assert(d >= 0);

		state res;
		for (int i = 0; i < Y; i++) {
			num cnt = v[i];
			if (i + d >= Y) {
				res.totLong += cnt * (i + d);
				res.v[Y] += cnt;
			} else {
				res.v[i + d] += cnt;
			}
		}

		res.v[Y] += v[Y];
		res.totLong += d * v[Y];
		res.totLong += totLong;
		return res;
	}
};

state dp;
state dp_down[MAXN];
state dp_up[MAXN];
state ndp;

void dfs_down(int cur, int prv) {
	vis[cur] = true;
	dp_down[cur] = dp;
	for (auto it : adj[cur]) {
		int nxt = it.first;
		int d = it.second;
		if (nxt == prv) continue;
		dfs_down(nxt, cur);
		dp_up[nxt] = dp_down[nxt].addEdge(d);
		dp_down[cur] += dp_up[nxt];
	}
	//cerr << "down " << cur << ' ' << dp_down[cur] << '\n';
}

state tmp;
void dfs_up(int cur, int prv) {
	dp_up[cur] += dp_down[cur];
	//cerr << "up " << cur << ' ' << dp_up[cur] << '\n';
	ndp += dp_up[cur];
	ndp -= dp; // subtract length 0
	for (auto it : adj[cur]) {
		int nxt = it.first;
		int d = it.second;
		if (nxt == prv) continue;
		tmp = dp_up[cur];
		tmp -= dp_up[nxt];
		dp_up[nxt] = tmp.addEdge(d);
		dfs_up(nxt, cur);
	}
}

void goTree(int cur) {
	ndp = state(); // reset it
	dfs_down(cur, 0);
	dp_up[cur] = state();
	dfs_up(cur, 0);
	dp = ndp;
}

int main() {
	if(fopen("mooriokart.in", "r")) {
		freopen("mooriokart.in", "r", stdin);
		freopen("mooriokart.out", "w", stdout);
	}
	cin >> N >> M >> X >> Y;
	for (int i = 0; i < M; i++) {
		int a, b, d; cin >> a >> b >> d;
		adj[a].emplace_back(b, d);
		adj[b].emplace_back(a, d);
	}

	Y -= X * (N - M);
	Y = max(Y, 0);
	//cerr << "Y " << Y << '\n';

	dp.v[0] += 1;

	int numCC = 0;
	for (int i = 1; i <= N; i++) {
		if (!vis[i]) {
			//cerr << "Tree " << i << '\n';
			numCC++;
			goTree(i);
			//cerr << dp << '\n';
		}
	}
	assert(numCC == N - M);
	num res = dp.totLong + X * numCC * dp.v[Y];
	res /= 2; // divide by the direction
	// for each cc, you divide by 2
	for (int i = 1; i < numCC; i++) {
		res *= i;
	}
	cout << res << '\n';

	return 0;
}
