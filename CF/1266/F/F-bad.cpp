#include<bits/stdc++.h>
using namespace std;

const int MAXN = 5.1e5;
int N;
int deg[MAXN];
pair<int, int> edges[MAXN];
vector<int> adj[MAXN];
vector<int> nei[MAXN];

pair<int, int> maxDist(int cur, int prv) {
	pair<int, int> ans(0, cur);
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		pair<int, int> ch = maxDist(nxt, cur);
		ch.first++;
		ans = max(ans, ch);
	}
	return ans;
}

int diameter() {
	return maxDist(maxDist(0, -1).second, -1).first;
}

bool alive[MAXN];
bool justCulled[MAXN];

int cntDeg[MAXN]; // star without center
int cntOne[MAXN]; // star with justCulled
int cntTwo[MAXN]; // edge-star

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v; u--, v--;
		edges[i] = {u, v};
		adj[u].push_back(v);
		adj[v].push_back(u);
		deg[u] ++;
		deg[v] ++;
	}

	for (int i = 0; i < N-1; i++) {
		int u = edges[i].first, v = edges[i].second;
		if (deg[u] > deg[v]) swap(u, v);
		assert(deg[u] <= deg[v]);
		nei[u].push_back(v);
	}

	vector<int> ans; ans.reserve(2*N);

	int nAlive = N;
	for (int i = 0; i < N; i++) alive[i] = true;

	int bestDeg = 0;
	for (int i = 0; i < N; i++) {
		bestDeg = max(bestDeg, deg[i]);
		cntDeg[deg[i]]++;
	}
	vector<int> wasJustCulled;
	int bestOne = 0;
	for (int i = 0; i < N; i++) {
		justCulled[i] = true;
		wasJustCulled.push_back(i);
		bestOne = max(bestOne, deg[i] + justCulled[i]);
		cntOne[deg[i] + justCulled[i]]++;
	}
	int bestTwo = 0;
	for (int i = 0; i < N; i++) {
		for (int j : adj[i]) {
			if (i < j) {
				bestTwo = max(bestTwo, deg[i] + deg[j]);
				cntTwo[deg[i] + deg[j]] ++;
			}
		}
	}

	vector<int> ncull;
	for (int i = 0; i < N; i++) {
		if (deg[i] <= 1) ncull.push_back(i);
	}

	vector<int> nncull;

	ans.push_back(0); // filler

	while (nAlive >= 1) {
		cerr << ans.size() << ' ' << nAlive << ' ' << bestOne << ' ' << bestDeg << ' ' << bestTwo << '\n';
		ans.push_back(bestOne);
		ans.push_back(max(bestDeg, bestTwo - 2));

		for (int i : wasJustCulled) {
			cntOne[deg[i] + justCulled[i]]--;
			justCulled[i] = false;
			cntOne[deg[i] + justCulled[i]]++;
		}
		wasJustCulled.clear();

		for (int v : ncull) {
			alive[v] = false;
			assert(deg[v] <= 1);
			nAlive--;
		}

		for (int v : ncull) {
			cntOne[deg[v]+justCulled[v]] --;
			cntDeg[deg[v]] --;
			for (int i : adj[v]) {
				if (!alive[i]) continue;
				cntTwo[deg[v] + deg[i]] --; // delete this edge
			}
		}

		for (int v : ncull) {
			assert(deg[v] <= 1);
			int i = -1;
			for (int x : adj[v]) {
				if (alive[x]) {
					i = x;
					break;
				}
			}

			if (i == -1) {
				// must've been culled this time
				continue;
			}

			cntDeg[deg[i]] --;
			cntOne[deg[i] + justCulled[i]] --;
			for (int j : adj[i]) {
				if (!alive[j]) continue;
				cntTwo[deg[i] + deg[j]] --;
			}
			if (!justCulled[i]) {
				justCulled[i] = true;
				wasJustCulled.push_back(i);
			}
			deg[i] --;
			if (deg[i] == 1) {
				nncull.push_back(i);
			}

			cntDeg[deg[i]] ++;
			cntOne[deg[i] + justCulled[i]] ++;
			for (int j : adj[i]) {
				if (!alive[j]) continue;
				cntTwo[deg[i] + deg[j]] ++;
			}
		}

		while (bestDeg > 0 && !cntDeg[bestDeg]) --bestDeg;
		while (bestOne > 0 && !cntOne[bestOne]) --bestOne;
		while (bestTwo > 0 && !cntTwo[bestTwo]) --bestTwo;

		//cerr << "Deg" << '\n'; for (int i = 0; i <= N+2; i++) if (cntDeg[i]) cerr << i << " *" << cntDeg[i] << '\n';
		//cerr << "One" << '\n'; for (int i = 0; i <= N+2; i++) if (cntOne[i]) cerr << i << " *" << cntOne[i] << '\n';
		//cerr << "Two" << '\n'; for (int i = 0; i <= N+2; i++) if (cntTwo[i]) cerr << i << " *" << cntTwo[i] << '\n';

		swap(nncull, ncull);
		nncull.clear();
	}

	while (int(ans.size()) < N+1) ans.push_back(1);
	int diam = diameter();
	for (int i = 1; i <= N; i++) {
		if (i <= diam) {
			ans[i] = max(ans[i], 2);
		} else {
			ans[i] = max(ans[i], 1);
		}
		cout << ans[i] << " \n"[i==N];
	}

	return 0;
}
