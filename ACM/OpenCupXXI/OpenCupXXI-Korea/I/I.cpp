#include<bits/stdc++.h>

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

struct decision_tree {
	int output;
	int l, r;
	decision_tree* if_bigger;
	decision_tree* if_smaller;
	~decision_tree() { delete if_bigger; delete if_smaller; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;

		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	const int ROOT = 0;
	vector<int> sz(N);
	vector<int> par(N);
	vector<int> depth(N);

	std::y_combinator([&](auto self, int cur, int prv) -> void {
		par[cur] = prv;
		depth[cur] = (prv == -1 ? 0 : depth[prv] + 1);
		sz[cur] = 1;
		if (prv != -1) {
			auto it = find(adj[cur].begin(), adj[cur].end(), prv);
			assert(it != adj[cur].end());
			adj[cur].erase(it);
		}

		for (int nxt : adj[cur]) {
			self(nxt, cur);
			sz[cur] += sz[nxt];
		}
		sort(adj[cur].begin(), adj[cur].end(), [&](int a, int b) { return sz[a] > sz[b]; });
	})(ROOT, -1);

	vector<int> st(N);
	vector<int> en(N);
	vector<int> preorder; preorder.reserve(N);
	vector<int> heavy_top(N);

	{
		int nxt_idx = 0;

		std::y_combinator([&](auto self, int cur) -> void {
			preorder.push_back(cur);
			st[cur] = nxt_idx;
			heavy_top[cur] = (par[cur] != -1 && st[cur] == st[par[cur]]+1 ? heavy_top[par[cur]] : cur);
			nxt_idx++;
			for (int nxt : adj[cur]) {
				self(nxt);
			}
			en[cur] = nxt_idx;

			assert(en[cur] - st[cur] == sz[cur]);
		})(ROOT);
		assert(nxt_idx == N);
	}

	auto lca = [&](int a, int b) -> int {
		while (true) {
			if (st[a] > st[b]) swap(a, b);
			assert(st[a] <= st[b]);
			if (st[a] >= st[heavy_top[b]]) {
				return a;
			}
			b = par[heavy_top[b]];
		}
	};

	const int S = N;
	std::vector<int64_t> subtree_val(2*S);
	std::vector<int64_t> subtree_path_cnt(2*S);
	std::vector<int64_t> supertree_cnt(2*S);

	auto incr_subtree = [&](int i) -> void {
		for (int a = S+st[i]; a; a >>= 1) {
			subtree_val[a] += sz[i];
		}
		for (int a = S+st[i]+1, b = S+en[i]; a < b; a >>= 1, b >>= 1) {
			if (a & 1) supertree_cnt[a++] ++;
			if (b & 1) supertree_cnt[--b] ++;
		}
	};

	auto incr_supertree = [&](int i, int v) -> void {
		for (int a = S+st[i]; a; a >>= 1) {
			subtree_val[a] += v * (depth[i]+1);
			subtree_path_cnt[a] += v;
		}
	};

	auto query_range = [&](int l, int r) -> int64_t {
		assert(0 <= l && l < r && r <= N);
		int64_t ans = 0;
		for (int a = S+l; a; a >>= 1) {
			ans += (r-l) * supertree_cnt[a];
		}
		int depth_sub = depth[preorder[l]];
		for (int a = S+l, b = S+r; a < b; a >>= 1, b >>= 1) {
			if (a & 1) {
				ans += subtree_val[a];
				ans -= depth_sub * subtree_path_cnt[a];
				a++;
			}
			if (b & 1) {
				--b;
				ans += subtree_val[b];
				ans -= depth_sub * subtree_path_cnt[b];
			}
		}
		return ans;
	};

	auto query_subtree = [&](int i) -> int64_t { return query_range(st[i], en[i]); };

	auto incr_path = [&](int a, int b) -> void {
		int c = lca(a, b);
		incr_supertree(a, 1);
		incr_supertree(b, 1);

		incr_supertree(c, -1);
		if (par[c] != -1) {
			incr_supertree(par[c], -1);
		}
	};

	decision_tree* QUERY_TREE;

	{
		vector<bool> alive(N, true);
		vector<int> cur_sz(N);
		QUERY_TREE = std::y_combinator([&](auto build_decision_tree, int a) -> decision_tree* {
			if (!alive[a]) {
				return new decision_tree{par[a], -1, -1, nullptr, nullptr};
			}

			{ // Generate sz
				std::y_combinator([&](auto self, int cur) -> void {
					if (!alive[cur]) {
						cur_sz[cur] = 0;
						return;
					}
					cur_sz[cur] = 1;
					for (int nxt : adj[cur]) {
						self(nxt);
						cur_sz[cur] += cur_sz[nxt];
					}
				})(a);
			}

			assert(cur_sz[a] >= 1);
			int centroid = std::y_combinator([&](auto self, int cur) -> int {
				assert(cur_sz[cur] * 2 > cur_sz[a]);
				for (int nxt : adj[cur]) {
					if (!alive[nxt]) continue;
					if (cur_sz[nxt] * 2 > cur_sz[a]) return self(nxt);
				}
				return cur;
			})(a);

			alive[centroid] = false;
			decision_tree* res = new decision_tree{-1, st[centroid], en[centroid], nullptr, nullptr};
			vector<int> cur_adj; cur_adj.reserve(adj[centroid].size());
			for (int nxt : adj[centroid]) {
				if (!alive[nxt]) continue;
				cur_adj.push_back(nxt);
			}

			res->if_bigger = std::y_combinator([&](auto self, int lo, int hi) -> decision_tree* {
				if (lo == hi) {
					// nothing
					return new decision_tree{centroid, -1, -1, nullptr, nullptr};
				}

				assert(lo < hi);
				if (hi - lo == 1) {
					return build_decision_tree(cur_adj[lo]);
				}

				assert(hi - lo >= 2);

				int tot = 0;
				for (int k = lo; k < hi; k++) {
					tot += cur_sz[cur_adj[k]];
				}

				int md = lo+1;
				int lhs = cur_sz[cur_adj[lo]];

				while (md+1 < hi && abs((cur_sz[cur_adj[md]] + lhs) * 2 - tot) < abs(lhs * 2 - tot)) {
					lhs += cur_sz[cur_adj[md]];
					md++;
				}
				assert(lo < md && md < hi);

				auto sub = new decision_tree{-1, st[cur_adj[lo]], st[cur_adj[md]], nullptr, nullptr};
				sub->if_bigger = self(lo, md);
				sub->if_smaller = self(md, hi);
				return sub;
			})(0, int(cur_adj.size()));
			res->if_smaller = build_decision_tree(a);
			return res;
		})(0);
	}

	auto do_query = [&]() -> int {
		int64_t tot = query_subtree(ROOT);
		decision_tree* cur = QUERY_TREE;
		while (true) {
			assert(cur);
			if (cur->output != -1) {
				return cur->output;
			}
			int64_t v = query_range(cur->l, cur->r);
			if (v * 2 > tot) {
				cur = cur->if_bigger;
			} else {
				cur = cur->if_smaller;
			}
		}
	};

	int Q; cin >> Q;
	while (Q--) {
		int op; cin >> op;
		if (op == 1) {
			int u; cin >> u; u--;
			incr_subtree(u);
		} else if (op == 2) {
			int u, v; cin >> u >> v; u--, v--;
			incr_path(u, v);
		} else assert(false);
		/*
		for (int i = 0; i < N; i++) {
			cerr << i+1 << ' ' << query_subtree(i) << '\n';
		}
		cerr << '\n';
		*/
		int ans = do_query();
		cout << ans+1 << '\n';
	}

	//delete QUERY_TREE;

	return 0;
}
