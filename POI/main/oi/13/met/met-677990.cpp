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
		u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
		deg[u] ++;
		deg[v] ++;
	}

	queue<int> q;

	for(int i = 0; i < N; i++) {
		if(deg[i] == 1) {
			deg[i] = -1;
			q.push(i);
		}
	}

	int res = N;
	while(int(q.size()) > L * 2) {
		int cur = q.front(); q.pop();
		assert(deg[cur] < 0);
		int cst = deg[cur];
		vector<int>::iterator it = adj[cur].begin();
		while(deg[*it] < 0) it++;
		int nxt = *it;
		assert(deg[nxt] >= 2);
		if(--deg[nxt] == 1) {
			deg[nxt] = cst - 1;
			q.push(nxt);
		} else {
			res += cst;
		}
	}
	cout << res << '\n';

	return 0;
}
