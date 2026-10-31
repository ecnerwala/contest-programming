#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<vector<int>> adj(N);
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].emplace_back(v);
		adj[v].emplace_back(u);
	}
	int ans = 0;
	for (int i = 0; i < N; i++) ans += (adj[i].size() <= 1);
	cout << (ans+1) / 2 << '\n';

	return 0;
}
