#include<bits/stdc++.h>
using namespace std;

int main() {
	if (fopen("clocktree.in", "r")) {
		freopen("clocktree.in", "r", stdin);
		freopen("clocktree.out", "w", stdout);
	}

	int N; cin >> N;
	vector<int> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i]; A[i] %= 12;
	}
	vector<vector<int>> adj(N);
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	vector<bool> color(N);
	vector<int> colorSum(2);
	vector<int> colorCnt(2);

	std::function<void(int, int)> self = [&](int cur, int prv) -> void {
		colorSum[color[cur]] += A[cur];
		colorCnt[color[cur]] ++;

		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			color[nxt] = !color[cur];
			self(nxt, cur);
		}
	};
	self(0, -1);

	colorSum[0] %= 12;
	colorSum[1] %= 12;
	if (colorSum[0] == colorSum[1]) {
		cout << N << '\n';
	} else if ((colorSum[0] + 1) % 12 == colorSum[1]) {
		cout << colorCnt[1] << '\n';
	} else if ((colorSum[1] + 1) % 12 == colorSum[0]) {
		cout << colorCnt[0] << '\n';
	} else {
		cout << 0 << '\n';
	}
	return 0;
}

// sum of the evens and sum of the odds must match
// except you can get +1 on the off-ones
