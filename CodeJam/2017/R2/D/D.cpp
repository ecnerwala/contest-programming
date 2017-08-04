#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

const int mx[] = { -1, 0, 1, 0 };
const int my[] = { 0, -1, 0, 1 };

const int MAXR = 200;
const int MAXC = 200;
int R, C;
char G[MAXR][MAXC];
int dist[MAXR][MAXC];

int M;

const int MAXS = 200;
int S;
vector<pii> SL;
const int MAXT = 200;
int T;
vector<pii> TL;

vector<pii> res;

void go() {
	S = 0, SL.clear();
	T = 0, TL.clear();
	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			if (G[i][j] == 'S') {
				SL.emplace_back(i,j);
				S++;
				G[i][j] = '.';
			} else if (G[i][j] == 'T') {
				TL.emplace_back(i,j);
				T++;
				G[i][j] = '.';
			}
		}
	}
	for (int i = 0; i < S; i++) {
		int x = SL[i].first, y = SL[i].second;
		memset(dist, -1, sizeof(dist));
		dist[x][y] = 0;
		queue<pii> bfs;
		bfs.emplace(x, y);
		while (!bfs.empty()) {
			int cx = bfs.front().first, cy = bfs.front().second;
			bfs.pop();
			if (G[cx][cy] != '.') continue;
			for (int d = 0; d < 4; d++) {
				int nx = cx + mx[d], ny = cy + my[d];
				if (dist[nx][ny] == -1) {
					dist[nx][ny] = dist[cx][cy] + 1;
					bfs.emplace(nx, ny);
				}
			}
		}

		for (int j = 0; j < T; j++) {
			if (tm & (1 << j)) continue;
			int tx = TL[j].first, ty = TL[j].second;
			bool good = false;
			for (int d = 0; !good && d < 4; d++) {
				for (int nx = tx, ny = ty; G2[nx][ny] != '#'; nx += mx[d], ny += my[d]) {
					if (dist[nx][ny] <= M) {
						good = true;
						break;
					}
				}
			}
			if (good) {
				adj[i].push_back(j);
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	int __T; cin >> __T;

	for(int case_num = 1; case_num <= __T; case_num ++) {
		cin >> C >> R;
		cin >> M;
		for (int i = 1; i <= R; i++) {
			cin >> (G[i] + 1);
			G[i][0] = '#';
			G[i][C + 1] = '#';
		}
		for (int i = 0; i <= C + 1; i++) {
			G[0][i] = G[R + 1][i] = '#';
		}

		res.clear();
		go();
		cout << "Case #" << case_num << ": " << res.size() << '\n';
		for (auto& it : res) {
			cout << it.first + 1 << ' ' << it.second + 1 << '\n';
		}
	}

	return 0;
}
