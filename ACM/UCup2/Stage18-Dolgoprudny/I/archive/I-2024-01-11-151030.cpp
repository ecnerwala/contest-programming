#include <bits/stdc++.h>

namespace kactl {
using namespace std;
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

vi val, comp, z, cont;
int Time, ncomps;
template<class G, class F> int dfs(int j, G& g, F& f) {
	int low = val[j] = ++Time, x; z.push_back(j);
	for (auto e : g[j]) if (comp[e] < 0)
		low = min(low, val[e] ?: dfs(e,g,f));

	if (low == val[j]) {
		do {
			x = z.back(); z.pop_back();
			comp[x] = ncomps;
			cont.push_back(x);
		} while (x != j);
		f(cont); cont.clear();
		ncomps++;
	}
	return val[j] = low;
}
template<class G, class F> void scc(G& g, F f) {
	int n = sz(g);
	val.assign(n, 0); comp.assign(n, -1);
	Time = ncomps = 0;
	rep(i,0,n) if (comp[i] < 0) dfs(i, g, f);
}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::vector<int>> adj_pass(N);
	std::vector<std::vector<int>> adj_no_pass(N);
	for (int e = 0; e < M; e++) {
		int u, v, t; cin >> u >> v >> t; u--, v--;
		if (t == 1) {
			adj_pass[u].push_back(v);
		} else if (t == 2) {
			adj_pass[u].push_back(v);
			adj_no_pass[u].push_back(v);
		} else assert(false);
	}

	std::vector<bool> vis(N);
	std::vector<int> q; q.reserve(N);
	vis[0] = true; q.push_back(0);
	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		for (int nxt : adj_pass[cur]) {
			if (!vis[nxt]) {
				vis[nxt] = true;
				q.push_back(nxt);
			}
		}
	}

	kactl::scc(adj_no_pass, [&](const std::vector<int>&) {});
	bool can_lose = false;
	for (int i = 0; i < N; i++) {
		if (vis[i]) {
			for (int j : adj_no_pass[i]) {
				if (kactl::comp[j] != kactl::comp[i]) {
					can_lose = true;
				}
			}
		}
	}
	cout << (can_lose ? "Yes" : "No") << '\n';
	return 0;
}
