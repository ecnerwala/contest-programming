#include <bits/stdc++.h>

#define java_is_my_fav_language main

const int XMID = 7;
const int XMAX = 15;

const int V = XMAX * XMAX * 4;

// left is +1
const int dx[4] = {1,0,-1,0};
const int dy[4] = {0,1,0,-1};

int dist[V][V];

int java_is_my_fav_language() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int B, S, R, L, N; cin >> B >> S >> R >> L >> N;

	const int INF = 1e9;
	for (int i = 0; i < V; i++) {
		for (int j = 0; j < V; j++) {
			dist[i][j] = (i == j ? 0 : INF);
		}
	}
	for (int x = 0; x < XMAX; x++) {
		for (int y = 0; y < XMAX; y++) {
			for (int d = 0; d < 4; d++) {
				int i = (x*XMAX+y)*4+d;
				int nx = x + dx[d];
				int ny = y + dy[d];
				if (0 <= nx && nx < XMAX && 0 <= ny && ny < XMAX) {
					int j = (nx*XMAX+ny)*4;
					dist[i][j+d%4] = B+S;
					dist[i][j+(d+1)%4] = B+L;
					dist[i][j+(d+3)%4] = B+R;
				}
			}
		}
	}
	for (int k = 0; k < V; k++) {
		for (int i = 0; i < V; i++) {
			for (int j = 0; j < V; j++) {
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}

	int cur = (XMID * XMAX + XMID) * 4;
	array<int, 4> dp{0,0,0,0};
	for (int t = 0; t < N; t++) {
		int x, y; cin >> x >> y;
		x += XMID, y += XMID;
		int nxt = (x * XMAX + y) * 4;
		array<int, 4> ndp{INF,INF,INF,INF};
		for (int d0 = 0; d0 < 4; d0++) {
			for (int d1 = 0; d1 < 4; d1++) {
				ndp[d1] = min(ndp[d1], dp[d0] + dist[cur+d0][nxt+d1]);
			}
		}
		cur = nxt, dp = ndp;
	}
	cout << *min_element(dp.begin(), dp.end()) << '\n';

	return 0;
}
