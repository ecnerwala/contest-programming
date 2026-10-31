#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int H, W; cin >> H >> W;
	vector<vector<bool>> G(H, vector<bool>(W));
	for (int i = 0; i < H; i++) {
		string s; cin >> s;
		for (int j = 0; j < W; j++) {
			assert(s[j] == '#' || s[j] == '.');
			G[i][j] = (s[j] == '#');
		}
	}

	vector<vector<int>> dist(H, vector<int>(W));
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (i == 0 && j == 0) {
				dist[0][0] = G[0][0];
				continue;
			}
			dist[i][j] = int(1e9);
			if (i) {
				dist[i][j] = min(dist[i][j], dist[i-1][j] + (G[i-1][j] != G[i][j]));
			}
			if (j) {
				dist[i][j] = min(dist[i][j], dist[i][j-1] + (G[i][j-1] != G[i][j]));
			}
		}
	}

	int ans = dist[H-1][W-1] + G[H-1][W-1];
	assert(ans % 2 == 0);
	cout << ans / 2 << '\n';

	return 0;
}
