#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
vector<int> adj[MAXN];

int K;

int dfs(int cur, int prv) {
	multiset<int> pathLen;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		int v = dfs(nxt, cur);
		if (v == -1) return -1;
		pathLen.insert(v+1);
	}
	if (pathLen.size() % 2 == 0) pathLen.insert(0);
	pathLen.insert(2*N);
	int res = -1;
	while (pathLen.size() >= 2) {
		int a = *pathLen.begin();
		pathLen.erase(pathLen.begin());
		auto it = pathLen.lower_bound(K-a);
		if (it == pathLen.end()) {
			return -1;
		} else if (*it == 2*N) {
			res = a;
		}
		pathLen.erase(it);
	}
	return res;
}

bool isGood(int K_) {
	K = K_;
	int ROOT = 1;
	while (adj[ROOT].size() != 1) ROOT++;
	int l = dfs(adj[ROOT][0], ROOT);
	if (l == -1) return false;
	return l + 1 >= K;
}

int main() {
	if (fopen("deleg.in", "r")) {
		freopen("deleg.in", "r", stdin);
		freopen("deleg.out", "w", stdout);
	}
	cin >> N;
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	if (N == 1) {
		// what do?
		cout << 1 << '\n';
		exit(0);
	}

	int mi = 1;
	int ma = N;
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (isGood(md)) {
			mi = md;
		} else {
			ma = md;
		}
	}
	cout << mi << '\n';

	return 0;
}
