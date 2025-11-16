#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::vector<int> cur_deg(N);
		std::set<std::pair<int, int>> deg;
		for (int i = 0; i < N; i++) {
			deg.insert({cur_deg[i] = int(adj[i].size()), i});
		}
		int edge_cnt = 1;
		std::vector<int> ans; ans.reserve(N-1);
		ans.push_back(1);
		std::vector<int> leaves; leaves.reserve(N);
		std::vector<bool> alive(N, true);
		for (int colors = 2; int(ans.size()) < N-1; colors++) {
			assert(!deg.empty());
			if (colors % 2 == 0) {
				edge_cnt += deg.rbegin()->first - 1;
			} else {
				edge_cnt -= deg.rbegin()->first - 1;
				// now add in the number of leaves
				while (!deg.empty() && deg.begin()->first <= 1) {
					assert(deg.begin()->first != 0);
					leaves.push_back(deg.begin()->second);
					deg.erase(deg.begin());
				}
				edge_cnt += int(leaves.size());
				for (int v : leaves) {
					assert(alive[v]);
					alive[v] = false;
					int neighbor = -1;
					for (int nxt : adj[v]) {
						if (alive[nxt]) {
							assert(neighbor == -1);
							neighbor = nxt;
						}
					}
					assert(neighbor != -1);
					assert(alive[neighbor]);
					deg.erase({cur_deg[neighbor], neighbor});
					cur_deg[neighbor]--;
					deg.insert({cur_deg[neighbor], neighbor});
				}
				leaves.clear();
			}
			while (int(ans.size()) < edge_cnt) {
				ans.push_back(colors);
			}
			assert(int(ans.size()) == edge_cnt);
		}
		for (int i = 0; i < N-1; i++) {
			cout << ans[i] << " \n"[i+1==N-1];
		}
	}

	return 0;
}
