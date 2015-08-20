#include<bits/stdc++.h>
using namespace std;

const int mx[4] = {-1, 0, 1, 0};
const int my[4] = {0, -1, 0, 1};

const int MAXN = 2e3;
const int MAXM = 2e3;
int N, M;

char grid[MAXN][MAXM];

int dist[3][MAXN][MAXM];

bool valid(int a, int b) {
	return 0 <= a && a < N && 0 <= b && b < M;
}

void bfs(int t) {
	deque<pair<int, int>> q;

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			if(dist[t][i][j] == 0) q.emplace_back(i, j);
			else assert(dist[t][i][j] == -1);
		}
	}

	while(!q.empty()) {
		int a = q.front().first, b = q.front().second;
		q.pop_front();
		for(int d = 0; d < 4; d++) {
			int na = a + mx[d], nb = b + my[d];
			if(!valid(na, nb)) continue;
			int nd = dist[t][a][b] + (grid[a][b] == '#');
			if(dist[t][na][nb] == -1 || nd < dist[t][na][nb]) {
				dist[t][na][nb] = nd;
				if(grid[a][b] == '#') q.emplace_back(na, nb);
				else q.emplace_front(na, nb);
			}
		}
	}
}

int main() {
	memset(dist, -1, sizeof(dist));
	cin >> N >> M;
	for(int i = 0; i < N; i++) cin >> grid[i];
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			if(grid[i][j] == 'S') dist[1][i][j] = 0;
			if(grid[i][j] == 'P') dist[2][i][j] = 0;
		}
	}
	for(int i = 0; i < N; i++) dist[0][i][0] = dist[0][i][M - 1] = 0;
	for(int j = 0; j < M; j++) dist[0][0][j] = dist[0][N - 1][j] = 0;

	bfs(0), bfs(1), bfs(2);

	int res = N * M * 2;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			res = min(res, dist[0][i][j] + dist[1][i][j] + dist[2][i][j] + (grid[i][j] == '#'));
		}
	}
	cout << res << '\n';
	return 0;
}
