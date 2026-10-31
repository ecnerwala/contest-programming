#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N_CASES; cin >> N_CASES;
	while (N_CASES--) {
		int N; cin >> N;
		struct edge_t {
			int64_t w; std::array<int, 2> v;
		};
		std::vector<edge_t> edges(N-1);
		std::vector<int64_t> deg(N);
		for (auto& [w, vs] : edges) {
			for (auto& v : vs) { cin >> v; v--; }
			cin >> w;
			for (auto v : vs) deg[v] += w;
		}

		// sort by decreasing edge weight
		std::sort(edges.begin(), edges.end(), [&](edge_t a, edge_t b) { return a.w > b.w; });

		struct heap_t {
			int64_t v;
			int64_t delta;
			std::array<heap_t*, 2> c = {nullptr, nullptr};
		};
		auto meld = [&](this auto&& self, heap_t* a, heap_t* b) -> heap_t* {
			if (!a) return b;
			if (!b) return a;
			if (a->v > b->v) {
				std::swap(a, b);
			}
			std::swap(a->c[0], a->c[1]);
			a->c[0] = self(a->c[0], b);
			return a;
		};
		std::vector<heap_t> heap_nodes(4*N);
		int NODES = 0;
		auto new_heap_node = [&](int64_t v, int64_t delta) -> heap_t* {
			assert(NODES < int(heap_nodes.size()));
			return &(heap_nodes[NODES++] = {v, delta, {nullptr, nullptr}});
		};
		std::vector<heap_t*> heaps(N);
		for (int i = 0; i < N; i++) {
			heaps[i] = new_heap_node(deg[i], 1);
		}
		std::vector<int> par(N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		for (auto [w, vs] : edges) {
			for (auto& v : vs) v = get_par(v);
			auto [u, v] = vs;
			if (par[u] > par[v]) std::swap(u, v);
			par[u] += par[v];
			par[v] = u;

			heaps[u] = meld(heaps[u], heaps[v]);
			deg[u] += deg[v] - 2 * w;

			assert(heaps[u]);
			if (deg[u] < heaps[u]->v) {
				int64_t cur_pos = heaps[u]->v;
				int64_t cur_slope = 0;
				int64_t cur_val = 0;
				assert(cur_val - cur_pos < -deg[u]);
				while (true) {
					assert(cur_val - cur_pos < -deg[u]);
					if (!heaps[u] || cur_val + cur_slope * (heaps[u]->v - cur_pos) - heaps[u]->v >= -deg[u]) {
						// we exceed it within cur_slope
						assert(cur_slope > 1);
						int64_t nx = cur_pos + (cur_pos - cur_val - deg[u]) / (cur_slope - 1);
						int64_t gap = (cur_pos - cur_val - deg[u]) % (cur_slope - 1);
						// push (cur_slope - 1 - gap) at nx and gap at nx+1
						if (gap) heaps[u] = meld(new_heap_node(nx+1, gap), heaps[u]);
						heaps[u] = meld(new_heap_node(nx, cur_slope - 1 - gap), heaps[u]);
						heaps[u] = meld(new_heap_node(deg[u], 1), heaps[u]);
						break;
					} else {
						cur_val += cur_slope * (heaps[u]->v - cur_pos);
						cur_pos = heaps[u]->v;
						cur_slope += heaps[u]->delta;
						heaps[u] = meld(heaps[u]->c[0], heaps[u]->c[1]);
					}
				}
			}
		}

		int Q; cin >> Q;
		while (Q--) {
			int64_t t; cin >> t;
			int64_t ans = 0;
			[&](this auto&& self, int cur, int prv, int64_t par_w) -> int64_t {
				int64_t cur_sum = 0;
				for (auto [nxt, w] : adj[cur]) {
					if (nxt == prv) continue;
					cur_sum += self(nxt, cur, w);
				}
				// if cur_sum >= t, it doesn't matter either, we just don't get anything new
				int64_t e_top = std::max(cur_sum, t - par_w);
				if (e_top > par_w) {
					ans += e_top - par_w;
					return par_w;
				} else {
					return e_top;
				}
			}(0, -1, 0);
			cout << ans << '\n';
		}
	}

	return 0;
}
