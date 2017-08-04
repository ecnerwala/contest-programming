#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = int(5e5) + 200;
int N;
int T[MAXN];
vector<int> adj[MAXN];
ll S;

ll R;

// return the amount passed upwards
ll dfs(int cur, int prv = -1) {
	ll sum = T[cur] - S;
	for (vector<int>::iterator it = adj[cur].begin(); it != adj[cur].end(); it++) {
		int nxt = *it;
		if (nxt == prv) continue;
		sum += dfs(nxt, cur);
	}
	R += abs(sum);
	return sum;
}

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &T[i]);
		S += T[i];
	}
	assert(S % N == 0);
	S /= N;
	for (int i = 0; i < N-1; i++) {
		int u, v; scanf("%d %d", &u, &v);
		u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	R = 0;
	dfs(0);

	printf("%lld\n", R);

	return 0;
}
