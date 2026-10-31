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

	int N_orig, Q; cin >> N_orig >> Q;
	const int V = 3 * N_orig - 2;
	std::vector<std::vector<std::pair<int, int>>> adj(V);
	std::vector<int> lengths(V-1);
	auto add_edge = [&](int e, int u, int v, int w) {
		lengths[e] = w;
		adj[u].push_back({e, v});
		adj[v].push_back({e, u});
	};
	std::vector<int> cur_vert(N_orig);
	std::iota(cur_vert.begin(), cur_vert.end(), 0);
	for (int e = 0; e < N_orig-1; e++) {
		int u, v, w; cin >> u >> v >> w; u--, v--;
		int a = N_orig + 2 * e;
		int b = N_orig + 2 * e + 1;

		add_edge(a - 1, cur_vert[u], a, 0);
		cur_vert[u] = a;

		add_edge(b - 1, cur_vert[v], b, 0);
		cur_vert[v] = b;

		add_edge(e, cur_vert[u], cur_vert[v], w);
	}

	const int SIZE_LIMIT = 500;

	std::vector<std::array<int, 2>> edge_verts(V-1);
	std::vector<bool> is_special(V);
	std::y_combinator([&](auto self, int cur, int prv) -> std::pair<int, bool> {
		int sz = 0;
		int num_special = 0;
		for (auto [e, nxt] : adj[cur]) {
			if (nxt == prv) continue;
			edge_verts[e] = {cur, nxt};
			auto [ch_sz, has_special] = self(nxt, cur);
			sz += ch_sz;
			num_special += has_special;
		}
		if (prv == -1 || num_special >= 2 || sz > SIZE_LIMIT) {
			is_special[cur] = true;
			return {0, true};
		} else {
			return {sz, num_special > 0};
		}
	})(0, -1);

	std::vector<int> st(V);
	std::vector<int> en(V);
	std::vector<int> comp_id(V-1);
	int C = 0;
	std::vector<std::array<int, 2>> comp_verts; comp_verts.reserve(V-1);
	std::vector<int64_t> comp_len(C);
	{
		int nxt_idx = 0;
		std::y_combinator([&](auto self, int cur, int prv, int cur_comp, int64_t dist) -> void {
			st[cur] = nxt_idx;
			nxt_idx++;
			if (is_special[cur]) {
				if (cur_comp != -1) {
					assert(comp_verts[cur_comp][1] == -1);
					comp_verts[cur_comp][1] = cur;
					comp_len[cur_comp] = dist;
				}
			}
			for (auto [e, nxt] : adj[cur]) {
				if (nxt == prv) continue;
				int nxt_comp;
				int64_t nxt_dist;
				if (is_special[cur]) {
					nxt_comp = C++;
					comp_verts.push_back({cur, -1});
					nxt_dist = 0;
				} else {
					nxt_comp = cur_comp;
					nxt_dist = dist + lengths[e];
				}
				comp_id[e] = nxt_comp;
				self(nxt, cur, nxt_comp, nxt_dist);
			}
			en[cur] = nxt_idx;
		})(0, -1, -1, 0);
		assert(nxt_idx == V);
	}
	//cerr << "number of components: " << C << '\n';

	std::vector<std::array<std::vector<std::pair<int64_t, int>>, 2>> comp_sorted(C);
	for (int rt = 0; rt < V; rt++) {
		if (is_special[rt]) {
			//cerr << "special vert " << rt << '\n';
			for (auto [rt_e, rt_dir] : adj[rt]) {
				int comp = comp_id[rt_e];
				int z = comp_verts[comp][1] == rt ? 1 : 0;
				std::y_combinator([&](auto self, int cur, int prv, int64_t dist) -> void {
					if (is_special[cur]) return;
					if (cur < N_orig) {
						comp_sorted[comp][z].push_back({dist, cur});
					}
					for (auto [e, nxt] : adj[cur]) {
						if (nxt == prv) continue;
						self(nxt, cur, dist + lengths[e]);
					}
				})(rt_dir, rt, lengths[rt_e]);
				std::sort(comp_sorted[comp][z].begin(), comp_sorted[comp][z].end());
			}
		}
	}

	std::vector<std::pair<int64_t, int>> buf1, buf2; buf1.reserve(V), buf2.reserve(V);

	auto update = [&](int e, int w) -> void {
		int old_w = lengths[e];
		int64_t delta = int64_t(w) - int64_t(old_w);
		lengths[e] = w;

		int comp = comp_id[e];
		auto [u, v] = edge_verts[e];
		auto [top, bot] = comp_verts[comp];
		bool in_path = bot != -1 && (st[v] <= st[bot] && st[bot] < en[v]);
		if (in_path) {
			comp_len[comp] += delta;
		}

		for (int z = 0; z < 2; z++) {
			auto& vec = comp_sorted[comp][z];
			buf1.clear();
			buf2.clear();
			for (auto it : vec) {
				int x = it.second;
				bool in_subtree = (st[v] <= st[x] && st[x] < en[v]);
				bool does_change;
				if (z == 0) {
					does_change = in_subtree;
				} else {
					if (in_path) {
						does_change = !in_subtree;
					} else {
						does_change = in_subtree;
					}
				}
				if (does_change) {
					it.first += delta;
					buf2.push_back(it);
				} else {
					buf1.push_back(it);
				}
			}
			std::merge(buf1.begin(), buf1.end(), buf2.begin(), buf2.end(), vec.begin());
		}
	};

	auto query_comp = std::y_combinator([&](auto self, int cur, int prv_comp, int64_t max_dist) -> int {
		assert(is_special[cur]);
		if (max_dist < 0) return 0;
		int ans = (cur < N_orig);
		for (auto [e, _] : adj[cur]) {
			int comp = comp_id[e];
			if (comp == prv_comp) continue;
			int z = comp_verts[comp][1] == cur ? 1 : 0;
			//cerr << "checking comp " << comp << " from " << cur << " to " << comp_verts[comp][!z] << '\n';
			const auto& vec = comp_sorted[comp][z];
			int cur_cnt = int(std::lower_bound(vec.begin(), vec.end(), max_dist, [](const auto& a, int64_t v) { return a.first <= v; }) - vec.begin());
			ans += cur_cnt;
			int nxt = comp_verts[comp][!z];
			if (nxt == -1) continue;
			ans += self(nxt, comp, max_dist - comp_len[comp]);
		}
		return ans;
	});

	auto query_vert = std::y_combinator([&](auto self, int cur, int prv_e, int64_t max_dist) -> int {
		if (max_dist < 0) return 0;
		if (is_special[cur]) {
			return query_comp(cur, prv_e == -1 ? -1 : comp_id[prv_e], max_dist);
		}
		int ans = (cur < N_orig);
		for (auto [e, nxt] : adj[cur]) {
			if (e == prv_e) continue;
			ans += self(nxt, e, max_dist - lengths[e]);
		}
		return ans;
	});

	auto query = [&](int x, int64_t d) -> int {
		//cerr << "query " << x << ' ' << d << '\n';
		// dfs from x to a special vertex
		return query_vert(x, -1, d);
	};

	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int e, w; cin >> e >> w; e--;
			update(e, w);
		} else if (op == 2) {
			int x; int64_t d; cin >> x >> d; x--;
			cout << query(x, d) << '\n';
		}
	}

	return 0;
}
