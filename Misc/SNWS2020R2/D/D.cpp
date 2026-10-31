#include<bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi num, st;
vector<vector<pii>> ed;
int Time;
template<class F>
int dfs(int at, int par, F f) {
	int me = num[at] = ++Time, e, y, top = me;
	trav(pa, ed[at]) if (pa.second != par) {
		tie(y, e) = pa;
		if (num[y]) {
			top = min(top, num[y]);
			if (num[y] < me)
				st.push_back(e);
		} else {
			int si = sz(st);
			int up = dfs(y, e, f);
			top = min(top, up);
			if (up == me) {
				st.push_back(e);
				f(vi(st.begin() + si, st.end()));
				st.resize(si);
			}
			else if (up < me) st.push_back(e);
			else { f(vector<int>({e}));/* e is a bridge */ }
		}
	}
	return top;
}

template<class F>
void bicomps(F f) {
	num.assign(sz(ed), 0);
	rep(i,0,sz(ed)) if (!num[i]) dfs(i, -1, f);
}

const int MAXN = 2.1e5;
int N, M;
int par[MAXN];
ll V[MAXN];
void reset() {
	memset(par, -1, sizeof(par));
}
int getPar(int a) { return par[a] == -1 ? a : (par[a] = getPar(par[a])); }
bool merge(int a, int b) {
	a = getPar(a);
	b = getPar(b);
	if (a == b) return false;
	V[a] += V[b];
	par[b] = a;
	return true;
}

vector<int> adj[MAXN];

ll ans = 0;
ll dfs(int cur, int prv) {
	assert(getPar(cur) == cur);
	ll bestPath = 0;
	ll bestPath2 = 0;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		ll nxtPath = dfs(nxt, cur);
		if (nxtPath >= bestPath) {
			bestPath2 = bestPath;
			bestPath = nxtPath;
		} else if (nxtPath >= bestPath2) {
			bestPath2 = nxtPath;
		}
	}
	ans = max(ans, V[cur] + bestPath + bestPath2);
	bestPath += V[cur];
	return bestPath;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 0; i < N; i++) cin >> V[i];

	reset();

	vector<pair<int, int>> edges(M);
	ed.resize(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		edges[e] = {u,v};
		ed[u].emplace_back(v, e);
		ed[v].emplace_back(u, e);
	}

	vector<pair<int, int>> bridges;

	bicomps([&](const vector<int>& edgelist) {
		if (edgelist.size() == 1) {
			bridges.push_back(edges[edgelist[0]]);
			return;
		}
		for (int e : edgelist) {
			int u = edges[e].first;
			int v = edges[e].second;
			merge(u, v);
		}
	});

	for (auto it : bridges) {
		int u = getPar(it.first);
		int v = getPar(it.second);
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	dfs(getPar(0), -1);

	cout << ans << '\n';

	return 0;
}
