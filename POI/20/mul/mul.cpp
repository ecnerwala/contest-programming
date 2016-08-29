#include<bits/stdc++.h>
using namespace std;

const int MAXN = 6e5;
int N;
vector<int> adj[MAXN];

int dp[MAXN];
int cnt[MAXN][8];
vector<int> ch[MAXN][8];

vector<int> res;

int dfs(bool build, int cur, int prv = -1, int depth = 1) {
	if(!build) {
		for(vector<int>::iterator it = adj[cur].begin(); it != adj[cur].end(); it++) {
			int nxt = *it;
			if(nxt == prv) continue;
			int v = dfs(false, nxt, cur);
			cnt[cur][v] ++;
			ch[cur][v].push_back(nxt);
			cnt[cur][0] ++;
		}
	}

	if(build) {
		assert(dp[cur] < 5);
		if(depth == 1) assert(dp[cur] <= 3);
		if(dp[cur] == 4) assert(depth == 2);
		if(depth == 2) assert(dp[cur] == 2 || dp[cur] == 4);
	}
	if(cnt[cur][0] == 0) {
		dp[cur] = 1;
		if(build) {
			res.push_back(cur);
		}
	} else if(cnt[cur][5]) {
		dp[cur] = 5;
		if(build) {
			assert(false);
		}
	} else if(cnt[cur][4] + cnt[cur][3] >= 2) {
		dp[cur] = 5;
		if(build) {
			assert(false);
		}
	} else if(cnt[cur][4]) {
		if(!cnt[cur][1] && !cnt[cur][2]) {
			dp[cur] = 3;
			if(build) {
				res.push_back(cur);
				dfs(true, ch[cur][4][0], cur, 2);
			}
		} else if(cnt[cur][2] <= 1) {
			dp[cur] = 4;
			if(build) {
				for(int i = 0; i < cnt[cur][1]; i++) {
					dfs(true, ch[cur][1][i], cur, 1);
				}
				if(cnt[cur][2]) dfs(true, ch[cur][2][0], cur, 1);
				res.push_back(cur);
				dfs(true, ch[cur][4][0], cur, 2);
			}
		} else {
			dp[cur] = 5;
			if(build) {
				assert(false);
			}
		}
	} else if(cnt[cur][3]) {
		if(cnt[cur][2] <= 1) {
			dp[cur] = 3;
			if(build) {
				res.push_back(cur);
				if(cnt[cur][2]) dfs(true, ch[cur][2][0], cur, 2);
				for(int i = 0; i < cnt[cur][1]; i++) {
					dfs(true, ch[cur][1][i], cur, 1);
				}
				dfs(true, ch[cur][3][0], cur, 1);
			}
		} else if(cnt[cur][2] == 2) {
			dp[cur] = 4;
			if(build) {
				for(int i = 0; i < cnt[cur][1]; i++) {
					dfs(true, ch[cur][1][i], cur, 1);
				}
				dfs(true, ch[cur][2][0], cur, 1);
				res.push_back(cur);
				dfs(true, ch[cur][2][1], cur, 2);
				dfs(true, ch[cur][3][0], cur, 1);
			}
		} else {
			dp[cur] = 5;
			if(build) {
				assert(false);
			}
		}
	} else {
		if(cnt[cur][2] <= 1) {
			dp[cur] = 2;
			if(build) {
				if(depth == 2) {
					for(int i = 0; i < cnt[cur][1]; i++) {
						dfs(true, ch[cur][1][i], cur, 1);
					}
					if(cnt[cur][2]) dfs(true, ch[cur][2][0], cur, 1);
					res.push_back(cur);
				} else {
					res.push_back(cur);
					if(cnt[cur][2]) dfs(true, ch[cur][2][0], cur, 2);
					for(int i = 0; i < cnt[cur][1]; i++) {
						dfs(true, ch[cur][1][i], cur, 1);
					}
				}
			}
		} else if(cnt[cur][2] == 2) {
			dp[cur] = 3;
			if(build) {
				res.push_back(cur);
				dfs(true, ch[cur][2][0], cur, 2);
				for(int i = 0; i < cnt[cur][1]; i++) {
					dfs(true, ch[cur][1][i], cur, 1);
				}
				dfs(true, ch[cur][2][1], cur, 1);
			}
		} else if(cnt[cur][2] == 3) {
			dp[cur] = 4;
			if(build) {
				for(int i = 0; i < cnt[cur][1]; i++) {
					dfs(true, ch[cur][1][i], cur, 1);
				}
				dfs(true, ch[cur][2][0], cur, 1);
				res.push_back(cur);
				dfs(true, ch[cur][2][1], cur, 2);
				dfs(true, ch[cur][2][2], cur, 1);
			}
		} else {
			dp[cur] = 5;
			if(build) {
				assert(false);
			}
		}
	}

	if(cur == N) {
		if(dp[cur] == 1) {
			dp[cur] = 3;
		} else if(dp[cur] == 2) {
			dp[cur] = 4;
		} else {
			dp[cur] = 5;
		}
	}

	return dp[cur];
}

int main() {
	scanf("%d", &N);
	for(int i = 1; i <= N - 1; i++) {
		int u, v;
		scanf("%d %d", &u, &v);
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	if(dfs(false, 1) <= 3) {
		dfs(true, 1);
		for(int i = 0; i < N; i++) {
			printf("%d\n", res[i]);
		}
	} else {
		printf("BRAK\n");
	}
	return 0;
}

/*
possibilities:
subsets of nxt (in order of most flexible -> most demanding)

type 1 - can go in & out (1 & 1) (leaf node)
type 2 - can go in & out (1 & 2)
type 3 - only in by 1
type 4 - only in by 2
type 5 - can't do 1-sided

type 1 - leaf
type 2 - inf type 1 + type 2, inf type 1
type 3 - start or end or type 4 or type 3 + inf type 1 + type 2, type 3 + inf type 1, type 3 + inf type
type 4 - inf type 1 + type 2 + type 4, inf type 1 + type 2 + type 2 + type 3
type 5 - else

we win if we have 3 + 4
*/
