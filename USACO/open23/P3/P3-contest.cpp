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

	int N; cin >> N;
	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	std::vector<int> sz(N);
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		if (prv != -1) {
			auto it = std::find(adj[cur].begin(), adj[cur].end(), prv);
			assert(it != adj[cur].end());
			adj[cur].erase(it);
		}

		sz[cur] = 1;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur);
			sz[cur] += sz[nxt];
		}

		for (int z = 0; z < int(adj[cur].size()); z++) {
			if (sz[adj[cur][z]] > sz[adj[cur][0]]) {
				std::swap(adj[cur][z], adj[cur][0]);
			}
		}
	})(0, -1);

	/*
	std::vector<int> preorder_idx(N);
	int nxt_idx = 0;
	std::vector<int> heavy_root(N);
	std::vector<int> heavy_par(N);
	std::y_combinator([&](auto self, int cur, int prv, int h_par, int h_root) -> void {
		int v = nxt_idx; nxt_idx++;
		preorder_idx[cur] = v;
		if (h_root == -1) h_root = v;
		heavy_par[v] = h_par;
		heavy_root[v] = h_root;

		for (int z = 0; z < int(adj[cur].size()); z++) {
			if (sz[adj[cur][z]] > sz[adj[cur][0]]) {
				std::swap(adj[cur][z], adj[cur][0]);
			}
		}
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			self(nxt, cur, h_par, h_root);
			h_par = v;
			h_root = -1;
		}
	})(0, -1, -1, -1);

	std::vector<int> heavy_tail(N);
	for (int i = N-1; i >= 0; i--) {
		heavy_tail[i] = (i+1 < N && heavy_root[i+1] != heavy_root[i]) ? i : heavy_tail[i+1];
	}
	*/

	struct c_seg_t {
		bool is_open;
		int64_t through_1;
		int64_t top_1;
		int64_t bot_1;
		int64_t top_2;
		int64_t bot_2;
		int64_t full_2;
		int64_t full_3;
	};
	auto merge_compress = [&](const c_seg_t& a, const c_seg_t& b) -> c_seg_t {
		c_seg_t r;
		r.is_open = a.is_open && b.is_open;
		r.through_1 = a.through_1 * b.is_open + a.is_open * b.through_1;
		r.top_1 = a.top_1 + a.is_open * b.top_1;
		r.bot_1 = b.bot_1 + b.is_open * a.bot_1;

		// x -- y -- top
		r.top_2 =
			a.top_2 // x in a, y in a
			+ a.bot_1 * b.top_1 * a.is_open // x in a, y in b
			+ b.top_1 * a.through_1 // x in b, y in a
			+ b.top_2 * a.is_open; // x in b, y in b
		r.bot_2 =
			b.bot_2 // x in b, y in b
			+ b.top_1 * a.bot_1 * b.is_open // x in b, y in a
			+ a.bot_1 * b.through_1 // x in a, y in b
			+ a.bot_2 * b.is_open; // x in a, y in a

		r.full_2 = a.full_2 + b.full_2 + a.bot_1 * b.top_1 * 2;
		r.full_3 = a.full_3 + b.full_3
			+ a.bot_2 * b.top_1 * 2 + a.bot_1 * b.top_2 * 2
			+ a.bot_1 * b.top_1 * a.bot_1 + b.top_1 * a.bot_1 * b.top_1;

		return r;
	};

	struct r_seg_t {
		int64_t top_1;
		int64_t top_2;
		int64_t cross_top_2;
		int64_t full_2;
		int64_t full_3;
		int64_t cross_full_2;
		int64_t cross_full_3;
	};
	auto merge_rake = [&](const r_seg_t& a, const r_seg_t& b) -> r_seg_t {
		r_seg_t r;
		r.top_1 = a.top_1 + b.top_1;
		r.top_2 = a.top_2 + b.top_2;
		r.cross_top_2 = a.cross_top_2 + b.cross_top_2 + a.top_1 * b.top_1 * 2;
		r.full_2 = a.full_2 + b.full_2;
		r.full_3 = a.full_3 + b.full_3;
		r.cross_full_2 = a.cross_full_2 + b.cross_full_2 + a.top_1 * b.top_1 * 2;
		r.cross_full_3 = a.cross_full_3 + b.cross_full_3 + a.top_1 * b.cross_top_2 * 2 + a.cross_top_2 * b.top_1 * 2 + a.top_1 * b.top_1 * (a.top_1 + b.top_1);
		return r;
	};

	auto empty_rake = [&]() -> r_seg_t {
		r_seg_t r;
		r.top_1 = 0;
		r.top_2 = 0;
		r.cross_top_2 = 0;
		r.full_2 = 0;
		r.full_3 = 0;
		r.cross_full_2 = 0;
		r.cross_full_3 = 0;
		return r;
	};

	auto rake_to_compress = [&](const r_seg_t& a, bool is_alive) -> c_seg_t {
		c_seg_t r;
		r.is_open = !is_alive;
		r.through_1 = is_alive ? 1 : a.top_1;
		r.top_1 = r.bot_1 = is_alive ? 1 : a.top_1;
		r.top_2 = r.bot_2 = is_alive ? a.top_1 : a.cross_top_2;

		r.full_2 = is_alive ? 2 * a.top_1 + a.full_2 : a.cross_full_2;
		r.full_3 = is_alive ? 2 * a.top_2 + a.top_1 + a.top_1 * a.top_1 + a.full_3 : a.cross_full_3;
		return r;
	};
	auto compress_to_rake = [&](const c_seg_t& a) -> r_seg_t {
		r_seg_t r;
		r.top_1 = a.top_1;
		r.top_2 = r.cross_top_2 = a.top_2;
		r.full_2 = r.cross_full_2 = a.full_2;
		r.full_3 = r.cross_full_3 = a.full_3;
		return r;
	};

	// there are 4 types of nodes:
	// 0 = vertices = rake->compress
	// 1 = heavy-edges = compress-merge
	// 2 = light-edges = compress->rake
	// 3 = rake-merge
	struct node_t {
		int p = -1;
		std::array<int, 2> c;
		int type;
		bool is_alive;
		union {
			c_seg_t c_dat;
			r_seg_t r_dat;
		};
	};
	std::vector<node_t> nodes; nodes.reserve(4*N);
	std::vector<int> node_id(N);

	auto update_node = [&](int n) -> void {
		auto c = nodes[n].c;
		if (nodes[n].type == 0) {
			assert(c[0] == c[1]);
			if (c[0] == -1) {
				nodes[n].c_dat = rake_to_compress(empty_rake(), nodes[n].is_alive);
			} else {
				nodes[n].c_dat = rake_to_compress(nodes[c[0]].r_dat, nodes[n].is_alive);
			}
		} else if (nodes[n].type == 1) {
			nodes[n].c_dat = merge_compress(nodes[c[0]].c_dat, nodes[c[1]].c_dat);
		} else if (nodes[n].type == 2) {
			assert(c[0] == c[1]);
			nodes[n].r_dat = compress_to_rake(nodes[c[0]].c_dat);
		} else if (nodes[n].type == 3) {
			nodes[n].r_dat = merge_rake(nodes[c[0]].r_dat, nodes[c[1]].r_dat);
		} else assert(false);
	};

	// this builds up a compress node
	int root_vert = std::y_combinator([&](auto self, int cur) -> int {
		std::vector<int> cur_chain;
		cur_chain.push_back(cur);
		while (!adj[cur_chain.back()].empty()) {
			cur_chain.push_back(adj[cur_chain.back()].front());
		}
		std::vector<int> verts; verts.reserve(cur_chain.size());
		for (int v : cur_chain) {
			std::vector<int> light_rakes; light_rakes.reserve(adj[v].size());
			for (int z = 1; z < int(adj[v].size()); z++) {
				int u = adj[v][z];
				int c = self(u);
				int r = int(nodes.size());
				nodes.emplace_back();
				nodes[c].p = r;
				nodes[r].c = {c, c};
				nodes[r].type = 2;
				update_node(r);
				light_rakes.push_back(r);
			}
			while (light_rakes.size() > 1) {
				int l = 0;
				for (int i = 0; i < int(light_rakes.size()); i += 2) {
					if (i+1 == int(light_rakes.size())) {
						light_rakes[l++] = light_rakes[i];
					} else {
						int m = int(nodes.size());
						nodes.emplace_back();
						nodes[m].c[0] = light_rakes[i];
						nodes[m].c[1] = light_rakes[i+1];
						nodes[nodes[m].c[0]].p = m;
						nodes[nodes[m].c[1]].p = m;
						nodes[m].type = 3;
						update_node(m);

						light_rakes[l++] = m;
					}
				}
				light_rakes.resize(l);
			}
			int n = int(nodes.size());
			nodes.emplace_back();
			node_id[v] = n;
			nodes[n].type = 0;
			nodes[n].is_alive = true;
			if (light_rakes.empty()) {
				nodes[n].c = {-1, -1};
			} else {
				nodes[light_rakes[0]].p = n;
				nodes[n].c = {light_rakes[0], light_rakes[0]};
			}
			update_node(n);
			verts.push_back(n);
		}
		assert(!verts.empty());
		while (verts.size() > 1) {
			int l = 0;
			for (int i = 0; i < int(verts.size()); i += 2) {
				if (i+1 == int(verts.size())) {
					verts[l++] = verts[i];
				} else {
					int m = int(nodes.size());
					nodes.emplace_back();
					nodes[m].c[0] = verts[i];
					nodes[m].c[1] = verts[i+1];
					nodes[nodes[m].c[0]].p = m;
					nodes[nodes[m].c[1]].p = m;
					nodes[m].type = 1;
					update_node(m);

					verts[l++] = m;
				}
			}
			verts.resize(l);
		}
		return verts[0];
	})(0);

	auto dump_tree = [&]() -> void {
		for (int i = 0; i < int(nodes.size()); i++) {
			cerr << "Node " << i << " type = " << nodes[i].type << ", p = " << nodes[i].p << ", c = " << nodes[i].c[0] << '+' << nodes[i].c[1] << '\n';
			if (nodes[i].type <= 1) {
				const auto& d = nodes[i].c_dat;
				cerr << d.is_open << ' ' << d.through_1 << ' ' << d.top_1 << '/' << d.bot_1 << ' ' << d.top_2 << '/' << d.bot_2 << ' ' << d.full_2 << ' ' << d.full_3 << '\n';
			} else {
				const auto& d = nodes[i].r_dat;
				cerr << d.top_1 << ' ' << d.top_2 << '/' << d.cross_top_2 << ' ' << d.full_2 << '/' << d.cross_full_2 << ' ' << d.full_3 << '/' << d.cross_full_3 << '\n';
			}
		}
		cerr << '\n';
	};

	auto get_ans = [&]() -> int64_t {
		const auto& n = nodes[root_vert].c_dat;
		//cerr << n.full_2 << ' ' << n.full_3 << '\n';
		return n.full_3 - n.full_2;
	};

	auto erase_vert = [&](int n) -> void {
		nodes[n].is_alive = false;
		for (int cur = n; true; cur = nodes[cur].p) {
			update_node(cur);
			if (cur == root_vert) break;
		}
	};

	//dump_tree();
	for (int i = 0; i < N; i++) {
		cout << get_ans() << '\n';
		erase_vert(node_id[i]);
	}

	return 0;
}
