#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e4;
int N, M;
vector<int> adj[MAXN];

char vis[MAXN]; // if in, 2, else 1

bool dfs(int a) {
	if(vis[a]) return vis[a] - 1;
	vis[a] = 2;
	vis[a^1] = 1;
	for(vector<int>::iterator it = adj[a].begin(); it != adj[a].end(); it++) {
		if(!dfs(*it)) {
			vis[a] = vis[a^1] = 0;
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
		if(vis[i] - 1) {
			printf("%d\n", i + 1);
		}
	}
}
