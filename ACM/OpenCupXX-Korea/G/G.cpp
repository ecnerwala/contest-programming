#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
const int MAXM = 3.1e5;
int N, M;
int S, T;
vector<pair<int, int>> adj[MAXN];
vector<int> padj[MAXN];
bool reachable[MAXN];
bool inPath[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M >> S >> T; S--, T--;

	for (int e = 0; e < M; e++) {
		int u, v, c; cin >> u >> v >> c; u--, v--;
		adj[u].emplace_back(c, v);
		padj[v].emplace_back(u);
	}

	stack<int> q;
	reachable[T] = true;
	q.push(T);
	while (!q.empty()) {
		int cur = q.top(); q.pop();
		for (int nxt : padj[cur]) {
			if (reachable[nxt]) continue;
			reachable[nxt] = true;
			q.push(nxt);
		}
	}

	if (!reachable[S]) {
		cout << "IMPOSSIBLE" << '\n';
		exit(0);
	}
	
	vector<int> ans;
	for (int cur = S; true; ) {
		assert(reachable[cur]);
		if (cur == T) {
			break;
		} else if (inPath[cur]) {
			cout << "TOO LONG" << '\n';
			exit(0);
		}
		inPath[cur] = true;
		assert(reachable[cur]);
		pair<int, int> bestEdge(int(1e9)+10, -1);
		for (auto e : adj[cur]) {
			if (reachable[e.second]) {
				bestEdge = min(bestEdge, e);
			}
		}
		ans.push_back(bestEdge.first);
		cur = bestEdge.second;
	}

	for (int i = 0; i < int(ans.size()); i++) {
		cout << ans[i] << " \n"[i+1==int(ans.size())];
	}

	return 0;
}
