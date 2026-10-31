#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<std::pair<std::array<int, 2>, int>> edges(M);
	for (auto& [vs, w] : edges) {
		auto& [u, v] = vs;
		cin >> u >> v >> w; u--, v--;
	}

	const int C = N+1;
	std::vector<std::vector<int>> by_color(C);
	for (int e = 0; e < M; e++) {
		by_color[edges[e].second].push_back(e);
	}

	std::vector<int> color_edge(C, -1);

	int ans = 0;
	while (true) {
		std::vector<int> depth(N, -1);
		std::vector<int> cc_id(N, -1);
		std::vector<std::pair<int, int>> par(N, {-1, -1});
		{
			std::vector<std::vector<std::pair<int, int>>> adj(N);
			for (int c = 0; c < ans; c++) {
				int e = color_edge[c];
				assert(e != -1);
				auto [u, v] = edges[e].first;
				adj[u].push_back({v, c});
				adj[v].push_back({u, c});
			}
			std::vector<int> q; q.reserve(N);
			for (int st = 0; st < N; st++) {
				if (cc_id[st] != -1) continue;
				cc_id[st] = st;
				depth[st] = 0;
				par[st] = {-1, -1};
				q.push_back(st);

				for (int z = 0; z < int(q.size()); z++) {
					int cur = q[z];
					for (auto [nxt, e] : adj[cur]) {
						if (cc_id[nxt] != -1) continue;
						cc_id[nxt] = st;
						depth[nxt] = depth[cur] + 1;
						par[nxt] = {cur, e};
						q.push_back(nxt);
					}
				}
				q.clear();
			}
		}

		{
			std::vector<int> prv(ans+1, -1);
			std::vector<int> q; q.reserve(ans+1);
			q.push_back(ans);
			int final_edge = -1;
			for (int z = 0; z < int(q.size()); z++) {
				int color = q[z];
				for (int e : by_color[color]) {
					if (e == color_edge[color]) continue;

					auto [u, v] = edges[e].first;
					if (cc_id[u] != cc_id[v]) {
						final_edge = e;
						goto found;
					}
					while (u != v) {
						while (par[u].second == -2) {
							if (par[par[u].first].second == -2) {
								par[u].first = par[par[u].first].first;
							}
							u = par[u].first;
						}
						while (par[v].second == -2) {
							if (par[par[v].first].second == -2) {
								par[v].first = par[par[v].first].first;
							}
							v = par[v].first;
						}
						if (u == v) break;
						if (depth[v] > depth[u]) {
							std::swap(u, v);
						}

						assert(par[u].first != -1);
						int nc = par[u].second;
						par[u].second = -2;
						assert(prv[nc] == -1);
						prv[nc] = e;
						q.push_back(nc);
					}
				}
			}
			assert(final_edge == -1);
			break;
found:
			// backtrack from final edge
			assert(final_edge != -1);
			for (int cur = final_edge; cur != -1; ) {
				int c = edges[cur].second;
				color_edge[c] = cur;
				cur = prv[c];
				if (cur == -1) assert(c == ans);
			}
			ans++;
		}
	}

	cout << ans << '\n';

	return 0;
}
