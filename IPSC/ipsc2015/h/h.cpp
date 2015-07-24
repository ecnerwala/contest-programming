#include<bits/stdc++.h>
using namespace std;

const int MAXN = 300;
int N;
int M;
int adj[MAXN][MAXN];

int cap[MAXN][MAXN];

int prv[MAXN];
int maxflow(int s, int t) {
	int totflow = 0;
	memcpy(cap, adj, sizeof(adj));
	while(true) {
		memset(prv, -1, sizeof(prv));

		queue<int> q;
		q.push(0);
		while(!q.empty()) {
			int v = q.front();
			if(v == t) break;
			q.pop();
			for(int i = 0; i < N; i++) {
				if(prv[i] != -1) continue;
				if(cap[v][i]) {
					prv[i] = v;
					q.push(i);
				}
			}
		}
		if(q.empty()) break;
		for(int i = t, p = prv[i]; i != s; i = p, p = prv[i]) {
			cap[p][i] --;
			cap[i][p] ++;
		}
		totflow++;
	}
	return totflow;
}

int deg[MAXN];
bool pos[MAXN * MAXN];
int mindiff() {
	memset(pos, false, sizeof(pos));
	pos[deg[0]] = true;
	for(int i = 2; i < N; i++) {
		for(int v = N * N; v >= 0; v--) {
			if(pos[v]) pos[v + deg[i]] = true;
		}
	}

	int res = M * 2;
	for(int v = 0; v <= N * N; v++) {
		if(pos[v]) res = min(res, abs(v - M));
	}
	return res;
}

// diff is (sum degrees in a - sum degrees in b) / 2
// (2 * sum degrees in a - (sum all degrees)) / 2 
// (2 * sum degrees in a - 2 * M) / 2
// sum degrees in a - M

int main() {
	int T; cin >> T;
	while(T--) {
		cin >> N >> M;
		memset(adj, 0, sizeof(adj));
		memset(deg, 0, sizeof(deg));
		for(int i = 0; i < M; i++) {
			int a, b; cin >> a >> b;
			a--, b--;
			adj[a][b] = adj[b][a] = true;
			deg[a] ++;
			deg[b] ++;
		}

		int ans1 = M - maxflow(0, 1);
		int ans2 = mindiff();
		cout << ans1 << ' ' << ans2 << '\n';

	}
	return 0;
}
