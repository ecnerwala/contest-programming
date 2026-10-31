#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int W;
	int D;
	int N;

	cin >> W >> D >> N;

	const int INF = int(1e9) + 20;
	vector<vector<int>> height(W, vector<int>(D, -INF));
	for (int z = 0; z < N; z++) {
		int a, b, c; cin >> a >> b >> c;
		a--, b--;
		height[a][b] = c;
	}
	auto oldHeight = height;

	for (int i = 0; i < W; i++) {
		for (int j = 1; j < D; j++) {
			height[i][j] = max(height[i][j], height[i][j-1] - 1);
		}
		for (int j = D-2; j >= 0; j--) {
			height[i][j] = max(height[i][j], height[i][j+1] - 1);
		}
	}
	for (int j = 0; j < D; j++) {
		for (int i = 1; i < W; i++) {
			height[i][j] = max(height[i][j], height[i-1][j] - 1);
		}
		for (int i = W-2; i >= 0; i--) {
			height[i][j] = max(height[i][j], height[i+1][j] - 1);
		}
	}

	for (int i = 0; i < W; i++) {
		for (int j = 0; j < D; j++) {
			if (oldHeight[i][j] == -INF) continue;
			if (height[i][j] != oldHeight[i][j]) {
				cout << "No" << '\n';
				exit(0);
			}
		}
	}

	long long ans = 0;
	for (int i = 0; i < W; i++) {
		for (int j = 0; j < D; j++) {
			ans += height[i][j];
		}
	}

	cout << ans << '\n';
	return 0;
}
