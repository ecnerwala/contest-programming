#include<iostream>
#include<algorithm>
#include<cstring>
#include<queue>

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
			
			solver[i] = cur;
			adj[cur][i] = false;

			if(nxt == -1) {
				return true;
			}
			
			adj[nxt][i] = true;

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
	queue<int> q;
	for(int i = 0; i < N; i++) q.push(i);
	while(!q.empty()) {
		int i = q.front(); q.pop();
		if(numSolves[i] == T) break;

		if(dfs(i)) {
			flow ++;
			numSolves[i]++;
			cost += numSolves[i];
			q.push(i);
			memset(vis, 0, sizeof(vis));
		}
	}
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
