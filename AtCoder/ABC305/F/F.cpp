#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;

	std::vector<std::vector<int>> adj(N);
	std::vector<int> par(N, -1);
	std::vector<int> nxt_idx(N, 0);

	par[0] = 0;

	int cur = 0;
	while (true) {
		if (cur == N-1) {
			string s; cin >> s;
			assert(s == "OK");
			exit(0);
		}
		int k; cin >> k;
		adj[cur].resize(k);
		for (auto& nxt : adj[cur]) { cin >> nxt; nxt--; }

		while (true) {
			if (nxt_idx[cur] == int(adj[cur].size())) {
				// go to parent
				cout << par[cur]+1 << '\n' << flush;
				cur = par[cur];
				break;
			}
			int nxt = adj[cur][nxt_idx[cur]++];
			if (par[nxt] == -1) {
				par[nxt] = cur;
				cout << nxt+1 << '\n' << flush;
				cur = nxt;
				break;
			}
		}
	}

	return 0;
}
