#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;
const int MAXM = 3e5;

int N, M;

vector<int> adj[MAXN];

vector<int> cl[MAXM];

int vis[MAXN];

bool val[MAXM];
int prv[MAXM];

// v is what you mark visited with
bool dfs(int a, int v) {
	if(vis[a] == v) return false;
	vis[a] = v;

	for(int l : adj[a]) {
		int n = abs(l);
		if(!prv[n]) {
			prv[n] = a;
			val[n] = l > 0;
			return true;
		} else {
			int b = prv[n];

			val[n] ^= 1;
			prv[n] = a;

			if(dfs(b, v)) return true;

			val[n] ^= 1;
			prv[n] = b;
		}
	}

	return false;
}

bool go() {
	for(int i = 1; i <= M; i++) {
		if(cl[i].size() == 2 && (cl[i][0] > 0) == (cl[i][1] > 0)) {
			val[i] = cl[i][0] > 0;
			vis[abs(cl[i][0])] = -1;
			vis[abs(cl[i][1])] = -1;
		}
	}
	for(int i = 1; i <= N; i++) {
		if(vis[i] == -1) continue;
		if(!dfs(i, i)) return false;
	}
	return true;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 1; i <= N; i++) {
		int k; cin >> k;
		adj[i].resize(k);
		for(int j = 0; j < k; j++) {
			cin >> adj[i][j];
			if(adj[i][j] < 0) {
				cl[-adj[i][j]].push_back(-i);
			} else {
				cl[adj[i][j]].push_back(i);
			}
		}
	}

	if(go()) {
		cout << "YES\n";
		for(int i = 1; i <= M; i++) {
			cout << int(val[i]);
		}
		cout << '\n';
	} else {
		cout << "NO\n";
	}
	return 0;
}
