#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;

int N, K;
int A[MAXN];
vector<int> adj[MAXN];

int V;

int dp_down[MAXN];
int sum_whole[MAXN];
int best_parts[MAXN][2];
int dp_up[MAXN];

// positive is full
// <= 0 is partial
void dfs1(int cur, int prv = 0) {
	sum_whole[cur] = (A[cur] >= V);
	best_parts[cur][0] = best_parts[cur][1] = 1;
	for(int nxt : adj[cur]) {
		if(nxt == prv) continue;

		dfs1(nxt, cur);

		int v = dp_down[nxt];
		if(v > 0) {
			sum_whole[cur] += v;
		} else if(v < best_parts[cur][0]) {
			best_parts[cur][1] = best_parts[cur][0];
			best_parts[cur][0] = v;
		} else if(v < best_parts[cur][1]) {
			best_parts[cur][1] = v;
		}
	}

	if(A[cur] < V) dp_down[cur] = 0;
	else if(best_parts[cur][0] <= 0) dp_down[cur] = best_parts[cur][0] - sum_whole[cur];
	else dp_down[cur] = sum_whole[cur];
}

void dfs2(int cur, int prv = 0) {
	if(prv) {
		int v = dp_up[cur];
		if(v > 0) {
			sum_whole[cur] += v;
		} else if(v < best_parts[cur][0]) {
			best_parts[cur][1] = best_parts[cur][0];
			best_parts[cur][0] = v;
		} else if(v < best_parts[cur][1]) {
			best_parts[cur][1] = v;
		}
	}

	for(int nxt : adj[cur]) {
		if(nxt == prv) continue;
		int parts = best_parts[cur][0];
		if(parts == dp_down[nxt]) parts = best_parts[cur][1];

		int wholes = sum_whole[cur];
		if(dp_down[nxt] > 0) wholes -= dp_down[nxt];

		if(A[cur] < V) dp_up[nxt] = 0;
		else if(parts <= 0) dp_up[nxt] = parts - wholes;
		else dp_up[nxt] = wholes;

		dfs2(nxt, cur);
	}
}

bool is_good(int v) {
	V = v;
	dfs1(1);
	dfs2(1);
	for(int i = 1; i <= N; i++) {
		if(A[i] < V) continue;
		if(sum_whole[i] + max(0, -best_parts[i][0]) >= K) return true;
	}
	return false;
}

int main() {
	scanf("%d %d", &N, &K);
	for(int i = 1; i <= N; i++) scanf("%d", &A[i]);
	for(int i = 0; i < N - 1; i++) {
		int u, v; scanf("%d %d", &u, &v);
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	int mi = 0;
	int ma = 1e6 + 1;
	while(ma - mi > 1) {
		int md = (mi + ma) / 2;
		if(is_good(md)) {
			mi = md;
		} else {
			ma = md;
		}
	}
	cout << mi << '\n';
}
