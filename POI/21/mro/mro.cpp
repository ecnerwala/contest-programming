#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 2e9;

const int MAXN = 2e6;
const int MAXG = 2e6;
int N;
int A[MAXN], B[MAXN];
vector<int> adj[MAXN];

int G; 
ll M[MAXG];

ll K;

ll res = 0;

void update(ll val) {
	if(val >= INF) return;
	res += lower_bound(M, M + G, val * (K + 1)) - lower_bound(M, M + G, val * K);
}

void dfs(int cur, int prv, ll val = 1) {
	if(adj[cur].size() == 1) update(val);

	val *= int(adj[cur].size()) - 1;
	if(val >= INF) val = INF;

	for(vector<int>::iterator it = adj[cur].begin(); it != adj[cur].end(); it++) {
		int nxt = *it;
		if(nxt == prv) continue;

		dfs(nxt, cur, val);
	}
}

int main() {
	scanf("%d%d%lld", &N, &G, &K);

	for(int i = 0; i < G; i++) {
		scanf("%lld", &M[i]);
	}

	sort(M, M + G);

	for(int i = 0; i < N - 1; i++) {
		scanf("%d%d", &A[i], &B[i]);
		A[i] --, B[i] --;
		adj[A[i]].push_back(B[i]);
		adj[B[i]].push_back(A[i]);
	}

	dfs(A[0], B[0]);
	dfs(B[0], A[0]);

	res *= K;
	printf("%lld\n", res);
	return 0;
}
