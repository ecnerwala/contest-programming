#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 1e4;

int N;
vector<int> adj[MAXN];

ll dp[MAXN][3];
int cnt[MAXN];

void dfs(int cur, int prv, int dist = 1) {
	cnt[dist] ++;
	dist++;
	for(vector<int>::iterator it = adj[cur].begin(); it != adj[cur].end(); it++) {
		int nxt = *it;
		if(nxt == prv) continue;

		dfs(nxt, cur, dist);
	}
}

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N - 1; i++) {
		int a, b;

		scanf("%d%d", &a, &b);
		a--, b--;

		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	ll ans = 0;

	for(int a = 0; a < N; a++) {
		for(vector<int>::iterator it = adj[a].begin(); it != adj[a].end(); it++) {
			dfs(*it, a);

			for(int d = 1; d < N; d++) {
				ll v = cnt[d];
				cnt[d] = 0;
				ans += v * dp[d][2];
				dp[d][2] += v * dp[d][1];
				dp[d][1] += v;
			}
		}

		for(int d = 1; d < N; d++) {
			dp[d][1] = dp[d][2] = 0;
		}
	}

	printf("%lld\n", ans);

	return 0;
}
