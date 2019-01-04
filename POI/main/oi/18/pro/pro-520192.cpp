#include<iostream>
#include<algorithm>
#include<cstring>

using namespace std;

const int MAXN = 1000, MAXM = 1000, MAXV = MAXN + MAXN;

int N, M;
int R, T;
int K;

int numSolves[MAXN];
bool adj[MAXN][MAXM];
int solver[MAXM];

bool vis[MAXN];

//see if we can augment onto this person
bool dfs(int cur) {
	if(vis[cur]) return false;
	vis[cur] = true;

	for(int i = 0; i < M; i++) {
		if(adj[cur][i]) {
			int nxt = solver[i];
			
			if(nxt == -1) {
				solver[i] = cur;
				adj[cur][i] = false;
				return true;
			}
			
			solver[i] = cur;
			adj[nxt][i] = true;
			adj[cur][i] = false;

			if(dfs(nxt)) return true;

			solver[i] = nxt;
			adj[nxt][i] = false;
			adj[cur][i] = true;
		}
	}

	return false;
}

int flow = 0;
long long cost = 0;

void go() {
	//memset(numSolves, 0, sizeof(numSolves)) // global
	//memset(vis, 0, sizeof(vis)); // global
	memset(solver, -1, sizeof(solver));
	T /= R;
	if(T > M) T = M;
	for(int t = 0; t < T; t++) {
		for(int i = 0; i < N; i++) {
			if(numSolves[i] == t) {
				if(dfs(i)) {
					numSolves[i]++;
					flow ++;
					cost += t;
					memset(vis, 0, sizeof(vis));
				}
			}
		}
	}
	cost += flow;
	cost *= R;
}

int main() {
	cin >> N >> M >> R >> T >> K;

	//memset(adj, 0, sizeof(adj)) // global
	for(int i = 0; i < K; i++) {
		int a, b; 
		cin >> a >> b;
		a--, b--;
		adj[a][b] = true;
	}

	go();

	cout << flow << ' ' << cost << '\n';

	for(int i = 0; i < M; i++) {
		int p = solver[i];
		if(p >= 0) {
			numSolves[p] --;
			cout << (p + 1) << ' ' << (i + 1) << ' ' << (numSolves[p] * R) << '\n';
		}
	}

	return 0;
}
