#include<bits/stdc++.h>

namespace kactl {
using namespace std;

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

#undef sz
#undef all
#undef rep
} // namespace kactl

int main() {
	using namespace std;
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int N, M, A, B, C; cin >> N >> M >> A >> B >> C;

	vector<string> G(N); for (auto& s : G) cin >> s;

	const int INF = 1e9;

	kactl::PushRelabel flower(4*N*M+2);
	int S = 4*N*M;
	int T = 4*N*M+1;

	// black cells can be horiz, vert, both, or neither (single-cell)
	// white cells can be black neither (0 cost), black once + white once, black once + white neither (single-cell)

	// 0 is black dx, true on T-side
	// 1 is black dy, true on S-side
	// 2 is white dx, true on S-side
	// 3 is white dy, true on T-side

	auto V = [&](int i, int j, int t) { return (i*M+j) * 4 + t; };

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			// black per-cell costs
			flower.addEdge(S, V(i,j,0), A);
			flower.addEdge(V(i,j,1), T, A);

			// white per-cell costs
			flower.addEdge(V(i,j,2), T, G[i][j] == '#' ? INF : A);
			flower.addEdge(S, V(i,j,3), G[i][j] == '#' ? INF : A);

			// line costs: If you're true but your predecessor is on the opposite side
			flower.addEdge(i ? V(i-1,j,0) : S, V(i,j,0), B);
			flower.addEdge(V(i,j,1), j ? V(i,j-1,1) : T, B);
			flower.addEdge(V(i,j,2), i ? V(i-1,j,2) : T, B);
			flower.addEdge(j ? V(i,j-1,3) : S, V(i,j,3), B);

			if (G[i][j] == '#') {
				// if both are false, then +C
				flower.addEdge(V(i,j,0), V(i,j,1), C);
			} else {
				// Can't double black
				flower.addEdge(V(i,j,1), V(i,j,0), INF);

				// If no white and single black, then +C
				// This also penalizes Bdx && Wdx && !Wdy but that's okay because that's suboptimal anyways
				flower.addEdge(V(i,j,3), V(i,j,0), C);
				flower.addEdge(V(i,j,1), V(i,j,2), C);
			}
		}
	}

	cout << flower.calc(S, T) << '\n';

	return 0;
}
