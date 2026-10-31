#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int J, C, A, Q; cin >> J >> C >> A >> Q; A--, Q--;

		std::vector<std::vector<int>> adj(J);
		std::vector<int> deg(J, 0);
		std::vector<int> tot_out(J, 0);
		for (int e = 0; e < C; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);

			deg[u]++;
			deg[v]++;
			tot_out[u] ^= v;
			tot_out[v] ^= u;
		}

		auto solve = [&]() -> std::optional<int> {
			auto bfs = [&](int st) -> std::vector<int> {
				std::vector<int> q; q.reserve(J);
				std::vector<int> dist(J, -1);
				dist[st] = 0; q.push_back(st);
				for (int z = 0; z < int(q.size()); z++) {
					int cur = q[z];
					for (int nxt : adj[cur]) {
						if (dist[nxt] == -1) {
							dist[nxt] = dist[cur]+1;
							q.push_back(nxt);
						}
					}
				}
				return dist;
			};

			auto distQ = bfs(Q);
			if (distQ[A] == -1) {
				return std::nullopt;
			}
			auto distA = bfs(A);

			std::vector<bool> is_safe(J, true);
			{
				std::vector<int> q; q.reserve(J);
				for (int i = 0; i < J; i++) {
					if (deg[i] <= 1) q.push_back(i);
				}
				for (int z = 0; z < int(q.size()); z++) {
					int cur = q[z];
					is_safe[cur] = false;
					if (deg[cur] == 1) {
						int nxt = tot_out[cur];
						assert(nxt != cur);
						deg[cur] --;
						deg[nxt] --;
						tot_out[cur] = 0;
						tot_out[nxt] ^= cur;
						if (deg[nxt] == 1) {
							q.push_back(nxt);
						}
					}
				}
			}
			int ans = distQ[A];
			for (int i = 0; i < J; i++) {
				if (distQ[i] == -1) continue;
				if (distA[i] < distQ[i]) {
					if (is_safe[i]) return std::nullopt;
					ans = std::max(ans, distQ[i]);
				}
			}
			return ans;
		};

		auto res = solve();

		cout << "Case #" << case_num << ": ";
		if (res) {
			cout << 2 * (*res) << '\n';
		} else {
			cout << "SAFE" << '\n';
		}
	}

	return 0;
}
