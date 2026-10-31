#include <bits/stdc++.h>
#include <random>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::mt19937_64 mt(48);
		vector<uint64_t> msks(N);
		uint64_t tot_xor = 0;
		for (auto& v : msks) {
			v = mt();
			tot_xor ^= v;
		}

		vector<pair<uint64_t, int>> adj(N);
		for (int i = 0; i < N; i++) adj[i].second = i;

		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].first ^= msks[v];
			adj[v].first ^= msks[u];
		}
		sort(adj.begin(), adj.end());
		int ans = 0;
		string example(N, '0');
		for (int i = 0; i < N; ) {
			uint64_t cur_xor = 0;
			int j = i; while (j < N && adj[j].first == adj[i].first) {
				cur_xor ^= msks[adj[j].second];
				j++;
			}
			if ((cur_xor ^ adj[i].first) == tot_xor) {
				if (!ans) {
					for (int k = i; k < j; k++) example[adj[k].second] = '1';
				}
				ans++;
			}
			i = j;
		}
		cout << ans << '\n';
		cout << example << '\n';
	}

	return 0;
}
