#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e6;
int N, M;
vector<int> adj[MAXN];

bool vis[MAXN];

bool dfs(int a) {
	if(vis[a]) return true;
	if(vis[a ^ 1]) return false;
	vis[a] = true;
	for(int i = 0; i < int(adj[a].size()); i ++) {
		if(!dfs(adj[a][i])) {
			vis[a] = false;
			return false;
		}
	}
	return true;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 0; i < M; i++) {
		int a, b; cin >> a >> b;
		a--, b--;
		adj[a].push_back(b^1);
		adj[b].push_back(a^1);
	}
	for(int i = 0; i < 2 * N; i++) {
		dfs(i);
	}
	bool good = true;
	for(int i = 0; i < 2 * N; i++) {
		assert(!(vis[i] && vis[i ^ 1]));
		if(vis[i] == vis[i ^ 1]) {
			printf("NIE\n");
			exit(0);
		}
	}
	for(int i = 0; i < 2 * N; i++) {
		assert(vis[i] != vis[i ^ 1]);
		if(vis[i]) {
			printf("%d\n", i + 1);
		}
	}
}
