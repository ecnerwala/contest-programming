#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int H, W; cin >> H >> W;
	std::vector<std::string> G(H);
	for (int i = 0; i < H; i++) {
		cin >> G[i];
	}

	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (G[i][j] == '.') {
				int cnt = 0;
				if (i > 0 && G[i-1][j] == '#') cnt++;
				if (i < H-1 && G[i+1][j] == '#') cnt++;
				if (j > 0 && G[i][j-1] == '#') cnt++;
				if (j < W-1 && G[i][j+1] == '#') cnt++;
				if (cnt >= 2) {
					cout << i+1 << ' ' << j+1 << '\n';
					exit(0);
				}
			}
		}
	}
	assert(false);

	return 0;
}
