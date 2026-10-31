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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	std::vector<int64_t> tot_weight(N);
	std::vector<std::vector<int>> verts(N);

	for (int e = 0; e < N-1; e++) {
		int u, v; int64_t w; int c; cin >> u >> v >> w >> c;
		u--, v--; c--;
		tot_weight[c] += w;
		verts[c].push_back(u);
		verts[c].push_back(v);
	}

	std::vector<std::vector<int>> adj(2*N);

	for (int c = 0; c < N; c++) {
		if (verts[c].empty()) continue;

		std::sort(verts[c].begin(), verts[c].end());
		int num_leaves = 0;
		for (int i = 0; i < int(verts[c].size()); ) {
			int j = i;
			while (j < int(verts[c].size()) && verts[c][j] == verts[c][i]) j++;
			num_leaves += (j - i == 1);
			i = j;
		}
		assert(num_leaves >= 2);
		if (num_leaves != 2) {
			verts[c].clear();
			tot_weight[c] = 0;
			continue;
		}
		verts[c].resize(std::unique(verts[c].begin(), verts[c].end()) - verts[c].begin());
		for (int v : verts[c]) {
			adj[v].push_back(N+c);
			adj[N+c].push_back(v);
		}
	}

	std::vector<int> par(2*N, -1);

	auto dfs = std::y_combinator([&](auto self, int cur, int prv) -> void {
		par[cur] = prv;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
		}
	});

	for (int i = 0; i < N; i++) {
		if (par[i] == -1) dfs(i, -1);
	}

	std::vector<int> num_blocked_ch(2*N, 0);
	std::vector<std::set<std::pair<int64_t, int>>> live_children(N);
	std::set<std::pair<int64_t, int>> good_vals;

	good_vals.insert({0, -1});
	for (int i = 0; i < N; i++) {
		live_children[i].insert({0, -1});
		for (int nxt : adj[i]) {
			if (nxt == par[i]) continue;
			live_children[i].insert({tot_weight[nxt - N], nxt});
		}
		good_vals.insert({live_children[i].rbegin()->first, i});
	}

	std::vector<bool> is_alive(N, true);

	for (int q = 0; q < Q; q++) {
		int op, v; cin >> op >> v; v--;
		if (op == 0) {
			// block this node
			assert(is_alive[v]);
			is_alive[v] = false;
			good_vals.erase({live_children[v].rbegin()->first, v});
			if (par[v] != -1) {
				if ((num_blocked_ch[par[v]] ++) == 0) {
					// fix the parent parent
					int u = par[par[v]];
					if (is_alive[u]) {
						good_vals.erase({live_children[u].rbegin()->first, u});
					}
					live_children[u].erase({tot_weight[par[v] - N], par[v]});
					if (is_alive[u]) {
						good_vals.insert({live_children[u].rbegin()->first, u});
					}
				}
			}
		} else if (op == 1) {
			assert(!is_alive[v]);
			is_alive[v] = true;
			good_vals.insert({live_children[v].rbegin()->first, v});
			if (par[v] != -1) {
				if ((--num_blocked_ch[par[v]]) == 0) {
					// fix the parent parent
					int u = par[par[v]];
					if (is_alive[u]) {
						good_vals.erase({live_children[u].rbegin()->first, u});
					}
					live_children[u].insert({tot_weight[par[v] - N], par[v]});
					if (is_alive[u]) {
						good_vals.insert({live_children[u].rbegin()->first, u});
					}
				}
			}
		} else assert(false);
		cout << good_vals.rbegin()->first << '\n';
	}

	return 0;
}
