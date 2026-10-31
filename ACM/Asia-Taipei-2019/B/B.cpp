#include<bits/stdc++.h>
using namespace std;

template <typename T> void setmax(T& a, const T& b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, const T& b) { if (b < a) a = b; }
// min-cost so the tree produces the edge

const int MAXN = 1.1e5;
int N;
vector<int> adj[MAXN];

int dpProduces[MAXN];
int dpMonitors[MAXN];
int dpNeeds[MAXN];

void dfs(int cur, int prv) {
	int costTake = 1;
	int costAllMonitor = 0;
	int bestProduces = 1e9;
	int bestProduceSavings = 1e9;
	int bestSavings = 0;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfs(nxt, cur);

		costTake += dpNeeds[nxt];

		costAllMonitor += dpMonitors[nxt];
		setmin(bestProduceSavings, (dpNeeds[nxt] - dpMonitors[nxt]) + bestProduces);
		setmin(bestProduceSavings, bestSavings + (dpProduces[nxt] - dpMonitors[nxt]));
		setmin(bestProduces, dpProduces[nxt] - dpMonitors[nxt]);
		setmin(bestSavings, dpNeeds[nxt] - dpMonitors[nxt]);
	}

	// monitors if at least one child produces, and at most one child needs
	dpProduces[cur] = costTake;
	setmin(dpProduces[cur], costAllMonitor + bestProduces);
	dpMonitors[cur] = dpProduces[cur];
	setmin(dpMonitors[cur], costAllMonitor + bestProduceSavings);
	dpNeeds[cur] = dpMonitors[cur];
	setmin(dpNeeds[cur], costAllMonitor + bestSavings);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	cin >> N;
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	assert(N >= 2);
	for (int i = 0; i < N; i++) {
		if (adj[i].size() == 1) {
			int j = adj[i][0];
			dfs(j, i);
			// if you'd PMU i, then you might as well PMU j instead, so we can assume that i is not PMU
			// This means that the subtree must produce the edge
			cout << dpProduces[j] << '\n';
			exit(0);
		}
	}
	assert(false);
	return 0;
}
