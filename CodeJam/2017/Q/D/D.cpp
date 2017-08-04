#include<bits/stdc++.h>
using namespace std;

const int MAXL = 5e2;
const int MAXR = 5e2;
int L, R;
vector<int> adj[MAXL];
int prv[MAXR];

void reset() {
	for (int i = 0; i < L; i++) adj[i].clear();
	for (int i = 0; i < R; i++) prv[i] = -1;
}

bool vis[MAXL];
bool dfs(int i) {
	if (vis[i]) {
		return false;
	}
	vis[i] = true;
	for (int j : adj[i]) {
		if (prv[j] == -1 || dfs(prv[j])) {
			prv[j] = i;
			return true;
		}
	}
	return false;
}

void do_max_flow() {
	for (int i = 0; i < L; i++) {
		memset(vis, 0, sizeof(vis));
		dfs(i);
	}
}

const int MAXN = 2e2;
const int DIAG = 0;
const int RECT = 1;
int N;
bool G[MAXN][MAXN][2];

void checkValid() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (G[i][j][RECT]) {
				for (int k = 0; k < N; k++) {
					assert(k == j || !G[i][k][RECT]);
					assert(i == k || !G[k][j][RECT]);
				}
			}
			if (G[i][j][DIAG]) {
				for (int dx = -1; dx <= 1; dx += 2) {
					for (int dy = -1; dy <= 1; dy += 2) {
						for (int x = i + dx, y = j + dy; 0 <= x && x < N && 0 <= y && y < N; x+=dx, y+=dy) {
							assert(!G[x][y][DIAG]);
						}
					}
				}
			}
		}
	}
}

void go() {
	L = N, R = N;
	reset();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			adj[i].push_back(j);
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (G[i][j][RECT]) {
				prv[j] = i;
				adj[i] = {j};
			}
		}
	}
	do_max_flow();
	for (int j = 0; j < N; j++) {
		int i = prv[j];
		if (i == -1) continue;
		G[i][j][RECT] = true;
	}
	L = 2 * N - 1, R = 2 * N - 1;
	reset();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			adj[i+j].push_back(i-j + (N-1));
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (G[i][j][DIAG]) {
				prv[i-j + (N-1)] = i+j;
				adj[i+j] = {i-j + (N-1)};
			}
		}
	}
	do_max_flow();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (prv[i-j + (N-1)] == i+j) {
				G[i][j][DIAG] = true;
			}
		}
	}
}

bool G0[MAXN][MAXN][2];
int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		int M;
		cin >> N >> M;
		memset(G, 0, sizeof(G));
		for (int m = 0; m < M; m++) {
			char c; int i, j;
			cin >> c >> i >> j;
			i--, j--;
			if (c == 'o' || c == 'x') G[i][j][RECT] = true;
			if (c == 'o' || c == '+') G[i][j][DIAG] = true;
		}
		memcpy(G0, G, sizeof(G));

		go();
		checkValid();

		cout << "Case #" << case_num << ": ";

		int res = 0;
		int cnt = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				res += G[i][j][RECT];
				res += G[i][j][DIAG];
				assert(G[i][j][RECT] >= G0[i][j][RECT]);
				assert(G[i][j][DIAG] >= G0[i][j][DIAG]);
				if (G[i][j][RECT] != G0[i][j][RECT] || G[i][j][DIAG] != G0[i][j][DIAG]) {
					cnt++;
				}
			}
		}
		cout << res << ' ' << cnt << '\n';
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				assert(G[i][j][RECT] >= G0[i][j][RECT]);
				assert(G[i][j][DIAG] >= G0[i][j][DIAG]);
				if (G[i][j][RECT] != G0[i][j][RECT] || G[i][j][DIAG] != G0[i][j][DIAG]) {
					char c = (G[i][j][RECT] && G[i][j][DIAG]) ? 'o' : (G[i][j][RECT] ? 'x' : '+');
					cout << c << ' ' << i+1 << ' ' << j+1 << '\n';
				}
			}
		}
	}

	return 0;
}
