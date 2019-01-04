#include<bits/stdc++.h>
using namespace std;

template <typename T> T& setmax(T& a, const T& b) {
	if (b > a) a = b;
	return a;
}
template <typename T> T& setmin(T& a, const T& b) {
	if (b < a) a = b;
	return a;
}

const int MAXN = 1.1e5;
int N;
vector<int> adj[MAXN];

int depth[MAXN];
int par[MAXN][20];
int sz[MAXN];
int build_data(int cur, int prv = 0) {
	depth[cur] = depth[prv] + 1;
	par[cur][0] = prv;
	for (int i = 0; par[cur][i]; i++) {
		par[cur][i+1] = par[par[cur][i]][i];
	}
	sz[cur] = 1;
	int res = 0;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		int v = build_data(nxt, cur);
		if (v) res = v;
		sz[cur] += sz[nxt];
	}
	if (res) return res;
	if (sz[cur] * 2 >= N) return cur;
	return 0;
}

int ROOT;

int answer[MAXN];
int downCentroid[MAXN];
int upCentroid[MAXN];

int bsearchCentroid(int bot, int top, int target) {
	assert(sz[top] * 2 >= target);
	if (sz[bot] * 2 >= target) return bot;
	int i;
	for (i = 0; (1 << i) <= depth[bot] - depth[top]; i++);
	for (; i >= 0; i--) {
		if ((1 << i) <= depth[bot] - depth[top] && sz[par[bot][i]] * 2 < target) {
			bot = par[bot][i];
		}
	}
	return par[bot][0];
}

int move2Sz(int cent, int target) {
	assert(target >= sz[cent]);
	assert(2 * sz[cent] >= target);
	int res = target - sz[cent];
	if (!adj[cent].empty()) {
		setmax(res, sz[adj[cent][0]]);
	}
	return target - res;
}

int getDownCentroid(int cur) {
	if (adj[cur].empty() || 2 * sz[adj[cur][0]] < sz[cur]) {
		return cur;
	} else {
		return bsearchCentroid(downCentroid[adj[cur][0]], cur, sz[cur]);
	}
}

void dfsDowns(int cur, int prv) {
	for (int nxt : adj[cur]) {
		assert(nxt != prv);
		dfsDowns(nxt, cur);
	}
	downCentroid[cur] = getDownCentroid(cur);
	setmax(answer[prv], move2Sz(downCentroid[cur], sz[cur]));
}

void dfsUps(int cur, int prv) {
	upCentroid[cur] = bsearchCentroid(upCentroid[prv], ROOT, N - sz[cur]);
	setmax(answer[cur], move2Sz(upCentroid[cur], N - sz[cur]));
	for (int nxt : adj[cur]) {
		assert(nxt != prv);
		dfsUps(nxt, cur);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0), cerr.tie(0);
	cin >> N;
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v;
		assert(1 <= u && u <= N);
		assert(1 <= v && v <= N);
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	ROOT = build_data(1);
	memset(par, 0, sizeof(par));
	memset(depth, 0, sizeof(depth));
	memset(sz, 0, sizeof(sz));
	build_data(ROOT);
	for (int i = 1; i <= N; i++) {
		sort(adj[i].begin(), adj[i].end(), [](int a, int b) { return sz[a] > sz[b]; });
		if (i != ROOT) {
			assert(par[i][0]);
			assert(adj[i][0] == par[i][0]);
			adj[i].erase(adj[i].begin());
		}
	}

	for (int cur : adj[ROOT]) {
		dfsDowns(cur, ROOT);
	}
	auto tops = adj[ROOT];
	for (int i = 0; i < int(tops.size()); i++) {
		int cur = tops[i];
		if (i == 0) {
			assert(*adj[ROOT].begin() == cur);
			adj[ROOT].erase(adj[ROOT].begin());
		} else {
			assert(adj[ROOT][i-1] == cur);
			adj[ROOT][i-1] = tops[i-1];
		}

		sz[ROOT] -= sz[cur];

		downCentroid[ROOT] = getDownCentroid(ROOT);
		upCentroid[ROOT] = downCentroid[ROOT];
		dfsUps(cur, ROOT);

		sz[ROOT] += sz[cur];
	}
	adj[ROOT] = tops;

	int bestMove = *min_element(answer + 1, answer + N + 1);
	cout << N - bestMove << '\n';

	return 0;
}
