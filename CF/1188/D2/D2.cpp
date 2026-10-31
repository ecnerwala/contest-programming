#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1100;
int N;
pair<pair<int, int>, int> edges[MAXN];
vector<int> adj[MAXN];

int get_leaf(int cur, int prv) {
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		return get_leaf(nxt, cur);
	}
	return cur;
}

pair<int, int> get_leaves(int cur, int prv) {
	vector<int> ans;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		ans.push_back(get_leaf(nxt, cur));
		if (ans.size() == 2) return {ans[0], ans[1]};
	}
	return {cur, cur};
}

int tot[MAXN][MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	cin >> N;

	for (int i = 0; i < N-1; i++) {
		int u, v, val; cin >> u >> v >> val; u--, v--;
		edges[i] = {{u, v}, val};
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	for (int i = 0; i < N; i++) {
		if (adj[i].size() == 2) {
			cout << "NO\n"; exit(0);
		}
	}

	cout << "YES" << '\n';

	for (int i = 0; i < N-1; i++) {
		int u, v; tie(u, v) = edges[i].first;
		int val = edges[i].second;

		auto us = get_leaves(u, v);
		auto vs = get_leaves(v, u);
		tot[us.first][vs.first] += val / 2;
		tot[us.second][vs.second] += val / 2;
		tot[us.first][us.second] -= val / 2;
		tot[vs.first][vs.second] -= val / 2;
	}

	int cnt = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			if (tot[i][j] == 0) continue;
			cnt++;
		}
	}
	cout << cnt << '\n';
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (j == i) continue;
			if (tot[i][j] == 0) continue;
			cout << i+1 << ' ' << j+1 << ' ' << tot[i][j] << '\n';
		}
	}

	return 0;
}
