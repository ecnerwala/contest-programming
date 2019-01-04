#include<bits/stdc++.h>
using namespace std;

const int MAXN = 5e5;
int N;
vector<int> adj[MAXN];
int par[MAXN];

void dfs_par(int cur, int prv = 0) {
	par[cur] = prv;
	if (prv != 0) {
		for (auto it = adj[cur].begin(); true; it++) {
			if (*it == prv) {
				adj[cur].erase(it);
				break;
			}
		}
	}

	for (int nxt : adj[cur]) {
		assert(nxt != prv);
		dfs_par(nxt, cur);
	}
}

int dfs_cnt(int cur, int top) {
	if (cur > top) {
		return 0;
	}
	int res = 1;
	for (int nxt : adj[cur]) {
		res += dfs_cnt(nxt, top);
	}
	return res;
}

int C[MAXN];

// v is answer, assuming you're <= top
void solve(int cur, int top = 0, int v = 0) {
	cerr << "Solve\n";
	cerr << cur << ' ' << top << ' ' << v << '\n';
	if (cur > top) {
		C[cur] = v;
		for (int nxt : adj[cur]) {
			C[cur] += dfs_cnt(nxt, top);
		}
		for (int nxt : adj[cur]) {
			solve(nxt, cur, C[cur] + dfs_cnt(nxt, cur) - dfs_cnt(nxt, top) + 1);
		}
	} else {
		C[cur] = v-1;
		for (int nxt : adj[cur]) {
			solve(nxt, top, v);
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N-1; i++) {
		int a, b; cin >> a >> b;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	dfs_par(1);
	solve(1);
	for (int i = 2; i <= N; i++) {
		cout << C[i] << " \n"[i==N];
	}

	return 0;
}
