#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1100;
int R, C;
bool vis[MAXN][MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> R >> C;
	vector<pair<int, int>> q;
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			char c; cin >> c;
			if (c == '#') {
				vis[i][j] = true;
				q.emplace_back(i, j);
			} else {
				vis[i][j] = false;
			}
		}
	}

	int ans = 0;
	assert(!q.empty());
	while (true) {
		assert(!q.empty());
		vector<pair<int, int>> nq;
		for (auto it : q) {
			const int mx[4] = {-1, 0, 1, 0};
			const int my[4] = {0, -1, 0, 1};
			for (int d = 0; d < 4; d++) {
				int i = it.first + mx[d];
				int j = it.second + my[d];
				if (0 <= i && i < R && 0 <= j && j < C && !vis[i][j]) {
					vis[i][j] = true;
					nq.emplace_back(i, j);
				}
			}
		}
		if (nq.empty()) {
			break;
		}
		q = nq;
		ans++;
	}
	cout << ans << '\n';

	return 0;
}
