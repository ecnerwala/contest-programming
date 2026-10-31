#include<bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

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
		return flow; }
};

const int MAXN = 2100;
const int MAXK = 2100;
int N, K;
int A[MAXN];
vector<int> B[MAXN];
bool inCyc[MAXN];

int C[MAXK];

const int MAXM = MAXK;
int M;
vector<int> materials;
vector<int> workers[MAXM];

pair<int, int> edge[MAXK];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K;
	for (int i = 0; i < N; i++) {
		cin >> A[i]; A[i] --;
		int s; cin >> s;
		B[i].reserve(s);
		while (s--) {
			int v; cin >> v;
			B[i].push_back(v);
		}
	}
	for (int i = 0; i < K; i++) {
		cin >> C[i];
	}
	for (int i = 0; i < K; i++) {
		materials.push_back(C[i]);
	}
	sort(materials.begin(), materials.end());
	M = int(unique(materials.begin(), materials.end()) - materials.begin());
	materials.resize(M);
	for (int i = 0; i < N; i++) {
		vector<int> nb;
		for (int v : B[i]) {
			auto it = lower_bound(materials.begin(), materials.end(), v);
			if (it == materials.end() || *it != v) continue;
			nb.push_back(int(it - materials.begin()));
		}
		B[i] = std::move(nb);
	}
	for (int i = 0; i < K; i++) {
		C[i] = int(lower_bound(materials.begin(), materials.end(), C[i]) - materials.begin());
		workers[C[i]].push_back(i);
	}

	Dinic d(N + M + 1 + 1 + 1);
	const int S = N + M;
	const int Tcyc = N + M + 1;
	const int T = N + M + 1 + 1;

	for (int i = 0; i < M; i++) {
		d.addEdge(N + i, S, int(workers[i].size()));
	}

	for (int i = 0; i < N; i++) {
		for (int v : B[i]) {
			d.addEdge(i, N + v, 1);
		}
	}

	{
		// find the cycle
		int a = 0, b = 0;
		do {
			a = A[A[A[a]]];
			b = A[A[b]];
		} while (a != b);
		int start = a;
		int cycLen = 0;
		for (int cur = start; true; cur = A[cur]) {
			inCyc[cur] = true;
			cycLen++;
			if (A[cur] == start) break;
		}

		for (int i = 0; i < N; i++) {
			if (inCyc[i]) {
				d.addEdge(Tcyc, i, 1);
			} else {
				d.addEdge(T, i, 1);
			}
		}
		d.addEdge(T, Tcyc, cycLen - 1);
	}

	if (d.calc(T, S) != N-1) {
		cout << -1 << '\n';
		exit(0);
	}

	for (int i = 0; i < N; i++) {
		for (auto e : d.adj[i]) {
			if (e.f > 0) {
				assert(N <= e.to && e.to < N + M);
				int v = e.to - N;
				assert(!workers[v].empty());
				edge[workers[v].back()] = {i+1, A[i]+1};
				workers[v].pop_back();
			}
		}
	}

	for (int i = 0; i < K; i++) {
		cout << edge[i].first << ' ' << edge[i].second << '\n';
	}

	return 0;
}
