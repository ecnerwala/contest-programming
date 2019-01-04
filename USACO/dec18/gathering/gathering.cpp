#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
const int MAXM = 2e5;
int N;
vector<int> adj[MAXN];
int treeDeg[MAXN];

bool vis[MAXN];

int M;
vector<int> outEdges[MAXN];
int inDeg[MAXN];

bool D[MAXN];

void dfs(int cur, int prv = 0) {
	if (!outEdges[cur].empty()) return;
	D[cur] = true;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfs(nxt, cur);
	}
}

void go() {
	stack<int> q;
	assert(N >= 2);
	for (int i = 1; i <= N; i++) {
		if (treeDeg[i] <= 1 && inDeg[i] == 0) {
			vis[i] = true;
			q.push(i);
		}
	}
	int root = 0;
	for (int t = 1; t <= N; t++) {
		if (q.empty()) {
			return;
		}
		int cur = q.top(); q.pop();
		if (t == N) {
			root = cur;
			break;
		}
		for (int nxt : outEdges[cur]) {
			assert(!vis[nxt]);
			inDeg[nxt] --;
			if (inDeg[nxt] == 0 && treeDeg[nxt] <= 1) {
				vis[nxt] = true;
				q.push(nxt);
			}
		}
		for (int nxt : adj[cur]) {
			if (vis[nxt]) continue;
			treeDeg[nxt] --;
			if (treeDeg[nxt] <= 1 && inDeg[nxt] == 0) {
				vis[nxt] = true;
				q.push(nxt);
			}
		}
	}
	dfs(root);
}

int main() {
	if(fopen("gathering.in", "r")) {
		FILE* a = freopen("gathering.in", "r", stdin);
		FILE* b = freopen("gathering.out", "w", stdout);
		assert(a && b);
	}
	cin >> N >> M;
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
		treeDeg[u]++;
		treeDeg[v]++;
	}
	for (int i = 0; i < M; i++) {
		int a, b; cin >> a >> b;
		outEdges[a].push_back(b);
		inDeg[b]++;
	}

	go();
	for (int i = 1; i <= N; i++) {
		cout << "01"[D[i]] << '\n';
	}

	return 0;
}
