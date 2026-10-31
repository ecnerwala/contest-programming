#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, MOD; cin >> N >> MOD;
	vector<vector<int>> V(MOD);
	for (int i = 0; i < N; i++) {
		int a; cin >> a; a++; a %= MOD;
		V[a].push_back(i);
	}
	int cost = 0;
	vector<int> ans; ans.reserve(N);

	priority_queue<pair<int, int>> pq;
	for (int v = 0; v < MOD; v++) {
		if (!V[v].empty()) { pq.push({int(V[v].size()), v}); }
	}
	int cur = 0;
	while (!pq.empty()) {
		int v = pq.top().second; pq.pop();
		if ((cur + v) % MOD == MOD-1 && !pq.empty()) {
			int u = pq.top().second; pq.pop();
			pq.push({int(V[v].size()), v});
			v = u;
		}

		ans.push_back(V[v].back());
		V[v].pop_back();
		if (!V[v].empty()) pq.push({int(V[v].size()), v});

		cur += v;
		cur %= MOD;
		if (int(ans.size()) < N && cur == MOD-1) {
			cost++;
			cur = 0;
		}
	}

	cout << cost << '\n';
	for (int z = 0; z < N; z++) cout << ans[z]+1 << " \n"[z+1==N];

	return 0;
}
