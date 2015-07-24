#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 2e5;
const int MAXM = 2e5;
int N, M;

pii G;

vector<int> adj[MAXN];
pii cnt[MAXN];

int ans = 0;

int val[MAXN];
int lowval[MAXN];
void dfs(int cur, int prv = -1) {
	assert(val[cur] > 0);
	lowval[cur] = val[cur];
	
	for(int nxt : adj[cur]) {
		if(nxt == prv) continue;
		if(val[nxt]) {
			lowval[cur] = min(lowval[cur], val[nxt]);
		} else {
			val[nxt] = val[cur] + 1;

			dfs(nxt, cur);
			lowval[cur] = min(lowval[cur], lowval[nxt]);

			cnt[cur].A += cnt[nxt].A;
			cnt[cur].B += cnt[nxt].B;

			if(lowval[nxt] > val[cur]) {
				if(cnt[nxt].A == G.A || cnt[nxt].B == G.B || cnt[nxt].A == 0 || cnt[nxt].B == 0) {
					ans++;
				}
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> G.A >> G.B;
	for(int i = 0; i < G.A; i++) {
		int v; cin >> v;
		cnt[v].A ++;
	}
	for(int i = 0; i < G.B; i++) {
		int v; cin >> v;
		cnt[v].B ++;
	}

	for(int i = 0; i < M; i++) {
		int u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	val[1] = 1;
	dfs(1);
	cout << ans << '\n';
}
