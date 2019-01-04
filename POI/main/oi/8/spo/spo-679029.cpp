#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e4;
int N, M;
vector<int> adj[MAXN];

bool vis[MAXN];

bool dfs(int a) {
	if(vis[a]) return true;
	if(vis[a ^ 1]) return false;
	vis[a] = true;
	for(vector<int>::iterator it = adj[a].begin(); it != adj[a].end(); it++) {
		if(!dfs(*it)) {
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
	for(int i = 0; i < N; i++) {
		if(dfs(2 * i)) {
		} else if(dfs(2 * i + 1)) {
		} else {
			printf("NIE\n");
			exit(0);
		}
	}
	for(int i = 0; i < 2 * N; i++) {
		if(vis[i]) {
			printf("%d\n", i + 1);
		}
	}
}
