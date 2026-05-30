#include <bits/stdc++.h>
#include <cassert>

namespace kactl {
using namespace std;
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
vi val, comp, z, cont, depth;
int Time, ncomps;
template<class G, class F> int dfs(int j, int d, G& g, F& f) {
	depth[j] = d;
	int low = val[j] = ++Time, x; z.push_back(j);
	for (auto e : g[j]) if (comp[e] < 0)
		low = min(low, val[e] ?: dfs(e,d+1,g,f));

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
	depth.assign(n, -1); val.assign(n, 0); comp.assign(n, -1);
	Time = ncomps = 0;
	rep(i,0,n) if (comp[i] < 0) dfs(i, 0, g, f);
}
}

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int M; std::cin >> M;
		std::map<std::array<int, 2>, int> verts;
		auto get_id = [&](std::array<int, 2> p) {
			auto [it, did_insert] = verts.insert({p, int(verts.size())});
			return it->second;
		};
		std::vector<std::array<int, 2>> edges(M);
		for (int e = 0; e < M; e++) {
			int a, b, c; std::cin >> a >> b >> c;
			int x = get_id({a, b});
			int y = get_id({b, c});
			edges[e] = {x, y};
		}

		int N = int(verts.size());
		std::vector<std::vector<int>> adj(N);
		for (auto [x, y] : edges) adj[x].push_back(y);
		kactl::scc(adj, [&](const std::vector<int>&) -> void { });
		bool found = false;
		for (auto [u, v] : edges) {
			std::cerr << kactl::comp[u] << ' ' << kactl::comp[v] << '\n';
			if (kactl::comp[v] != kactl::comp[u]) continue;
			if (kactl::depth[v] % 3 != (kactl::depth[u] + 1) % 3) {
				found = true;
			}
		}
		std::cout << (found ? "yes" : "no") << '\n';
	}

	return 0;
}
