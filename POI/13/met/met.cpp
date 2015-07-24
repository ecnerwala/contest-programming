#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e6;
int N;
vector<int> adj[MAXN];
int deg[MAXN];

int L;

int main() {
	scanf("%d %d", &N, &L);
	if(L == 0) {
		printf("0\n");
		return 0;
	}

	for(int i = 0; i < N - 1; i++) {
		int u, v;
		scanf("%d %d", &u, &v);
		adj[u].push_back(v);
		adj[v].push_back(u);
		deg[u] ++;
		deg[v] ++;
	}

	queue<int> q;
	for(int i = 1; i <= N; i++) {
		if(!(--deg[i])) {
			deg[i] = -1;
			q.push(i);
		}
	}

	int res = N;
	for(int v = int(q.size()) - 2 * L; v > 0;) {
		int cur = q.front(); q.pop();
		vector<int>::iterator it = adj[cur].begin();
		while(deg[*it] < 0) it++;
		int nxt = *it;
		if(!(--deg[nxt])) {
			deg[nxt] = deg[cur] - 1;
			q.push(nxt);
		} else {
			res += deg[cur];
			v--;
		}
	}

	cout << res << '\n';

	return 0;
}
