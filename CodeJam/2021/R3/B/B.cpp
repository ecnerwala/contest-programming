#include <bits/stdc++.h>
#include <numeric>

namespace kactl {

using namespace std;

// https://github.com/kth-competitive-programming/kactl/blob/main/content/graph/Dinic.h

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
struct Dinic {
	struct Edge {
		int to, rev;
		ll c, oc;
		ll flow() { return max(oc - c, 0LL); } // if you need flows
	};
	vi lvl, ptr, q;
	vector<vector<Edge>> adj;
	Dinic(int n) : lvl(n), ptr(n), q(n), adj(n) {}
	void addEdge(int a, int b, ll c, ll rcap = 0) {
		adj[a].push_back({b, sz(adj[b]), c, c});
		adj[b].push_back({a, sz(adj[a]) - 1, rcap, rcap});
	}
	ll dfs(int v, int t, ll f) {
		if (v == t || !f) return f;
		for (int& i = ptr[v]; i < sz(adj[v]); i++) {
			Edge& e = adj[v][i];
			if (lvl[e.to] == lvl[v] + 1)
				if (ll p = dfs(e.to, t, min(f, e.c))) {
					e.c -= p, adj[e.to][e.rev].c += p;
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
				for (Edge e : adj[v])
					if (!lvl[e.to] && e.c >> (30 - L))
						q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
			}
			while (ll p = dfs(s, t, LLONG_MAX)) flow += p;
		} while (lvl[t]);
		return flow;
	}
	bool leftOfMinCut(int a) { return lvl[a] != 0; }
};
}

// Just put the forward slashes as far up-right as possible
// A square can always be flipped to be more up-right

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int R, C; cin >> R >> C;
		vector<int> A(R); for (auto& a : A) cin >> a;
		vector<int> B(C); for (auto& b : B) cin >> b;

		auto possible = [&](int r0, int cmax) -> bool {
			for (int a : A) if (a < 0) return false;
			for (int b : B) if (b < 0) return false;
			int tot = std::accumulate(A.begin(), A.end(), 0);
			if (std::accumulate(B.begin(), B.end(), 0) != tot) return false;
			kactl::Dinic flower(R+C+2);
			int S = R+C;
			int T = R+C+1;
			for (int i = 0; i < R; i++) {
				flower.addEdge(S, i, A[i]);
			}
			for (int j = 0; j < C; j++) {
				flower.addEdge(R+j, T, B[j]);
			}
			for (int j = 0; j < cmax; j++) {
				flower.addEdge(r0, R+j, 1);
			}
			for (int i = r0+1; i < R; i++) {
				for (int j = 0; j < C; j++) {
					flower.addEdge(i, R+j, 1);
				}
			}
			return flower.calc(S, T) == tot;
		};

		cout << "Case #" << case_num << ": ";
		if (!possible(0, C)) {
			cout << "IMPOSSIBLE" << '\n';
		} else {
			cout << "POSSIBLE" << '\n';
			vector<string> grid(R, string(C, '\\'));
			for (int i = 0; i < R; i++) {
				for (int j = C-1; j >= 0; j--) {
					A[i]--; B[j]--;
					if (possible(i, j)) {
						grid[i][j] = '/';
					} else {
						A[i]++, B[j]++;
					}
				}
			}
			for (string s : grid) {
				cout << s << '\n';
			}
		}
	}

	return 0;
}
