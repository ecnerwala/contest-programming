#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

const int MAXN = 2e4;
int N;
vector<int> adj[MAXN];

const int MAXK = 2e3;
int K;
int ant[MAXN];
int cnt[MAXK];
int pos[MAXK];

int tim[MAXN];

int dfs_tim(int cur, int prv) {
	if(ant[cur] != -1) {
		return tim[cur] = ant[cur];
	}
	int res = INF;
	for(int i = 0; i < int(adj[cur].size()); i++) {
		int nxt = adj[cur][i];
		if(nxt == prv) continue;
		res = min(res, dfs_tim(nxt, cur) + K);
	}
	return tim[cur] = res;
}

void dfs_move(int cur, int prv, int time) {
	if(ant[cur] != -1) {
		return;
	}
	if(time / K < tim[cur] / K) {
		tim[cur] = tim[cur] % K + time / K * K;
	}
	for(int i = 0; i < int(adj[cur].size()); i++) {
		int nxt = adj[cur][i];
		if(nxt == prv) continue;
		dfs_move(nxt, cur, tim[cur]);
		assert(tim[nxt] + K >= tim[cur]);
		if(tim[nxt] + K == tim[cur]) {
			assert(ant[cur] == -1);
			ant[cur] = ant[nxt];
			ant[nxt] = -1;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N - 1; i++) {
		int a, b; cin >> a >> b; a--, b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	memset(ant, -1, sizeof(ant));
	cin >> K;
	for(int i = 0; i < K; i++) {
		int l; cin >> l; l--;
		ant[l] = i;
	}
	int Q;
	cin >> Q;
	for(int i = 0; i < Q; i++) {
		int l; cin >> l; l--;
		int t = dfs_tim(l, -1);
		cnt[t % K] ++;
		dfs_move(l, -1, t);
	}
	for(int i = 0; i < N; i++) {
		if(ant[i] == -1) continue;
		pos[ant[i]] = i;
	}
	for(int i = 0; i < K; i++) {
		cout << pos[i] + 1 << ' ' << cnt[i] << '\n';
	}
	return 0;
}
