#include<iostream>
#include<vector>

using namespace std;

const int MAXN = 2e5;

int N, M;
vector<int> adj[MAXN];
bool vis[MAXN];
int par[MAXN]; //pseudo-parent

void dfsPar(int v) {
	for(__typeof(adj[v].begin()) it = adj[v].begin(); it != adj[v].end(); it++) {
		int n = *it;
		if(par[n] == -1) {
			par[n] = v;
			dfsPar(n);
		}
	}
}

bool dfsCyc(int v, int p = -1) {
	vis[v] = true;
	for(__typeof(adj[v].begin()) it = adj[v].begin(); it != adj[v].end(); it++) {
		int n = *it;
		if(n == p) continue;
		if(vis[n]) {
			if(par[n] == -1) {
				par[n] = v;
				dfsPar(n);
			}
		} else {
			dfsCyc(n, v);
		}
	}
	return false;
}

int main() {
	cin >> N >> M;
	for(int i = 0; i < M; i++) {
		int a, b; cin >> a >> b; a--, b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	for(int i = 0; i < N; i++) { par[i] = -1; }
	for(int i = 0; i < N; i++) {
		if(!vis[i]) {
			dfsCyc(i);
		}
	}
	bool isGood = true;
	for(int i = 0; i < N; i++) {
		if(par[i] == -1) {
			isGood = false;
		}
	}
	if(isGood) {
		cout << "TAK" << '\n';
		for(int i = 0; i < N; i++) {
			cout << par[i] + 1 << '\n';
		}
	} else {
		cout << "NIE" << '\n';
	}
	return 0;
}
