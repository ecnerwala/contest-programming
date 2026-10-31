#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
vector<int> adj[MAXN];

int dfs(int cur, int prv) {
	int chVal = 0;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		chVal += dfs(nxt, cur);
	}
	if (chVal == 0) {
		return 1;
	} else {
		return chVal - 1;
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N-1; i++) {
		int a, b; cin >> a >> b;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	cout << (dfs(1, 0) ? "Alice" : "Bob") << '\n';

	return 0;
}
