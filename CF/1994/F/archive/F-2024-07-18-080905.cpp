#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

namespace kactl {
using namespace std;
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
vi eulerWalk(vector<vector<pii>>& gr, int nedges, int src=0) {
	int n = sz(gr);
	vi D(n), its(n), eu(nedges), ret, s = {src};
	D[src]++; // to allow Euler paths, not just cycles
	while (!s.empty()) {
		int x = s.back(), y, e, &it = its[x], end = sz(gr[x]);
		if (it == end){ ret.push_back(x); s.pop_back(); continue; }
		tie(y, e) = gr[x][it++];
		if (!eu[e]) {
			D[x]--, D[y]++;
			eu[e] = 1; s.push_back(y);
		}}
	for (int x : D) if (x < 0 || sz(ret) != nedges+1) return {};
	return {ret.rbegin(), ret.rend()};
}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::vector<std::pair<int, int>>> must_edges(N);
		std::vector<std::vector<std::pair<int, int>>> can_edges(N);
		for (int e = 0; e < M; e++) {
			int u, v, c; cin >> u >> v >> c; u--, v--;
			if (c == 0) {
				can_edges[u].push_back({v, e});
				can_edges[v].push_back({u, e});
			} else if (c == 1) {
				must_edges[u].push_back({v, e});
				must_edges[v].push_back({u, e});
			} else assert(false);
		}
		bool good = true;
		std::vector<bool> vis(N);
		for (int st = 0; st < N; st++) {
			if (vis[st]) continue;
			bool root_extra = std::y_combinator([&](auto self, int cur) -> bool {
				assert(!vis[cur]);
				vis[cur] = true;
				for (auto [nxt, e] : can_edges[cur]) {
					if (!vis[nxt]) {
						bool has_extra = self(nxt);
						if (has_extra) {
							must_edges[nxt].push_back({cur, e});
							must_edges[cur].push_back({nxt, e});
						}
					}
				}
				return int(must_edges[cur].size()) & 1;
			})(st);
			if (root_extra) {
				good = false;
				break;
			}
		}
		if (good) {
			auto res = kactl::eulerWalk(must_edges, M, 0);
			cout << "YES" << '\n';
			cout << res.size() << '\n';
			for (int i = 0; i < int(res.size()); i++) {
				cout << res[i]+1 << " \n"[i+1==int(res.size())];
			}
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
