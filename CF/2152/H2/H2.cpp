#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N_CASES; cin >> N_CASES;
	while (N_CASES--) {
		int N; cin >> N;
		std::vector<int64_t> deg(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; int64_t w; cin >> u >> v >> w; u--, v--;
			deg[u] += w;
			deg[v] += w;
		}

		int Q; cin >> Q;
		while (Q--) {
			int64_t t; cin >> t;
			int64_t ans = 0;
			for (int i = 0; i < N; i++) {
				ans += std::max(t, deg[i]) - deg[i];
			}
			cout << ans << '\n';
		}
	}

	return 0;
}
