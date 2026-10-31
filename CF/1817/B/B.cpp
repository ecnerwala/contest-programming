#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;

		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		std::vector<int> prv(N, -1);
		std::vector<int> q; q.reserve(N);
		std::vector<std::array<int, 2>> edges; edges.reserve(N);
		for (int st = 0; st < N; st++) {
			if (adj[st].size() < 4) continue;

			for (int i = 0; i < N; i++) {
				prv[i] = -1;
			}
			for (auto v : adj[st]) {
				prv[v] = st;
			}

			for (auto v0 : adj[st]) {
				q.clear();
				q.push_back(v0);
				for (int z = 0; z < int(q.size()); z++) {
					int cur = q[z];
					for (int nxt : adj[cur]) {
						if (nxt == st) {
							assert(cur == v0);
							continue;
						}
						if (nxt != v0 && prv[nxt] == st) {
							// we found something
							edges.push_back({st, nxt});
							edges.push_back({cur, nxt});
							for (int v = cur; v != st; v = prv[v]) {
								edges.push_back({v, prv[v]});
							}
							int num_needed = 2;
							for (int v : adj[st]) {
								if (v == nxt) continue;
								if (v == v0) continue;
								if (num_needed > 0) {
									edges.push_back({st, v});
									num_needed--;
								}
							}
							goto done;
						}

						if (prv[nxt] == -1) {
							prv[nxt] = cur;
							q.push_back(nxt);
						}
					}
				}
			}
		}
		cout << "NO" << '\n';
		continue;
done:
		cout << "YES" << '\n';
		cout << edges.size() << '\n';
		for (auto [u, v] : edges) {
			cout << u+1 << ' ' << v+1 << '\n';
		}
	}

	return 0;
}
