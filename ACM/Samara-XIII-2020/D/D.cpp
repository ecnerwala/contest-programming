#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, M; cin >> N >> M;
	vector<vector<pair<char, int>>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; char c; cin >> u >> v >> c; u--, v--;
		adj[u].emplace_back(c, v);
		adj[v].emplace_back(c, u);
	}

	vector<array<int, 26>> trans;

	vector<int> vis(N, -1);
	vector<pair<int, char>> prv(N);
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> q;
	vis[0] = true;
	q.emplace(0, 0);

	int lastVal = -1;

	while (!q.empty()) {
		int val = q.top().first;
		int cur = q.top().second;
		q.pop();
		if (val > vis[cur]) continue;
		if (val != lastVal) {
			int v = int(trans.size());
			trans.emplace_back();
			iota(trans.back().begin(), trans.back().end(), v * 26 + 1);
		}
		lastVal = val;
		const array<int, 26>& t = trans.back();

		for (auto e : adj[cur]) {
			char c = e.first;
			int nxt = e.second;
			int nxtVal = t[c - 'a'];
			if (vis[nxt] == -1 || nxtVal < vis[nxt]) {
				vis[nxt] = nxtVal;
				prv[nxt] = {cur, c};
				q.emplace(nxtVal, nxt);
			}
		}
	}

	vector<int> res;
	string path;
	for (int cur = N-1; cur != 0; cur = prv[cur].first) {
		res.push_back(cur);
		path += prv[cur].second;
	}
	res.push_back(0);

	reverse(path.begin(), path.end());
	reverse(res.begin(), res.end());
	cout << path.size() << '\n';
	for (int i : res) { cout << i+1 << ' '; } cout << '\n';
	cout << path << '\n';

	return 0;
}
