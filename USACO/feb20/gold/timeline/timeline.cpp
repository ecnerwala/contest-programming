#include<bits/stdc++.h>
using namespace std;

int main() {
	if (fopen("timeline.in", "r")) {
		freopen("timeline.in", "r", stdin);
		freopen("timeline.out", "w", stdout);
	}
	int N, M, C;
	cin >> N >> M >> C;
	vector<int> S(N);
	for (int i = 0; i < N; i++) cin >> S[i];
	vector<vector<pair<int, int>>> outEdges(N);
	vector<int> inDeg(N);
	for (int c = 0; c < C; c++) {
		int a, b, x; cin >> a >> b >> x; a--, b--;
		outEdges[a].emplace_back(b, x);
		inDeg[b]++;
	}
	vector<int> q; q.reserve(N);
	for (int i = 0; i < N; i++) if (!inDeg[i]) q.push_back(i);
	for (int z = 0; z < N; z++) {
		int cur = q[z];
		for (auto it : outEdges[cur]) {
			int nxt = it.first;
			S[nxt] = max(S[nxt], S[cur] + it.second);
			if (!(--inDeg[nxt])) {
				q.push_back(nxt);
			}
		}
	}

	for (int i = 0; i < N; i++) cout << S[i] << '\n';

	return 0;
}
