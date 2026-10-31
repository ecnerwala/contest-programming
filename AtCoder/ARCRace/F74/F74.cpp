#include<bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

const int MAXH = 1.1e3;
const int MAXW = 1.1e3;
int H, W;
char A[110][110];

struct Dinic {
	struct Edge {
		int to, rev;
		ll c, f;
	};
	vi lvl, ptr, q;
	vector<vector<Edge>> adj;
	Dinic(int n) : lvl(n), ptr(n), q(n), adj(n) {}
	void addEdge(int a, int b, ll c, int rcap = 0) {
		adj[a].push_back({b, sz(adj[b]), c, 0});
		adj[b].push_back({a, sz(adj[a]) - 1, rcap, 0});
	}
	ll dfs(int v, int t, ll f) {
		if (v == t || !f) return f;
		for (int& i = ptr[v]; i < sz(adj[v]); i++) {
			Edge& e = adj[v][i];
			if (lvl[e.to] == lvl[v] + 1)
				if (ll p = dfs(e.to, t, min(f, e.c - e.f))) {
					e.f += p, adj[e.to][e.rev].f -= p;
					return p;
				}
		}
		return 0;
	}
	ll calc(int s, int t) {
		ll flow = 0; q[0] = s;
		rep(L,0,31) do { // 'int L=30' maybe faster for random data
			lvl = ptr = vi(sz(q));
			int qi = 0, qe = lvl[s] = 1;
			while (qi < qe && !lvl[t]) {
				int v = q[qi++];
				trav(e, adj[v])
					if (!lvl[e.to] && (e.c - e.f) >> (30 - L))
						q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
			}
			while (ll p = dfs(s, t, LLONG_MAX)) flow += p;
		} while (lvl[t]);
		return flow;
	}
};

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> H >> W;
	for (int i = 0; i < H; i++) cin >> A[i];
	Dinic mf(H + W + 2);
	const ll INF = 1e9;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (A[i][j] == 'S') {
				mf.addEdge(H + W, i, INF);
				mf.addEdge(H + W, H + j, INF);
			} else if (A[i][j] == 'T') {
				mf.addEdge(i, H + W + 1, INF);
				mf.addEdge(H + j, H + W + 1, INF);
			} else if (A[i][j] == 'o') {
				mf.addEdge(i, H + j, 1, 1);
			} else if (A[i][j] == '.') {
			} else assert(false);
		}
	}
	ll ans = mf.calc(H + W, H + W + 1);
	if (ans >= INF) {
		cout << -1 << '\n';
	} else {
		cout << ans << '\n';
	}

	return 0;
}
