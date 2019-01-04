#include<bits/stdc++.h>
using namespace std;

const int N = 6;
int deg[7];
int adj[7][7];

vector<int> edges[7];

const int MAXM = 200;
int M;
int E[MAXM][2];
bool used[MAXM];

void dfs(int cur) {
	while (!edges[cur].empty()) {
		int e = edges[cur].back();
		edges[cur].pop_back();
		if (used[e]) continue;
		used[e] = true;

		int z = (cur == E[e][1]);
		assert(cur == E[e][z]);
		int nxt = E[e][!z];
		dfs(nxt);

		printf("%d %c\n", e+1, z ? '+' : '-');
	}
}

int main() {
	scanf("%d", &M);
	for (int i = 0; i < M; i++) {
		scanf("%d %d", &E[i][0], &E[i][1]);
	}

	for (int i = 0; i < M; i++) {
		deg[E[i][0]] ++;
		deg[E[i][1]] ++;
		adj[E[i][0]][E[i][1]] = true;
		adj[E[i][1]][E[i][0]] = true;
		edges[E[i][0]].push_back(i);
		edges[E[i][1]].push_back(i);
	}

	for (int i = 0; i <= N; i++) adj[i][i] = true;
	for (int k = 0; k <= N; k++) {
		for (int i = 0; i <= N; i++) {
			for (int j = 0; j <= N; j++) {
				if (adj[i][k] && adj[k][j]) adj[i][j] = true;
			}
		}
	}

	for (int i = 0; i <= N; i++) {
		for (int j = 0; j <= N; j++) {
			if (deg[i] && deg[j] && !adj[i][j]) {
				printf("No solution\n");
				return 0;
			}
		}
	}

	int n_odd = 0;
	for (int i = 0; i <= N; i++) n_odd += deg[i] % 2;
	if (n_odd > 2) {
		printf("No solution\n");
		return 0;
	}

	int st;
	if (n_odd) {
		for (int i = 0; i <= N; i++) {
			if (deg[i] % 2) {
				st = i;
				break;
			}
		}
	} else {
		for (int i = 0; i <= N; i++) {
			if (deg[i]) {
				st = i;
				break;
			}
		}
	}

	dfs(st);

	return 0;
}
