#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;
int N;
vector<int> adj[MAXN];
pair<int, int> furthest(int cur, int prv = 0) {
	pair<int, int> ans(0, cur);
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		auto res = furthest(nxt, cur);
		res.first++;
		ans = max(ans, res);
	}
	return ans;
}

int diameter() {
	return furthest(furthest(1).second).first;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	cin >> N;
	for (int i = 0; i < N-1; i++) {
		int a, b; cin >> a >> b;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	cout << ((diameter() + 1) % 3 == 2 ? "Second" : "First") << '\n';

	return 0;
}
