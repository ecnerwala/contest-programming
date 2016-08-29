#include<bits/stdc++.h>
using namespace std;

const int MAXN = 4e5;
int N;
vector<int> adj[MAXN];

int C;

int dfs(int cur, int prv = -1) {
	int res = 0;
	for(vector<int>::iterator it = adj[cur].begin(); it != adj[cur].end(); it++) {
		int nxt = *it;
		if(nxt == prv) continue;
		res += dfs(nxt, cur);
	}
	res -= C;
	if(res < 0) res = 0;
	res++;
	return res;
} 

bool isgood(int c) {
	C = c;
	return dfs(0) == 1;
}

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N - 1; i++) {
		int a, b; scanf("%d %d", &a, &b);
		a--, b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	int mi = -1;
	int ma = N;
	while(ma - mi > 1) {
		int md = (mi + ma) / 2;
		if(isgood(md)) {
			ma = md;
		} else {
			mi = md;
		}
	}
	printf("%d\n", ma);
	return 0;
}
