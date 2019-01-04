#include<bits/stdc++.h>
using namespace std;

const int MAXN = 200;
int N;
int G[MAXN][MAXN];

vector<int> adj[MAXN];
int prv[MAXN];
bool vis[MAXN];

bool dfs(int cur) {
	if (cur == -1) return true;
	if (vis[cur]) return false;
	vis[cur] = true;
	for (int nxt : adj[cur]) {
		int pn = prv[nxt];
		prv[nxt] = cur;
		if (dfs(pn)) {
			return true;
		}
		prv[nxt] = pn;
	}
	return false;
}

int go() {
	int res = 0;
	for (int c = -N; c <= N; c++) {
		memset(vis, 0, sizeof(vis));
		memset(prv, -1, sizeof(prv));

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (G[i][j] == c) {
					adj[i].push_back(j);
				}
			}
			if (dfs(i)) {
				res ++;
				memset(vis, 0, sizeof(vis));
			}
		}
		for (int i = 0; i < N; i++) {
			adj[i].clear();
		}
	}
	return N * N - res;
}

int main() {
	int T; scanf("%d", &T);

	for (int case_num = 1; case_num <= T; case_num ++) {
		scanf("%d", &N);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				scanf("%d", &G[i][j]);
			}
		}

		int res = go();

		printf("Case #%d: %d\n", case_num, res);
	}

	return 0;
}
