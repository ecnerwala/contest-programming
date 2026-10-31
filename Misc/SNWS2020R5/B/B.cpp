#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int K; cin >> K;

	int N = 2;
	int s = 1, t = 2;
	vector<pair<int, int>> edges;
	while (K > 1) {
		if (K & 1) {
			edges.emplace_back(s, t);
		}
		int u = ++N;
		int v = ++N;
		int nt = ++N;
		edges.emplace_back(nt, u);
		edges.emplace_back(nt, v);
		edges.emplace_back(u, t);
		edges.emplace_back(v, t);
		t = nt;

		K /= 2;
	}

	assert(K == 1);
	edges.emplace_back(s, t);
	cout << N << ' ' << edges.size() << '\n';
	for (auto it : edges) {
		cout << it.first << ' ' << it.second << '\n';
	}

	return 0;
}
