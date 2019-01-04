#include <bits/stdc++.h>
using namespace std;

static const int mx[4] = {-1, 0, 1, 0};
static const int my[4] = {0, -1, 0, 1};

class TileFlippingGame {


	int N, M;
	vector<string> X;

	int num_flips(int x, int y) {
		vector<vector<char>> vis((size_t(N)), vector<char>(size_t(M), false));
		vector<vector<int>> dist((size_t(N)), vector<int>(size_t(M), -1));
		deque<pair<int, int>> q;
		dist[x][y] = 0;
		q.emplace_back(x, y);
		int res = 0;
		while (!q.empty()) {
			auto it = q.front();
			int i = it.first, j = it.second;
			q.pop_front();
			if (vis[i][j]) continue;
			vis[i][j] = true;
			res = max(res, dist[i][j]);
			for (int d = 0; d < 4; d++) {
				int nx = i + mx[d], ny = j + my[d];
				if (0 <= nx && nx < N && 0 <= ny && ny < M && X[nx][ny] != 'e') {
					int nd = dist[i][j];
					if (X[nx][ny] != X[i][j]) {
						nd++;
						if (dist[nx][ny] == -1 || dist[nx][ny] > nd) {
							dist[nx][ny] = nd;
							q.emplace_back(nx, ny);
						}
					} else {
						if (dist[nx][ny] == -1 || dist[nx][ny] > nd) {
							dist[nx][ny] = nd;
							q.emplace_front(nx, ny);
						}
					}
				}
			}
			if (dist[i][j] != -1) continue;
		}
		return res;
	}

public:

	int HowManyMoves(int n, int m, vector <string> x) {
		N = n;
		M = m;
		X = x;
		int rb = 0;
		int rw = 0;
		vector<vector<char>> vis((size_t(N)), vector<char>(size_t(M)));
		for (int si = 0; si < N; si++) {
			for (int sj = 0; sj < M; sj++) {
				if (vis[si][sj]) continue;
				if (X[si][sj] == 'e') continue;
				queue<pair<int, int>> q;
				vis[si][sj] = true;
				int val = num_flips(si, sj);
				int vb = val + ((X[si][sj] != 'b') ^ (val % 2));
				int vw = val + ((X[si][sj] != 'w') ^ (val % 2));
				q.emplace(si, sj);
				while (!q.empty()) {
					int i = q.front().first, j = q.front().second;
					q.pop();
					for (int d = 0; d < 4; d++) {
						int ni = i + mx[d], nj = j + my[d];
						if (0 <= ni && ni < N && 0 <= nj && nj < M && X[ni][nj] != 'e') {
							if (vis[ni][nj]) continue;
							vis[ni][nj] = true;
							q.emplace(ni, nj);
							val = num_flips(ni, nj);
							vb = min(vb, val + ((X[ni][nj] != 'b') ^ (val % 2)));
							vw = min(vw, val + ((X[ni][nj] != 'w') ^ (val % 2)));
						}
					}
				}
				rb += vb;
				rw += vw;
			}
		}
		cerr << rb << ' ' << rw << '\n';
		return min(rb, rw);
	}
};

// vim:ft=cpp
