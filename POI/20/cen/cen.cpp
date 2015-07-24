#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
const int MAXM = 2e5;

int N;
int M;
int K;
int A, B;

vector<int> adj[MAXN];

int dist[MAXN];
int dist2[MAXN];

vector<int> rem[MAXN]; // remaining adjacency

void bfs() {
	memset(dist, -1, sizeof(dist));
	queue<int> q;
	dist[K] = 0;
	q.push(K);
	while(!q.empty()) {
		int v = q.front(); q.pop();
		for(int i = 0; i < int(adj[v].size()); i++) {
			int x = adj[v][i];
			if(dist[x] != -1) continue;
			dist[x] = dist[v] + 1;
			q.push(x);
		}
	}
}

int edge[MAXN];

void bfs2() {
	memset(dist2, -1, sizeof(dist2));
	for(int i = 0; i < N; i++) {
		rem[i] = adj[i];
	}

	queue<int> q;

	dist2[K] = 0;
	q.push(K);

	memset(edge, -1, sizeof(edge));

	while(!q.empty()) {
		int v = q.front(); q.pop();

		for(int i = 0; i < int(adj[v].size()); i++) {
			edge[adj[v][i]] = v;
		}

		for(int i = 0; i < int(adj[v].size()); i++) {
			int x = adj[v][i];
			int s = 0;
			for(int j = 0; j < int(rem[x].size()); j++) {
				int y = rem[x][j];
				if(dist2[y] == -1 && edge[y] != v) {
					dist2[y] = dist2[v] + 1;
					q.push(y);
				} else {
					rem[x][s++] = y;
				}
			}
			rem[x].resize(s);
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N >> M >> K >> A >> B;
	K--;

	for(int i = 0; i < M; i++) {
		int x, y;
		cin >> x >> y;
		x--, y--;
		adj[x].push_back(y);
		adj[y].push_back(x);
	}

	bfs();
	bfs2();

	B = min(B, 2 * A);
	for(int i = 0; i < N; i++) {
		assert(dist[i] >= 0);
		dist[i] = (dist[i] / 2) * B + (dist[i] % 2) * A;
		if(dist2[i] == -1) continue;
		dist2[i] = dist2[i] * B;
		dist[i] = min(dist[i], dist2[i]);
	}

	for(int i = 0; i < N; i++) {
		cout << dist[i] << '\n';
	}

	return 0;
}
