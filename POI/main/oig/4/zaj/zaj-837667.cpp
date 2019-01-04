#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

int mx[8] = {1, 1, -1, -1, 2, 2, -2 ,-2};
int my[8] = {2, -2, 2, -2, 1, -1, 1, -1};

pii add(pii a, int i) {
	return pii(a.first + mx[i], a.second + my[i]);
}

const int MAXN = 3e3;
const int INF = 1e9;
int dist[MAXN][MAXN];
int N, M;

char grid[MAXN][MAXN];

bool is_good(pii a) {
	int x = a.first;
	int y = a.second;
	return 0 <= x && x < N && 0 <= y && y < M && grid[x][y] != 'x' && dist[x][y] == INF;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for (int i = 0; i < N; i++) cin >> grid[i];

	pii start, end;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			dist[i][j] = INF;
			if (grid[i][j] == 'z') {
				start = pii(i,j);
			} else if (grid[i][j] == 'n') {
				end = pii(i,j);
			}
		}
	}

	queue<pii> q;
	q.push(start);
	dist[start.A][start.B] = 0;
	while (!q.empty()) {
		pii cur = q.front(); q.pop();
		for (int i = 0; i < 8; i++) {
			pii nxt = add(cur, i);
			if (is_good(nxt)) {
				dist[nxt.A][nxt.B] = dist[cur.A][cur.B] + 1;
				q.push(nxt);
			}
		}
	}

	int res = dist[end.A][end.B];
	if (res == INF) {
		cout << "NIE\n";
	} else {
		cout << res << '\n';
	}

	return 0;
}
