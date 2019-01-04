#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

const int mx[4] = {-1, 0, 1, 0}, my[4] = {0, -1, 0, 1};
const int MAXN = 500;
const int MAXM = 500;
int N, M;

ll Q, P;
char G[MAXN][MAXM];
ll V[MAXN][MAXM];

bool vis[MAXN][MAXM];
void bfs(int si, int sj, ll v) {
	memset(vis, 0, sizeof(vis));
	vector<pair<int, int>> q;
	q.emplace_back(si,sj);
	while (!q.empty()) {
		vector<pair<int, int>> nq;
		for (auto it : q) {
			int pi = it.first, pj = it.second;
			V[pi][pj] += v;
			for (int d = 0; d < 4; d ++) {
				int i = pi + mx[d], j = pj + my[d];
				if (!(0 <= i && i < N && 0 <= j && j < M)) continue;
				if (G[i][j] == '*') continue;
				if (vis[i][j]) continue;
				vis[i][j] = true;
				nq.emplace_back(i, j);
			}
		}
		swap(q, nq);
		v /= 2;
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> Q >> P;
	for (int i = 0; i < N; i++) {
		cin >> G[i];
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if ('A' <= G[i][j] && G[i][j] <= 'Z') {
				bfs(i, j, Q * (G[i][j] - 'A' + 1));
			}
		}
	}
	int res = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			res += (V[i][j] > P);
		}
	}
	cout << res << '\n';

	return 0;
}
