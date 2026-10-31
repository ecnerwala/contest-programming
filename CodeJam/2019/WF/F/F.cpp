#include<bits/stdc++.h>
using namespace std;

const bool TEST = false;

const int INF = 1e8;

const int mx[4] = {-1, 0, 1, 0};
const int my[4] = {0, -1, 0, 1};

const int MAXR = 200;
const int MAXC = 200;
int R, C;
char G[MAXR][MAXC];

template <typename F> inline void bfs(int a, int b, int dist[MAXR][MAXC], const F& f) {
	for (int i = 0; i <= R+1; i++) {
		for (int j = 0; j <= C+1; j++) {
			dist[i][j] = INF;
		}
	}

	queue<pair<int, int>> q;
	dist[a][b] = 0;
	q.emplace(a,b);
	while (!q.empty()) {
		auto cur = q.front(); q.pop();
		int x = cur.first, y = cur.second;
		for (int d = 0; d < 4; d++) {
			int nx = x + mx[d], ny = y + my[d];
			if (1 <= nx && nx <= R && 1 <= ny && ny <= C && f(nx, ny) && dist[nx][ny] == INF) {
				dist[nx][ny] = dist[x][y] + 1;
				q.emplace(nx, ny);
			}
		}
	}
}

bool isOpen(int x, int y) {
	return 1 <= x && x <= R && 1 <= y && y <= C && G[x][y] != '#';
}

int sDist[MAXR][MAXC];

char openK[2][MAXR][MAXC];
int dtgt[MAXR][MAXC];
int dloop[MAXR][MAXC];

int go() {
	pair<int, int> src, tgt;
	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			if (G[i][j] == 'N') {
				tgt = pair<int, int>(i,j);
				G[i][j] = '.';
			} else if (G[i][j] == 'M'){
				src = pair<int, int>(i,j);
				G[i][j] = '.';
			}
		}
	}
	assert(src.first != 0 && tgt.first != 0);

	bfs(src.first, src.second, sDist, [](int x, int y) { return G[x][y] != '#'; });

	int ans = sDist[tgt.first][tgt.second];

	int cnt = 0;
	for (int dr = -R; dr <= R; dr++) {
		for (int dc = -C; dc <= C; dc++) {
			if (dr == 0 && dc == 0) continue;
			if (!isOpen(tgt.first + dr, tgt.second + dc)) continue;

			for (int i = 0; i <= R+1; i++) {
				for (int j = 0; j <= C+1; j++) {
					openK[0][i][j] = isOpen(i, j);
				}
			}

			for (int k = 1; isOpen(tgt.first + dr * k, tgt.second + dc * k); k ++) {
				cnt++;
				for (int i = 1; i <= R; i++) {
					for (int j = 1; j <= C; j++) {
						openK[k&1][i][j] = openK[!(k&1)][i][j] && isOpen(i + dr * k, j + dc * k);
					}
				}
				assert(openK[k&1][tgt.first][tgt.second]);
				bfs(tgt.first, tgt.second, dtgt, [&](int x, int y) { return openK[k&1][x][y]; });
				bfs(tgt.first + dr, tgt.second + dc, dloop, [&](int x, int y) { return openK[!(k&1)][x][y]; });
				for (int i = 1; i <= R; i++) {
					for (int j = 1; j <= C; j++) {
						if (!openK[k&1][i][j]) continue;
						ans = min(ans, dtgt[i][j] + dloop[i][j] + sDist[i + dr * k][j + dc * k] + 1);
					}
				}
			}
		}
	}
	//cerr << R << ' ' << C << ' ' << cnt << '\n';

	return ans;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << ": " << '\n';
		if (TEST) {
			R = (case_num <= 10 ? 100 : 50);
			C = (case_num <= 10 ? 100 : 50);
			for (int j = 0; j <= C+1; j++) G[0][j] = '#';
			for (int i = 1; i <= R; i++) {
				G[i][0] = '#';
				for (int j = 1; j <= C; j++) {
					if (i == 1 && j == 1) {
						G[i][j] = 'N';
					} else if (i == R && j == C) {
						G[i][j] = 'M';
					} else {
						G[i][j] = '.';
					}
				}
				G[i][C+1] = '#';
			}
			for (int j = 0; j <= C+1; j++) G[R+1][j] = '#';
		} else {
			cin >> R >> C;
			for (int j = 0; j <= C+1; j++) G[0][j] = '#';
			for (int i = 1; i <= R; i++) {
				G[i][0] = '#';
				cin >> (G[i] + 1);
				G[i][C+1] = '#';
			}
			for (int j = 0; j <= C+1; j++) G[R+1][j] = '#';
		}

		int ans = go();

		cerr << ans << '\n';

		cout << "Case #" << case_num << ": ";
		if (ans == INF) {
			cout << "IMPOSSIBLE" << '\n';
		} else {
			cout << ans << '\n';
		}
	}

	return 0;
}
