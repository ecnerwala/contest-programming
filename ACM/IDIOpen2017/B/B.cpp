#include <bits/stdc++.h>

#define i main

int i() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int A, F; cin >> A >> F;
	int L, W; cin >> L >> W;
	vector<string> G(L);
	for (int i = 0; i < L; i++) { cin >> G[i]; }
	int st, en;
	vector<array<int, 2>> locs;
	for (int i = 0; i < L; i++) {
		for (int j = 0; j < W; j++) {
			if (G[i][j] == 'S') {
				st = int(locs.size());
				locs.push_back({i,j});
			} else if (G[i][j] == 'G') {
				en = int(locs.size());
				locs.push_back({i,j});
			} else if (G[i][j] == 'W') {
				locs.push_back({i,j});
			}
		}
	}

	int N = int(locs.size());

	auto bfs = [&](auto is_adj) {
		vector<int> dist(N, -1);
		vector<int> q; q.reserve(N);
		dist[st] = 0;
		q.push_back(st);
		for (int z = 0; z < int(q.size()); z++) {
			int cur = q[z];
			for (int nxt = 0; nxt < N; nxt++) {
				if (dist[nxt] != -1) continue;
				int d0 = abs(locs[cur][0] - locs[nxt][0]);
				int d1 = abs(locs[cur][1] - locs[nxt][1]);
				if (is_adj(d0, d1)) {
					dist[nxt] = dist[cur]+1;
					q.push_back(nxt);
				}
			}
		}
		return dist[en];
	};

	int da = bfs([&](int a, int b) -> bool {
		return a * a + b * b <= A * A;
	});
	int df = bfs([&](int a, int b) -> bool {
		return min(a,b) == 0 && max(a, b) <= F;
	});

	if (da == -1 && df == -1) {
		cout << "NO WAY "<< '\n';
	} else {
		if (da == -1) da = N+1;
		if (df == -1) df = N+1;
		if (da == df) {
			cout << "SUCCESS" << '\n';
		} else if (da < df) {
			cout << "GO FOR IT" << '\n';
		} else {
			cout << "NO CHANCE" << '\n';
		}
	}

	return 0;
}
