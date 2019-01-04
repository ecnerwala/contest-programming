#include<iostream>
#include<cstring>
#include<vector>
#include<cassert>

using namespace std;

const int MAXN = 4e5;
const int MAXM = 1e6;

int N, M;
vector<int> adj[MAXN];

bool guild[MAXN];
bool vis[MAXN];

void dfs(int i, bool v = false) {
	vis[i] = true;
	guild[i] = v;
	for(__typeof(adj[i].begin()) it = adj[i].begin(); it != adj[i].end(); it++) {
		int n = *it;
		if(!vis[n]) {
			dfs(n, !v);
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 0; i < M; i++) {
		int a, b; cin >> a >> b; a--, b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	bool good = true;
	for(int i = 0; i < N; i++) {
		if(adj[i].empty()) good = false;
	}
	if(good) {
		cout << "TAK" << '\n';
		for(int i = 0; i < N; i++) {
			if(!vis[i]) dfs(i);
			cout << (guild[i] ? 'K' : 'S') << '\n';
		}
	} else {
		cout << "NIE" << '\n';
	}
}
