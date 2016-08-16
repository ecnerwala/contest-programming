#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

const int MAXV = 2e5;
const int MAXN = 400;
const int MAXK = 400;
int N, K;
int V;
unordered_map<char, int> nxt[MAXV];
int cnt[MAXV];
int dp[MAXV][MAXK];

void dfs(int cur) {
	for(int i = 0; i <= K; i++) {
		dp[cur][i] = (i <= cnt[cur]) ? i : INF;
	}
	for(auto it : nxt[cur]) {
		dfs(it.second);
		for(int i = K; i >= 0; i--) {
			for(int j = 1; i + j <= K; j++) {
				dp[cur][i + j] = min(dp[cur][i + j], dp[cur][i] + dp[it.second][j] + 2);
			}
		}
	}
}

int go() {
	cin >> N >> K;
	nxt[0].clear();
	V = 1;
	memset(cnt, 0, sizeof(cnt));
	for(int i = 0; i < N; i++) {
		string s; cin >> s;
		int cur = 0;
		for(char c : s) {
			if(nxt[cur].count(c)) {
				cur = nxt[cur][c];
			} else {
				nxt[V].clear();
				cur = (nxt[cur][c] = (V++));
			}
		}
		cnt[cur] ++;
	}
	dfs(0);
	return dp[0][K];
}

int main() {
	ios_base::sync_with_stdio(0);
	int __T; cin >> __T;
	for(int T = 1; T <= __T; T++) {
		int res = go();
		cout << "Case #" << T << ": " << res << '\n';
	}

	return 0;
}
