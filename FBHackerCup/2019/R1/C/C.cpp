#include<bits/stdc++.h>
using namespace std;

using ll = long long;
using vi = vector<int>;
#define sz(x) int((x).size())
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define trav(a, x) for (auto &a : x)
typedef ll Flow;
struct Edge {
	int dest, back;
	Flow f, c;
};
struct PushRelabel {
	vector<vector<Edge>> g;
	vector<Flow> ec;
	vector<Edge*> cur;
	vector<vi> hs; vi H;
	PushRelabel(int n) : g(n), ec(n), cur(n), hs(2*n), H(n) {}

	void add_edge(int s, int t, Flow cap, Flow rcap=0) {
		if (s == t) return;
		Edge a = {t, sz(g[t]), 0, cap};
		Edge b = {s, sz(g[s]), 0, rcap};
		g[s].push_back(a);
		g[t].push_back(b);
	}

	void add_flow(Edge& e, Flow f) {
		Edge &back = g[e.dest][e.back];
		if (!ec[e.dest] && f) hs[H[e.dest]].push_back(e.dest);
		e.f += f; e.c -= f; ec[e.dest] += f;
		back.f -= f; back.c += f; ec[back.dest] -= f;
	}
	Flow maxflow(int s, int t) {
		int v = sz(g); H[s] = v; ec[t] = 1;
		vi co(2*v); co[0] = v-1;
		rep(i,0,v) cur[i] = g[i].data();
		trav(e, g[s]) add_flow(e, e.c);

		for (int hi = 0;;) {
			while (hs[hi].empty()) if (!hi--) return -ec[s];
			int u = hs[hi].back(); hs[hi].pop_back();
			while (ec[u] > 0)  // discharge u
				if (cur[u] == g[u].data() + sz(g[u])) {
					H[u] = 1e9;
					trav(e, g[u]) if (e.c && H[u] > H[e.dest]+1)
						H[u] = H[e.dest]+1, cur[u] = &e;
					if (++co[H[u]], !--co[hi] && hi < v)
						rep(i,0,v) if (hi < H[i] && H[i] < v)
							--co[H[i]], H[i] = v + 1;
					hi = H[u];
				} else if (cur[u]->c && H[u] == H[cur[u]->dest]+1)
					add_flow(*cur[u], min(ec[u], cur[u]->c));
				else ++cur[u];
		}
	}
};

const int MAXN = 60;
int N;
ll H;
ll X[MAXN];
ll A[MAXN];
ll B[MAXN];

ll go() {
	cin >> N >> H;
	for (int i = 0; i < N; i++) cin >> X[i] >> A[i] >> B[i];

	PushRelabel flower(N+2);
	const int S = N;
	const int T = N+1;
	const ll INF = N * H + 100;
	//const ll INF = int(1e18);

	for (int i = 0; i < N; i++) {
		if (A[i] == 0) {
			flower.add_edge(S, i, INF);
		}
		if (B[i] == H) {
			flower.add_edge(i, T, INF);
		}
	}

	vector<ll> coords;
	coords.push_back(0);
	coords.push_back(H);
	for (int i = 0; i < N; i++) {
		coords.push_back(A[i]);
		coords.push_back(B[i]);
	}
	sort(coords.begin(), coords.end());
	coords.resize(unique(coords.begin(), coords.end()) - coords.begin());

	assert(coords.front() == 0);
	assert(coords.back() == H);

	for (int z = 0; z+1 < int(coords.size()); z++) {
		ll a = coords[z];
		ll b = coords[z+1];
		if (a == 0 || b == H) continue;
		vector<pair<ll, int>> ladders;
		for (int i = 0; i < N; i++) {
			if (A[i] <= a && b <= B[i]) {
				ladders.emplace_back(X[i], i);
			}
		}
		sort(ladders.begin(), ladders.end());
		for (int v = 0; v+1 < int(ladders.size()); v++) {
			flower.add_edge(ladders[v].second, ladders[v+1].second, b-a, b-a);
		}
	}

	ll ans = flower.maxflow(S, T);
	if (ans >= INF) {
		return -1;
	} else {
		return ans;
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		ll ans = go();
		cout << "Case #" << t << ": " << ans << '\n';
	}

	return 0;
}
