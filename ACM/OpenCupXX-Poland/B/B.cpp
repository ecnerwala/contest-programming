#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
vector<int> adj[MAXN];

int par[MAXN];
int depth[MAXN];
int maxDepth[MAXN];
int bestChild[MAXN];
void dfsDepth(int cur, int prv) {
	par[cur] = prv;
	depth[cur] = (prv == -1 ? 0 : depth[prv]) + 1;
	maxDepth[cur] = depth[cur];
	bestChild[cur] = -1;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfsDepth(nxt, cur);
		if (maxDepth[nxt] > maxDepth[cur]) {
			maxDepth[cur] = maxDepth[nxt];
			bestChild[cur] = nxt;
		}
	}
}

vector<pair<int, int>> solveOne(int r, int a, int b) {
	dfsDepth(r, -1);
	vector<pair<int, int>> res;
	while (a != r && b != r) {
		if(int(res.size()) >= 5 * N) {
			return {};
		}
		if (bestChild[a] == -1) {
			swap(a, b);
		}
		if (bestChild[a] == -1) {
			// neither side can make progress
			return {};
		}

		// a goes deeper
		pair<int, int> nxt(bestChild[a], b);
		res.push_back(nxt);
		b = par[b];
		a = bestChild[a];
	}
	if (b == r) {
		swap(a, b);
	}
	assert(a == r);
	while (b != -1) {
		pair<int, int> nxt(-1, b);
		res.push_back(nxt);
		b = par[b];
	}
	return res;
}

int getRoot(int c, int a, int b) {
	dfsDepth(c, -1);
	while (a != b) {
		if (depth[a] > depth[b]) a = par[a];
		else b = par[b];
	}
	return a;
}

int A, B;
int C, D;
int RA;
int RC;

void go() {
	cin >> N;
	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	cin >> A >> B >> C >> D;
	RA = getRoot(C, A, B);
	RC = getRoot(A, C, D);
	vector<pair<int, int>> va = solveOne(RA, A, B);
	vector<pair<int, int>> vc = solveOne(RC, C, D);
	/*
	cerr << "RA" << ' ' << RA << '\n';
	cerr << "VA" << '\n';
	for (auto it : va) {
		cerr << it.first << ' ' << it.second << '\n';
	}
	cerr << "RC" << ' ' << RC << '\n';
	cerr << "VC" << '\n';
	for (auto it : vc) {
		cerr << it.first << ' ' << it.second << '\n';
	}
	*/
	if (va.empty() || vc.empty()) {
		cout << -1 << '\n';
		return;
	}
	dfsDepth(RC, -1);
	vector<int> path;
	for (int i = par[RA]; i != RC; i = par[i]) {
		path.push_back(i);
	}
	//cerr << "path" << '\n'; for (auto it : path) cerr << it << '\n';

	vector<int> res;
	for (auto it : va) {
		if (it.first == -1) break;
		res.push_back(it.first);
	}
	for (auto it : path) {
		res.push_back(it);
	}
	reverse(vc.begin(), vc.end());
	for (auto it : vc) {
		res.push_back(it.second);
	}
	cout << res.size() << '\n';
	for (int z = 0; z < int(res.size()); z++) {
		cout << res[z] << " \n"[z+1==int(res.size())];
	}
}

void cleanup() {
	for (int i = 1; i <= N; i++) adj[i] = {};
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int Z; cin >> Z;
	while (Z--) {
		go();
		cleanup();
	}

	return 0;
}
