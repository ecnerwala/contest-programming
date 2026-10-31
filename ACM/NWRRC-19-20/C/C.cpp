#include<bits/stdc++.h>
using namespace std;

const int MAXW = 200, MAXH = 200;
int W, H;
bool G[MAXW][MAXH];

// dir = false means diag next
// dir = true means horiz next
void dfs(int i, int j, int px, int py, bool dir) {
	if (!G[i][j]) return;
	G[i][j] = false;
	int nx = i ^ (i-1) ^ px;
	int ny = j ^ (j-1) ^ py;

	auto visit = [&](int x, int y, bool d) {
		cout << x << ' ' << y << '\n';
		dfs(x+1, y+1, x, y, d);
		dfs(x, y+1, x, y, d);
		dfs(x+1, y, x, y, d);
		dfs(x, y, x, y, d);
	};

	if (dir) {
		// horiz next
		visit(nx, py, false);
		visit(px, ny, true);
		visit(nx, ny, false);
		visit(px, py, true);
	} else {
		// diag next
		visit(nx, ny, true);
		visit(px, ny, false);
		visit(nx, py, true);
		visit(px, py, false);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> W >> H;
	int cnt = 0;
	for (int j = 1; j <= H; j++) {
		string S; cin >> S;
		for (int i = 1; i <= W; i++) {
			cnt += (G[i][j] = (S[i-1] == 'X'));
		}
	}

	cout << (cnt * 4 - 1) << '\n';
	for (int i = 1; i <= W; i++) {
		for (int j = 1; j <= H; j++) {
			if (G[i][j]) {
				dfs(i, j, i-1, j-1, true);
				exit(0);
			}
		}
	}
	assert(false);

	return 0;
}
