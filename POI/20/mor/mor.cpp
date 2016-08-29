#include<bits/stdc++.h>
using namespace std;

const int MAXN = 5010;
const int MAXM = 5010;

int N;
int M;
int K;
vector<int> adj[MAXN];
short dist[MAXN][MAXN][2];

int main() {
	scanf("%d %d %d", &N, &M, &K);
	for(int i = 0; i < M; i++) {
		int a, b;
		scanf("%d %d", &a, &b);
		a--, b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	memset(dist, -1, sizeof(dist));

	for(int s = 0; s < N; s++) {
		queue<pair<int, bool> > q;
		q.push(make_pair(s, false));
		dist[s][s][0] = 0;
		while(!q.empty()) {
			int c = q.front().first;
			bool l = q.front().second;
			q.pop();
			short int d = dist[s][c][l] + 1;
			assert(d % 2 != l);
			l = !l;
			for(vector<int>::iterator it = adj[c].begin(); it != adj[c].end(); it++) {
				int n = *it;
				if(dist[s][n][l] != -1) continue;
				dist[s][n][l] = d;
				q.push(make_pair(n, l));
			}
		}
	}

	for(int i = 0; i < K; i++) {
		int s, t, d;
		scanf("%d %d %d", &s, &t, &d);
		s--, t--;
		bool good = dist[s][t][d % 2] != -1 && d >= dist[s][t][d % 2];
		if(s == t && d % 2 == 0 && adj[s].empty()) good = false;
		printf(good ? "TAK\n" : "NIE\n");
	}
	return 0;
}
