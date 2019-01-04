#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

const int MAXN = 120;
int N;
int K;
int M;

int adj[MAXN][MAXN];

int dist[MAXN];
bool vis[MAXN];
int shortest_path(int st, int en) {
	for (int i = 0; i < N; i++) dist[i] = INF, vis[i] = false;
	dist[st] = 0;
	for (int t = 0; t < N; t++) {
		int cur = 0;
		while (vis[cur]) cur++;
		for (int i = cur + 1; i < N; i++) {
			if (vis[i]) continue;
			if (dist[i] < dist[cur]) cur = i;
		}
		assert(dist[cur] < INF);
		if (cur == en) break;
		vis[cur] = true;
		for (int nxt = 0; nxt < N; nxt++) {
			if (vis[nxt]) continue;
			dist[nxt] = min(dist[nxt], dist[cur] + adj[cur][nxt]);
		}
	}
	return dist[en];
}

int main() {
	scanf("%d %d %d", &N, &K, &M);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			adj[i][j] = INF;
		}
	}
	for (int i = 0; i < K; i++) {
		int a, b, w; scanf("%d %d %d", &a, &b, &w);
		a--, b--;
		adj[a][b] = min(adj[a][b], w);
		adj[b][a] = min(adj[b][a], w);
	}
	int d = shortest_path(0, N-1);
	for (int i = 0; i < M; i++) {
		int a, b, w; scanf("%d %d %d", &a, &b, &w);
		a--, b--;
		assert(adj[a][b] == adj[b][a]);
		int p = adj[a][b];
		adj[a][b] = adj[b][a] = w;
		int nd = shortest_path(0, N-1);
		if (nd < d) {
			printf("1\n");
			d = nd;
		} else {
			printf("0\n");
			adj[a][b] = adj[b][a] = p;
		}
	}
	return 0;
}
