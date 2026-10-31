#include<bits/stdc++.h>
using namespace std;

int V;
vector<vector<pair<int, int>>> adj;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	string S; cin >> S;
	int M = int(S.size());
	reverse(S.begin(), S.end());
	for (int L = 1; L < M; L++) {
		V = 2 * M;

		adj.assign(V, vector<pair<int, int>>());

		for (int i = 0; i < L-1-i; i++) {
			adj[M+i].emplace_back(M+L-1-i, 0);
			adj[M+L-1-i].emplace_back(M+i, 0);
		}
		for (int i = 0; i < M-1-i; i++) {
			adj[i].emplace_back(M-1-i, 0);
			adj[M-1-i].emplace_back(i, 0);
		}

		for (int i = L; i < M; i++) {
			adj[M+i].emplace_back(-1, 0);
		}

		for (int i = 0; i < M; i++) {
			if (S[i] != '?') {
				assert(S[i] == '0' || S[i] == '1');
				adj[i].emplace_back(M + i, S[i] - '0');
				adj[M + i].emplace_back(i, S[i] - '0');
			}
		}
		for (int i = 0; i < V; i++) assert(adj[i].size() <= 2);

		int rnk = V;
		stack<int> q;
		for (int st = 0; st < V; st++) {
			int cur = st;
			while (cur != -1 && adj[cur].size() == 1) {
				rnk--;
				// remove the chain
				int nxt = adj[cur][0].first;
				int v = adj[cur][0].second;
				adj[cur].clear();
				if (nxt != -1) {
					int z = 0; while (adj[nxt][z].first != cur) z++;
					adj[nxt].erase(adj[nxt].begin() + z);
				}
				cur = nxt;
			}
		}

		for (int st = 0; st < V; st++) {
			assert(adj[st].size() == 2 || adj[st].size() == 0);
		}

	}

	return 0;
}
