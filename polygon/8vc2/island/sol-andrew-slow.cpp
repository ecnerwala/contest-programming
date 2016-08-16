#include<bits/stdc++.h>
using namespace std;

typedef vector<int> vi;

const int MAXN = 3e5;
int N;
vi A, B;
vector<int> adj[MAXN];

int search() {
	map<vi, int> vis;
	queue<vi> q;
	vis[A] = 0;
	q.push(A);
	while(!q.empty()) {
		vi cur = q.front(); q.pop();
		int d = vis[cur];
		if(cur == B) return d;
		for(int i = 0; i < N; i++) {
			if(cur[i] == 0) {
				for(int j : adj[i]) {
					swap(cur[j], cur[i]);
					if(!vis.count(cur)) {
						vis[cur] = d + 1;
						q.push(cur);
					}
					swap(cur[j], cur[i]);
				}
			}
		}
	}
	return -1;
}

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		int v; scanf("%d", &v);
		A.push_back(v);
	}
	for(int i = 0; i < N; i++) {
		int v; scanf("%d", &v);
		B.push_back(v);
	}
	for(int i = 0; i < N - 1; i++) {
		int u, v; scanf("%d %d", &u, &v); u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	int best = search();
	if(best != -1) {
		cout << 0 << ' ' << best << '\n';
		return 0;
	}
	int u = -1, v = -1;
	for(int a = 0; a < N; a++) {
		for(int b = a + 1; b < N; b++) {
			adj[a].push_back(b);
			adj[b].push_back(a);

			int val = search();

			adj[a].pop_back();
			adj[b].pop_back();

			if(val == -1) continue;
			assert(best == -1);
			best = val;
			u = a, v = b;
		}
	}
	u++, v++;
	if(best == -1) cout << -1 << '\n';
	else cout << min(u, v) << ' ' << max(u, v) << ' ' << best << '\n';
	return 0;
}
