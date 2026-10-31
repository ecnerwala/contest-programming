#include <bits/extc++.h>

template <typename K, typename V, typename Comp = std::less<K>>
using order_statistic_map = __gnu_pbds::tree<
	K, V, Comp,
	__gnu_pbds::rb_tree_tag,
	__gnu_pbds::tree_order_statistics_node_update
>;

template <typename K, typename Comp = std::less<K>>
using order_statistic_set = order_statistic_map<K, __gnu_pbds::null_type, Comp>;

// Supports
//  auto iterator = order_statistic_set().find_by_order(idx); // (0-indexed)
//  int num_strictly_smaller = order_statistic_set().order_of_key(key);

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

	int N; cin >> N;
	std::vector<std::vector<std::tuple<int, int, int64_t>>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int a, b; int64_t c; cin >> a >> b >> c; a--, b--;
		adj[a].push_back({e, b, c});
		adj[b].push_back({e, a, c});
	}

	int Q; cin >> Q;
	std::vector<std::tuple<int, int, int64_t>> all_queries(Q);
	std::vector<int> ans; ans.reserve(Q);
	for (int q = 0; q < Q; q++) {
		char op; cin >> op;
		if (op == '+') {
			int v; int64_t r; cin >> v >> r; v--;
			all_queries[q] = {-1-q, v, r};
		} else if (op == '?') {
			int e; cin >> e; e--;
			all_queries[q] = {int(ans.size()), e, 0};
			ans.push_back(0);
		}
	}

	std::vector<int> alive(N, true);
	std::vector<int> sz(N);
	std::vector<int64_t> vert_depth(N);
	std::vector<int64_t> edge_depth(N-1);

	std::vector<int> edge_comp(N-1);
	std::vector<int> vert_comp(N);
	std::vector<std::vector<std::tuple<int, int, int64_t>>> cur_queries(N);
	cur_queries[0] = all_queries;
	order_statistic_set<std::pair<int64_t, int>> all_set;
	std::vector<order_statistic_set<std::pair<int64_t, int>>> comp_set(N);

	std::y_combinator([&](auto solve_centroid, int rt) -> void {
		int tot_sz = std::y_combinator([&](auto self, int cur, int prvE) -> int {
			sz[cur] = 1;
			for (auto [e, nxt, l] : adj[cur]) {
				if (e == prvE) continue;
				if (!alive[nxt]) continue;
				sz[cur] += self(nxt, e);
			}
			return sz[cur];
		})(rt, -1);

		while (true) {
			bool found = false;
			for (auto [e, nxt, l] : adj[rt]) {
				if (!alive[nxt]) continue;
				if (sz[nxt] < sz[rt] && sz[nxt] * 2 > tot_sz) {
					std::swap(cur_queries[nxt], cur_queries[rt]);
					rt = nxt;
					found = true;
					break;
				}
			}
			if (!found) break;
		}

		vert_depth[rt] = 0;
		vert_comp[rt] = rt;
		std::y_combinator([&](auto self, int cur, int prvE, int comp) -> void {
			for (auto [e, nxt, l] : adj[cur]) {
				if (e == prvE) continue;
				int ncomp = (comp == -1 ? nxt : comp);
				vert_comp[nxt] = edge_comp[e] = ncomp;
				vert_depth[nxt] = edge_depth[e] = vert_depth[cur] + l;

				if (!alive[nxt]) continue;
				self(nxt, e, ncomp);
			}
		})(rt, -1, -1);

		for (auto [q, idx, r] : cur_queries[rt]) {
			if (q < 0) {
				int v = idx;
				int comp = vert_comp[v];
				if (v != rt) {
					assert(alive[vert_comp[v]]);
					cur_queries[vert_comp[v]].push_back({q, idx, r});
				}

				all_set.insert({r - vert_depth[v], -q});
				comp_set[comp].insert({r - vert_depth[v], -q});
			} else {
				assert(r == 0);
				int e = idx;
				int comp = edge_comp[e];
				if (alive[edge_comp[e]]) {
					cur_queries[edge_comp[e]].push_back({q, idx, r});
				}

				int ways = int(all_set.size()) - int(all_set.order_of_key({edge_depth[e], -1}));
				ways -= int(comp_set[comp].size()) - int(comp_set[comp].order_of_key({edge_depth[e], -1}));
				ans[q] += ways;
			}
		}

		cur_queries[rt] = {};
		alive[rt] = false;
		all_set = {};
		comp_set[rt] = {};
		for (auto [e, nxt, l] : adj[rt]) {
			comp_set[nxt] = {};
			if (!alive[nxt]) continue;
			solve_centroid(nxt);
		}
	})(0);

	for (int v : ans) {
		cout << v << '\n';
	}

	return 0;
}
