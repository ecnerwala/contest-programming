#include<bits/stdc++.h>
using namespace std;

using ll = long long;

template <int MOD_> struct modnum {
	static const int MOD = MOD_;

private:
	int v;
public:
	modnum() : v(0) {}
	modnum(int v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	friend std::ostream& operator << (std::ostream& o, const modnum& m) { return o << m.v; }

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

	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }

	friend bool operator < (const modnum& a, const modnum& b) { return a.v < b.v; }
};
const ll MOD = int(1e9) + 7;
using num = modnum<MOD>;

struct maxcounter {
	ll v = -1;
	num cnt = 0;

	void update(const maxcounter& o) {
		if (o.v > v) {
			v = o.v;
			cnt = 0;
		}
		if (o.v == v) {
			cnt += o.cnt;
		}
	}
};

const int MAXN = 1.1e5;
const int MAXM = 2.1e5;
const int MAXK = 110;
int N, M;
int K, T;

int W[MAXN];
int V[MAXN];
maxcounter dpFor[MAXN][MAXK];
maxcounter dpBack[MAXN][MAXK];
vector<int> outEdges[MAXN];
int inDeg[MAXN];

int topoInd[MAXN]; // node to index
int toposort[MAXN]; // index to node

pair<ll, num> ans[MAXN];

void go() {
	cin >> N >> M;
	for (int i = 1; i <= N; i++) {
		cin >> W[i] >> V[i];
	}
	for (int i = 1; i <= N; i++) {
		outEdges[i].clear();
		inDeg[i] = 0;
	}
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v;
		outEdges[u].push_back(v);
		inDeg[v]++;
	}


	{ // toposort
		int vert = 0;
		stack<int> q;
		for (int i = 1; i <= N; i++) {
			if (inDeg[i] == 0) q.push(i);
		}
		while (!q.empty()) {
			int cur = q.top(); q.pop();
			vert++;
			toposort[vert] = cur;
			topoInd[cur] = vert;
			for (int nxt : outEdges[cur]) {
				assert(inDeg[nxt] > 0);
				inDeg[nxt] --;
				if (inDeg[nxt] == 0) {
					q.push(nxt);
				}
			}
		}
		assert(vert == N);
	}

	cin >> K >> T;
	for (int i = 1; i <= N; i++) {
		W[i] %= K;
	}
	for (int i = 1; i <= N; i++) {
		for (int k = 0; k < K; k++) {
			dpFor[i][k] = {-1, 0};
			dpBack[i][k] = {-1, 0};
		}
	}
	dpFor[1][W[1] % K] = {V[1], 1};
	dpBack[N][W[N] % K] = {V[N], 1};

	for (int ind = 1; ind <= N; ind++) {
		int cur = toposort[ind];
		for (int nxt : outEdges[cur]) {
			for (int k = 0; k < K; k++) {
				if (dpFor[cur][k].v < 0) continue;

				int nk = (k + W[nxt]) % K;

				maxcounter mc = dpFor[cur][k];
				mc.v += V[nxt];
				dpFor[nxt][nk].update(mc);
			}
		}
	}

	for (int ind = N; ind >= 1; ind--) {
		int cur = toposort[ind];
		for (int prv : outEdges[cur]) {
			for (int k = 0; k < K; k++) {
				if (dpBack[prv][k].v < 0) continue;

				int nk = (k + W[cur]) % K;
				maxcounter mc = dpBack[prv][k];
				mc.v += V[cur];
				dpBack[cur][nk].update(mc);
			}
		}
	}

	priority_queue<pair<int, pair<ll, num>>, vector<pair<int, pair<ll, num>>>, greater<pair<int, pair<ll, num>>>> rem;
	map<ll, pair<num, int>> mp;
	for (int a = 1; a <= N; a++) {
		while (!rem.empty() && rem.top().first <= a) {
			assert(rem.top().first == a);
			auto val = rem.top().second;
			rem.pop();

			auto it = mp.find(val.first);
			assert(it != mp.end());
			it->second.first -= val.second;
			it->second.second --;
			if (it->second.second == 0) {
				mp.erase(it);
			}
		}

		int cur = toposort[a];

		if (mp.empty()) {
			ans[cur] = {-1, 0};
		} else {
			auto it = mp.rbegin();
			ans[cur] = {it->first, it->second.first};
		}

		for (int nxt : outEdges[cur]) {
			maxcounter emax;
			int b = topoInd[nxt];
			for (int ka = 0; ka < K; ka++) {
				int kb = T - ka;
				if (kb < 0) kb += K;

				if (dpFor[cur][ka].v < 0) continue;
				if (dpBack[nxt][kb].v < 0) continue;

				maxcounter mc;
				mc.v = dpFor[cur][ka].v + dpBack[nxt][kb].v;
				mc.cnt = dpFor[cur][ka].cnt * dpBack[nxt][kb].cnt;
				emax.update(mc);

			}

			if (emax.v < 0) continue;
			auto& it = mp[emax.v];
			it.first += emax.cnt;
			it.second ++;
			rem.emplace(b, pair<ll, num>{emax.v, emax.cnt});
		}
	}

	if (topoInd[1] <= topoInd[N]) {
		for (int i = 1; i <= N; i++) {
			if (topoInd[i] < topoInd[1] || topoInd[i] > topoInd[N]) {
				ans[i] = {dpFor[N][T].v, dpFor[N][T].cnt};
			}
		}
	}

	for (int i = 1; i <= N; i++) {
		if (ans[i].first == -1) {
			cout << -1 << '\n';
		} else {
			cout << ans[i].first << ' ' << ans[i].second << '\n';
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int NCASES; cin >> NCASES;
	while (NCASES--) {
		go();
	}

	return 0;
}
