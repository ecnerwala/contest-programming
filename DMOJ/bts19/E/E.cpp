#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
vector<int> adj[MAXN];

int furthestDown[MAXN];
int diamDown[MAXN];

int furthestUp[MAXN];
int diamUp[MAXN];

vector<int> diams[MAXN];

void dfsDown(int cur, int prv) {
	furthestDown[cur] = 0;
	diamDown[cur] = 0;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfsDown(nxt, cur);
		diamDown[cur] = max(diamDown[cur], diamDown[nxt]);
		diamDown[cur] = max(diamDown[cur], furthestDown[cur] + (furthestDown[nxt] + 1));
		furthestDown[cur] = max(furthestDown[cur], furthestDown[nxt] + 1);

		diams[cur].push_back(diamDown[nxt]);
	}
}

void dfsUp(int cur, int prv) {
	vector<pair<int, int>> bestDiams;
	vector<pair<int, int>> bestFar;
	bestDiams.emplace_back((prv == -1 ? 0 : diamUp[cur]), prv);
	bestFar.emplace_back((prv == -1 ? 0 : furthestUp[cur] + 1), prv);
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		bestDiams.emplace_back(diamDown[nxt], nxt);
		bestFar.emplace_back(furthestDown[nxt] + 1, nxt);
	}

	sort(bestDiams.begin(), bestDiams.end());
	reverse(bestDiams.begin(), bestDiams.end());
	sort(bestFar.begin(), bestFar.end());
	reverse(bestFar.begin(), bestFar.end());

	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		diamUp[nxt] = (bestDiams[0].second == nxt) ? bestDiams[1].first : bestDiams[0].first;
		furthestUp[nxt] = (bestFar[0].second == nxt) ? bestFar[1].first : bestFar[0].first;
		diamUp[nxt] = max(diamUp[nxt], furthestUp[nxt]);
		
		// update the nxt diameter
		if (bestFar.size() >= 2) {
			if (bestFar[0].second == nxt) {
				if (bestFar.size() >= 3) {
					diamUp[nxt] = max(diamUp[nxt], bestFar[1].first + bestFar[2].first);
				}
			} else if (bestFar[1].second == nxt) {
				if (bestFar.size() >= 3) {
					diamUp[nxt] = max(diamUp[nxt], bestFar[0].first + bestFar[2].first);
				}
			} else {
				diamUp[nxt] = max(diamUp[nxt], bestFar[0].first + bestFar[1].first);
			}
		}

		diams[nxt].push_back(diamUp[nxt]);

		dfsUp(nxt, cur);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int Q;
	cin >> N >> Q;
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	dfsDown(0, -1);
	dfsUp(0, -1);

	for (int i = 0; i < N; i++) {
		sort(diams[i].begin(), diams[i].end());
		reverse(diams[i].begin(), diams[i].end());
	}

	while (Q--) {
		int j, k; cin >> j >> k;
		j--, k--;
		if (k >= int(diams[j].size())) {
			cout << -1 << '\n';
		} else {
			cout << diams[j][k]+1 << '\n';
		}
	}

	return 0;
}
