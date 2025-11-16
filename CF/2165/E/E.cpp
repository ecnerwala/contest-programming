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
		std::vector<int> deg_freq(N);
		for (int i = 0; i < N; i++) {
			deg_freq[cur_deg[i] = int(adj[i].size())]++;
		}
		int edge_cnt = 1;
		std::vector<int> ans; ans.reserve(N-1);
		ans.push_back(1);
		std::vector<int> leaves; leaves.reserve(N);
		std::vector<int> nleaves; nleaves.reserve(N);
		for (int i = 0; i < N; i++) if (cur_deg[i] == 1) leaves.push_back(i);
		std::vector<bool> alive(N, true);
		int max_deg = N-1;
		for (int colors = 2; int(ans.size()) < N-1; colors++) {
			if (colors % 2 == 0) {
				while (!deg_freq[max_deg]) --max_deg;
				edge_cnt += max_deg - 1;
			} else {
				edge_cnt += max_deg - 1;
				// now add in the number of leaves
				edge_cnt += int(leaves.size());
				for (int v : leaves) {
					assert(alive[v]);
					alive[v] = false;
					assert(cur_deg[v] == 1);
					deg_freq[cur_deg[v]]--;
					int neighbor = -1;
					for (int nxt : adj[v]) {
						if (alive[nxt]) {
							assert(neighbor == -1);
							neighbor = nxt;
						}
					}
					assert(neighbor != -1);
					assert(alive[neighbor]);
					deg_freq[cur_deg[neighbor]]--;
					cur_deg[neighbor]--;
					deg_freq[cur_deg[neighbor]]++;
					if (cur_deg[neighbor] == 1) nleaves.push_back(neighbor);
				}
				leaves.clear();
				std::swap(leaves, nleaves);
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
