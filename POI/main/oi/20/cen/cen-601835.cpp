#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 1e5 + 20;
const int MAXM = 1e5 + 20;

int N;
int M;
int K;

vector<int> adj[MAXN];
int dist[MAXN];
int A, B;

int main() {
	scanf("%d %d %d %d %d", &N, &M, &K, &A, &B);
	for(int i = 0; i < M; i++) {
		int u, v; scanf("%d %d", &u, &v);
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	memset(dist, -1, sizeof(dist));
	dist[K] = 0;

	queue<int> q;
	q.push(K);

	for(int z = 0; z < N; z++) {
		//assert(!q.empty());
		int a = q.front();
		q.pop();
		for(vector<int>::iterator it = adj[a].begin(); it != adj[a].end(); it++) {
			int b = *it;
			if(dist[b] != -1) continue;
			dist[b] = dist[a] + 1;
			q.push(b);
		}
	}

	B = min(B, 2 * A);
	for(int i = 1; i <= N; i++) {
		//cerr << dist[i] << '\n';
		printf("%d\n", dist[i] / 2 * B + dist[i] % 2 * A);
	}
	return 0;
}
