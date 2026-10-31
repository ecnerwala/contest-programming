#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::string> G(N);
		for (auto& r : G) cin >> r;

		// If it's already connected, ans is 0
		// If there's a lone node or a non-clique, ans is 1
		// If >= 3 cliques, then ans is 2
		// If == 2 cliques, then it's smaller of the 2

		auto ans = [&]() -> std::vector<int> {
			std::vector<std::vector<int>> cliques; cliques.reserve(3);
			std::vector<int> deg(N);
			std::vector<bool> vis(N);
			std::vector<int> q; q.reserve(N);
			for (int i = 0; i < N; i++) {
				if (vis[i]) continue;
				int num_e = 0;
				int num_v = 0;
				vis[i] = true;
				q.clear();
				q.push_back(i);
				for (int z = 0; z < int(q.size()); z++) {
					int cur = q[z];
					num_v++;
					deg[cur] = 0;
					for (int nxt = 0; nxt < N; nxt++) {
						if (G[cur][nxt] != '1') continue;
						deg[cur] ++;
						if (vis[nxt]) continue;
						vis[nxt] = true;
						q.push_back(nxt);
					}
					num_e += deg[cur];
				}
				if (num_v == N) {
					return std::vector<int>({});
				}
				if (num_v == 1) {
					return std::vector<int>({q[0]});
				}
				bool is_clique = num_e == num_v * (num_v - 1);
				if (!is_clique) {
					if (deg[q.back()] != num_v-1) {
						// back of the bfs tree is fine, since it'll connect to at least 1 other thing
						return std::vector<int>({q.back()});
					}
					// there's at least one thing of full degree, so anything not full degree is fine
					for (int cur : q) {
						if (deg[cur] != num_v-1) {
							return std::vector<int>({cur});
						}
					}
				}
				if (cliques.size() <= 2) {
					cliques.push_back(q);
				}
			}
			if (cliques.size() > 2) {
				return std::vector<int>({cliques[0][0], cliques[1][0]});
			}
			assert(cliques.size() == 2);
			return (cliques[0].size() < cliques[1].size()) ? cliques[0] : cliques[1];
		}();

		cout << ans.size() << '\n';
		if (!ans.empty()) {
			for (int z = 0; z < int(ans.size()); z++) {
				cout << ans[z]+1 << " \n"[z+1==int(ans.size())];
			}
		}
	}

	return 0;
}
