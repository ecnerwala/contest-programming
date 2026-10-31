#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

/**
 * Author: Simon Lindholm
 * Date: 2017-04-17
 * License: CC0
 * Source: folklore
 * Description: Finds all biconnected components in an undirected graph, and
 *  runs a callback for the edges in each. In a biconnected component there
 *  are at least two distinct paths between any two nodes. Note that a node can
 *  be in several components. An edge which is not in a component is a bridge,
 *  i.e., not part of any cycle.
 * Usage:
 *  int eid = 0; ed.resize(N);
 *  for each edge (a,b) {
 *    ed[a].emplace_back(b, eid);
 *    ed[b].emplace_back(a, eid++); }
 *  bicomps([\&](const vi\& edgelist) {...});
 * Time: O(E + V)
 * Status: tested during MIPT ICPC Workshop 2017
 */

vi num, st;
vector<vector<pii>> ed;
int Time;
template<class F>
int dfs(int at, int par, F& f) {
	int me = num[at] = ++Time, e, y, top = me;
	for (auto pa : ed[at]) if (pa.second != par) {
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
			else { /* e is a bridge */ }
		}
	}
	return top;
}

template<class F>
void bicomps(F f) {
	num.assign(sz(ed), 0);
	rep(i,0,sz(ed)) if (!num[i]) dfs(i, -1, f);
}
}

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		kactl::ed.clear();
		kactl::ed.resize(N);
		std::vector<std::tuple<int, int, int>> edges(M);
		for (int e = 0; e < M; e++) {
			int u, v, c; cin >> u >> v >> c; u--, v--; c--;
			edges[e] = {u, v, c};
			kactl::ed[u].push_back({v, e});
			kactl::ed[v].push_back({u, e});
		}

		bool good = false;
		std::vector<int> msk(N, 0);
		kactl::bicomps([&](std::vector<int> edgelist) {
			int all = 0;
			for (auto e : edgelist) {
				auto [u, v, c] = edges[e];
				all |= (1 << c);
				msk[u] |= (1 << c);
				msk[v] |= (1 << c);
			}

			int num_multi = 0;
			for (auto e : edgelist) {
				auto [u, v, c] = edges[e];
				if (msk[u] & (msk[u] - 1)) {
					num_multi++;
				}
				msk[u] = 0;
				if (msk[v] & (msk[v] - 1)) {
					num_multi++;
				}
				msk[v] = 0;
			}
			if (all == 7 && num_multi > 2) good = true;
		});

		cout << (good ? "Yes" : "No") << '\n';
	}

	return 0;
}
