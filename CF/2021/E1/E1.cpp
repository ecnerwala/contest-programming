#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, P; cin >> N >> M >> P;
		std::vector<int64_t> deltas; deltas.reserve(N);

		struct vertex_t {
			int64_t first_edge = 0;
			int weight = 0;
			int cnt = 0;
		};
		std::vector<vertex_t> verts(N);
		for (int z = 0; z < P; z++) {
			int v; cin >> v; v--;
			verts[v].cnt ++;
		}

		std::vector<std::pair<int, std::array<int, 2>>> edges(M);
		for (auto& [w, vs] : edges) {
			cin >> vs[0] >> vs[1] >> w; vs[0]--, vs[1]--;
		}
		std::sort(edges.begin(), edges.end());

		std::vector<int> par(2*N-1, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) {
					par[a] = par[par[a]];
				}
				a = par[a];
			}
			return a;
		};
		for (auto [w, vs] : edges) {
			auto [u, v] = vs;
			u = get_par(u), v = get_par(v);
			if (u == v) continue;
			if (par[u] > par[v]) std::swap(u, v);
			par[u] += par[v];
			par[v] = u;

			assert(w >= verts[u].weight);
			verts[u].first_edge += int64_t(w - verts[u].weight) * verts[u].cnt;

			assert(w >= verts[v].weight);
			verts[v].first_edge += int64_t(w - verts[v].weight) * verts[v].cnt;

			verts[u].weight = w;

			deltas.push_back(std::min(verts[u].first_edge, verts[v].first_edge));
			verts[u].first_edge = std::max(verts[u].first_edge, verts[v].first_edge);

			verts[u].cnt += verts[v].cnt;
		}
		int rt = get_par(0);
		deltas.push_back(verts[rt].first_edge);
		assert(verts[rt].cnt == P);
		int64_t ans = int64_t(verts[rt].weight) * verts[rt].cnt;
		std::sort(deltas.begin(), deltas.end(), std::greater<>());
		assert(int(deltas.size()) == N);
		for (int i = 0; i < N; i++) {
			ans -= deltas[i];
			cout << ans << " \n"[i+1==N];
		}
	}

	return 0;
}
