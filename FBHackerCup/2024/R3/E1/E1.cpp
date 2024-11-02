#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		cerr << "Case #" << case_num << " " << "N = " << N << '\n';
		assert(N >= 2);
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		bool good = true;
		[&](this auto&& self, int cur, int prv) -> void {
			assert(prv < cur);
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				if (nxt > cur) {
					self(nxt, cur);
				} else {
					if (adj[nxt].size() == 1) {
						assert(adj[nxt][0] == cur);
						assert(prv < cur);
						if (prv == -1) {
							assert(cur == 0);
						} else if (adj[prv].size() == 1) {
								assert(prv == 0);
								// still good
						} else {
							good = false;
						}
					} else {
						// node 1 won't see nxt's children, which is bad
						good = false;
					}
				}
			}
		}(0, -1);

		cout << "Case #" << case_num << ": " << (good ? "Lucky" : "Wrong") << '\n';
	}

	return 0;
}
