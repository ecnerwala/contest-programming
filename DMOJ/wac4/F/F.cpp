#include<bits/stdc++.h>
using namespace std;

namespace kactl {
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
struct PushRelabel {
	struct Edge {
		int dest, back;
		ll f, c;
	};
	vector<vector<Edge>> g;
	vector<ll> ec;
	vector<Edge*> cur;
	vector<vi> hs; vi H;
	PushRelabel(int n) : g(n), ec(n), cur(n), hs(2*n), H(n) {}

	void addEdge(int s, int t, ll cap, ll rcap=0) {
		if (s == t) return;
		g[s].push_back({t, sz(g[t]), 0, cap});
		g[t].push_back({s, sz(g[s])-1, 0, rcap});
	}

	void addFlow(Edge& e, ll f) {
		Edge &back = g[e.dest][e.back];
		if (!ec[e.dest] && f) hs[H[e.dest]].push_back(e.dest);
		e.f += f; e.c -= f; ec[e.dest] += f;
		back.f -= f; back.c += f; ec[back.dest] -= f;
	}
	ll calc(int s, int t) {
		int v = sz(g); H[s] = v; ec[t] = 1;
		vi co(2*v); co[0] = v-1;
		rep(i,0,v) cur[i] = g[i].data();
		for (Edge& e : g[s]) addFlow(e, e.c);

		for (int hi = 0;;) {
			while (hs[hi].empty()) if (!hi--) return -ec[s];
			int u = hs[hi].back(); hs[hi].pop_back();
			while (ec[u] > 0)  // discharge u
				if (cur[u] == g[u].data() + sz(g[u])) {
					H[u] = 1e9;
					for (Edge& e : g[u]) if (e.c && H[u] > H[e.dest]+1)
						H[u] = H[e.dest]+1, cur[u] = &e;
					if (++co[H[u]], !--co[hi] && hi < v)
						rep(i,0,v) if (hi < H[i] && H[i] < v)
							--co[H[i]], H[i] = v + 1;
					hi = H[u];
				} else if (cur[u]->c && H[u] == H[cur[u]->dest]+1)
					addFlow(*cur[u], min(ec[u], cur[u]->c));
				else ++cur[u];
		}
	}
	bool leftOfMinCut(int a) { return H[a] >= sz(g); }
};

#undef rep
#undef all
#undef sz
} // namespace kactl

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	using i64 = int64_t;

	int N, M; cin >> N >> M;
	vector<pair<i64, i64>> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i].first >> A[i].second;
	}
	vector<vector<pair<i64, i64>>> C(N, vector<pair<i64, i64>>(M));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> C[i][j].first;
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> C[i][j].second;
		}
	}
	vector<pair<i64, i64>> E(M);
	for (int j = 0; j < M; j++) {
		cin >> E[j].first >> E[j].second;
	}

	kactl::PushRelabel flower(4+N+M);
	i64 needFlow = 0;

	const int Snew = 0, Tnew = 1;
	vector<i64> SnewCap(4+N+M);
	vector<i64> TnewCap(4+N+M);
	auto addEdge = [&](int l, int r, pair<i64, i64> bounds) {
		assert(bounds.first <= bounds.second);
		flower.addEdge(l, r, bounds.second - bounds.first);
		SnewCap[r] += bounds.first;
		TnewCap[l] += bounds.first;
		needFlow += bounds.first;
	};

	const int S = 2, T = 3;
	const int SN = 4;
	const int SM = 4 + N;

	for (int i = 0; i < N; i++) {
		addEdge(S, SN+i, A[i]);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			addEdge(SN+i, SM+j, C[i][j]);
		}
	}
	for (int j = 0; j < M; j++) {
		addEdge(SM+j, T, E[j]);
	}

	for (int v = 2; v < 4+N+M; v++) {
		flower.addEdge(Snew, v, SnewCap[v]);
		flower.addEdge(v, Tnew, TnewCap[v]);
	}

	const i64 INF = 1e18;

	{
		kactl::PushRelabel curF = flower;
		curF.addEdge(T, S, INF);
		if (curF.calc(Snew, Tnew) != needFlow) {
			cout << -1 << '\n';
			exit(0);
		}
	}

	i64 loVal;
	{
		kactl::PushRelabel curF = flower;
		loVal = needFlow - curF.calc(Snew, Tnew);
	}

	i64 hiVal;
	{
		kactl::PushRelabel curF = flower;
		// to check hi, you add T->S INF-hi, Snew->S hi, T->Tnew hi, and check if it is at least needFlow+hi
		curF.addEdge(T, S, INF);
		curF.addEdge(Snew, S, INF);
		curF.addEdge(T, Tnew, INF);

		hiVal = curF.calc(Snew, Tnew) - needFlow;
	}

	cout << loVal << '\n' << hiVal << '\n';

	return 0;
}
