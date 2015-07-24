#include<bits/stdc++.h>

using namespace std;

const int MAXN = 1e6;
const int MAXM = 2e6;

int N, M, K;
vector<int> adj[MAXN];
bool covered[MAXN];
bool vis[MAXN];

vector<int> ans;

int main() {
	scanf("%d%d%d", &N, &M, &K);
	for(int i = 0; i < M; i++) {
		int a, b; scanf("%d%d", &a, &b);
		a--, b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	for(int i = 0; i < N; i++) {
		if(vis[i]) continue;
		ans.push_back(i);
		covered[i] = true;
		vis[i] = true;
		for(vector<int>::iterator it = adj[i].begin(); it != adj[i].end(); it++) {
			int j = *it;
			if(covered[j]) continue;
			covered[j] = true;
			vis[j] = true;
			for(vector<int>::iterator jt = adj[j].begin(); jt != adj[j].end(); jt++) {
				int k = *jt;
				vis[k] = true;
			}
		}
	}
	printf("%d\n", ans.size());
	assert(ans.size() > 0);
	printf("%d", ans[0] + 1);
	for(int i = 1; i < ans.size(); i++) {
		printf(" %d", ans[i] + 1);
	}
	printf("\n");
	return 0;
}
