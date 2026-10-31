#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
vector<int> adj[MAXN];
int K;

vector<int> numE[MAXN];
int sz[MAXN];

int dfs(int cur, int prv) {
	sz[cur] = 0;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		int v = dfs(nxt, cur) + 1;
		numE[cur].push_back(v);
		sz[cur] += v;
	}
	return sz[cur];
}

bool solve(int K_) {
	K = K_;
	if ((N-1) % K) return false;
	if (K == 1) return true;
	vector<int> cnt(K);
	vector<int> vals;
	for (int i = 0; i < N; i++) {
		int exclude = sz[i] % K;
		vals.clear();
		for (auto v : numE[i]) {
			v %= K;
			if (!v) continue;
			if (v == exclude) {
				exclude = 0;
				continue;
			}

			if (cnt[K-v]) {
				cnt[K-v] --;
			} else {
				cnt[v]++;
				vals.push_back(v);
			}
		}
		for (auto v : vals) {
			if (cnt[v]) return false;
		}
	}
	return true;
}

int main() {
	if (fopen("deleg.in", "r")) {
		freopen("deleg.in", "r", stdin);
		freopen("deleg.out", "w", stdout);
	}

	cin >> N;
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	dfs(0, -1);
	for (int k = 1; k <= N-1; k++) {
		cout << (solve(k) ? '1' : '0');
	}
	cout << '\n';

	return 0;
}
