#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 3e5;

int N;
vector<int> adj[MAXN];
int sz[MAXN];

int dfssz(int cur, int prv = -1) {
	sz[cur] = 1;
	for(vector<int>::iterator it = adj[cur].begin(); it != adj[cur].end(); it++) {
		int nxt = *it;
		if(nxt == prv) continue;
		sz[cur] += dfssz(nxt, cur);
	}
	return sz[cur];
}

int findcentroid(int cur, int prv = -1) {
	if(sz[cur] * 2 < N) return -1;
	for(vector<int>::iterator it = adj[cur].begin(); it != adj[cur].end(); it++) {
		int nxt = *it;
		if(nxt == prv) continue;
		int v = findcentroid(nxt, cur);
		if(v >= 0) return v;
	}
	return cur;
}

int dp[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N - 1; i++) {
		int a, b; cin >> a >> b;
		a--, b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	dfssz(0);
	int cent = findcentroid(0);
	dfssz(cent);

	ll res = 0;

	for(int i = 0; i < N; i++) {
		res += sz[i] - 1;
	}

	map<int, int> vals;
	for(vector<int>::iterator it = adj[cent].begin(); it != adj[cent].end(); it++) {
		vals[sz[*it]] ++;
	}

	memset(dp, -1, sizeof(dp));

	dp[0] = 0;

	for(map<int, int>::iterator it = vals.begin(); it != vals.end(); it++) {
		int v = it->first;
		int c = it->second;
		for(int i = 0; i + v <= N; i++) {
			if(dp[i] >= 0 && dp[i] < c && dp[i + v] == -1) {
				dp[i + v] = dp[i] + 1;
			}
		}
		for(int i = 0; i <= N; i++) {
			if(dp[i] >= 0) dp[i] = 0;
		}
	}
	for(int i = N / 2; i >= 0; i--) {
		if(dp[i] >= 0) {
			res += ll(i) * ll(N - 1 - i);
			break;
		}
	}
	cout << N - 1 << ' ' << res << '\n';

	return 0;
}
